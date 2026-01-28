#include <Bluepad32.h>
#include <ESP32Servo.h>

// ==================== PINS ====================
const int motor1_enable_pin   = 13;
const int motor2_enable_pin   = 25;
const int motor1_forward_pin  = 12;
const int motor1_backward_pin = 14;
const int motor2_forward_pin  = 27;
const int motor2_backward_pin = 26;

const int servo1_pin = 2;   // Left Shoulder
const int servo2_pin = 4;   // Left Elbow
const int servo3_pin = 5;   // Right Shoulder
const int servo4_pin = 18;  // Right Elbow
const int servo5_pin = 19;  // Waist

const int buzzer = 15;

// ==================== PWM CHANNELS ====================
const int motor1_pwm_channel = 10;
const int motor2_pwm_channel = 11;
const int PWM_FREQ = 5000;
const int PWM_RES  = 8;

// ==================== STATE ====================
ControllerPtr activeController = nullptr;
Servo servo1, servo2, servo3, servo4, servo5;

unsigned long comboStart = 0;
bool comboActive = false;

// ==================== UTILS ====================
static inline uint8_t clamp180(int v) {
  return v < 0 ? 0 : (v > 180 ? 180 : v);
}
static inline int16_t clamp255(int v) {
  return v < -255 ? -255 : (v > 255 ? 255 : v);
}

// ==================== MOTORS ====================
void driveM1(int speed) {
  speed = clamp255(speed);
  digitalWrite(motor1_forward_pin, speed > 0);
  digitalWrite(motor1_backward_pin, speed < 0);
  ledcWrite(motor1_pwm_channel, abs(speed));
}

void driveM2(int speed) {
  speed = clamp255(speed);
  digitalWrite(motor2_forward_pin, speed > 0);
  digitalWrite(motor2_backward_pin, speed < 0);
  ledcWrite(motor2_pwm_channel, abs(speed));
}

void driveMotors(int l, int r) {
  driveM1(l);
  driveM2(r);
}

// ==================== ADVANCED MOVES ====================

// --- ANKLE BREAKER: Low hook attack for heavy opponents ---
void performAnkleBreaker() {
  // 1. Drop Right Arm Low (Hook Position)
  servo3.write(45);  // Right Shoulder Down
  servo4.write(30);  // Right Elbow Extended Low
  delay(150);
  
  // 2. Drive & Hook (Curve Right)
  driveMotors(200, 100); 
  delay(300);
  
  // 3. RIP UP (Use opponent's weight to tip them)
  servo3.write(160); // Shoulder Rips Up
  servo4.write(90);  // Elbow Curls
  driveMotors(255, 255); // Full Ram Forward
  delay(250);
  
  // 4. Reset
  driveMotors(0, 0);
  servo3.write(90);
  servo4.write(90);
}

void hipTwistPunch() {
  servo2.write(55); delay(100);
  servo2.write(125);
  servo1.write(180);
  delay(150);
  servo1.write(90);
  servo2.write(90);
}

void doubleJab() {
  for (int i = 0; i < 2; i++) {
    servo1.write(170); delay(100);
    servo1.write(90);  delay(100);
  }
}

void shoulderBash() {
  servo2.write(40);
  driveMotors(200, 200);
  delay(250);
  driveMotors(0, 0);
  servo2.write(90);
}

void feintPunch() {
  servo1.write(140); delay(80);
  servo1.write(90);  delay(60);
  servo1.write(180); delay(120);
  servo1.write(90);
}

void sideStep(bool left) {
  driveMotors(left ? -180 : 180, left ? 180 : -180);
  delay(180);
  driveMotors(0, 0);
  doubleJab();
}

// ==================== GAMEPAD ====================
void processGamepad(ControllerPtr ctl) {

  // ---------- EMERGENCY DISCONNECT ----------
  if (ctl->miscSystem() && ctl->miscHome()) {
    if (!comboActive) {
      comboActive = true;
      comboStart = millis();
    } else if (millis() - comboStart > 3000) {
      ctl->disconnect();
    }
    return;
  } else comboActive = false;

  int lx = ctl->axisX();
  int ly = ctl->axisY();
  int rx = ctl->axisRX();
  int ry = ctl->axisRY();

  int l2 = ctl->brake();
  int r2 = ctl->throttle();
  if (l2 < 50) l2 = 0;
  if (r2 < 50) r2 = 0;
  if (l2 > r2) r2 = 0;
  else l2 = 0;

  // ---------- ATTACKS ----------

  // SQUARE (X) -> ANKLE BREAKER (or Feint if L2 held)
  if (ctl->x()) {
     if (l2 > 200) feintPunch();
     else performAnkleBreaker(); 
     return; 
  }

  // Stick Clicks
  if (ctl->thumbR()) { hipTwistPunch(); return; }
  if (ctl->thumbL()) { doubleJab(); return; }
  
  // Select Button
  if (ctl->miscSystem()) { shoulderBash(); return; }

  // Counter Attacks (R1 + DPad)
  if (ctl->r1()) {
    uint8_t d = ctl->dpad();
    if (d & DPAD_LEFT)  { sideStep(true);  return; }
    if (d & DPAD_RIGHT) { sideStep(false); return; }
  }

  // ---------- DRIVE ----------
  int L = 0, R = 0;
  uint8_t d = ctl->dpad();
  if (d & DPAD_UP)        L = R = 200;
  else if (d & DPAD_DOWN) L = R = -200;
  else if (d & DPAD_LEFT)  { L = -200; R = 200; }
  else if (d & DPAD_RIGHT) { L = 200;  R = -200; }
  driveMotors(L, R);

  // ---------- SERVOS ----------
  servo1.write(clamp180(map(ly,  511, -512, 0, 180)));
  servo2.write(clamp180(map(lx,  511, -512, 0, 180)));
  servo3.write(clamp180(map(ry, -511,  512, 0, 180)));
  servo4.write(clamp180(map(rx,  511, -512, 0, 180)));

  if (l2 > 0)      servo5.write(180);
  else if (r2 > 0) servo5.write(0);
  else             servo5.write(90);
}

// ==================== CONTROLLERS ====================
void processControllers() {
  if (activeController &&
      activeController->isConnected() &&
      activeController->hasData() &&
      activeController->isGamepad()) {
    processGamepad(activeController);
  }
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);

  pinMode(motor1_forward_pin, OUTPUT);
  pinMode(motor1_backward_pin, OUTPUT);
  pinMode(motor2_forward_pin, OUTPUT);
  pinMode(motor2_backward_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // PWM Setup
  ledcSetup(motor1_pwm_channel, PWM_FREQ, PWM_RES);
  ledcSetup(motor2_pwm_channel, PWM_FREQ, PWM_RES);
  ledcAttachPin(motor1_enable_pin, motor1_pwm_channel);
  ledcAttachPin(motor2_enable_pin, motor2_pwm_channel);

  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
  servo3.attach(servo3_pin);
  servo4.attach(servo4_pin);
  servo5.attach(servo5_pin);

  BP32.setup(
    [](ControllerPtr c){ activeController = c; },
    [](ControllerPtr c){ activeController = nullptr; }
  );

  Serial.println("🤖 WAR BOT: ANKLE BREAKER READY");
}

// ==================== LOOP ====================
void loop() {
  BP32.update();
  processControllers();
  vTaskDelay(pdMS_TO_TICKS(1)); 
}
