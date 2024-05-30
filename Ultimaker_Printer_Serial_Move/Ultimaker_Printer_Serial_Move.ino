#include "TMC2209.h" // TMC2209 ライブラリをインクルード

// ピンの定義
const int xEnablePin = 2;    // X軸 Enable pin
const int xDirPin = 3;       // X軸 Direction pin
const int xStepPin = 4;      // X軸 Step pin

const int yEnablePin = 5;    // Y軸 Enable pin
const int yDirPin = 6;       // Y軸 Direction pin
const int yStepPin = 9;      // Y軸 Step pin

const int zEnablePin = 23;    // Z軸 Enable pin
const int zDirPin = 22;       // Z軸 Direction pin
const int zStepPin = 21;      // Z軸 Step pin

#define X_SERIAL_PORT Serial1 // X軸通信ポート
#define Y_SERIAL_PORT Serial2 // Y軸通信ポート
#define Z_SERIAL_PORT Serial3 // Z軸通信ポート

#define X_LIMIT A2
#define Y_LIMIT A3
#define Z_LIMIT A4

const float xStepPerRevolution = 200.0; // X軸モータのステップ数
const float xMicroSteps = 16.0;         // X軸のマイクロステップ数
const float xMmPerRevolution = 78.74;     // X軸の1回転あたりの移動距離

const float yStepPerRevolution = 200.0; // Y軸モータのステップ数
const float yMicroSteps = 16.0;         // Y軸のマイクロステップ数
const float yMmPerRevolution = 78.74;     // Y軸の1回転あたりの移動距離

const float zStepPerRevolution = 200.0; // Z軸モータのステップ数
const float zMicroSteps = 16.0;         // Z軸のマイクロステップ数
const float zMmPerRevolution = 3.0;     // Z軸の1回転あたりの移動距離

// ドライバのインスタンス生成
TMC2209 driverX;
TMC2209 driverY;
TMC2209 driverZ;

void setup() {
  Serial.begin(115200);
  pinMode(xEnablePin, OUTPUT);
  pinMode(xDirPin, OUTPUT);
  pinMode(xStepPin, OUTPUT);
  pinMode(yEnablePin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(zEnablePin, OUTPUT);
  pinMode(zDirPin, OUTPUT);
  pinMode(zStepPin, OUTPUT);
  pinMode(X_LIMIT, INPUT);
  pinMode(Y_LIMIT, INPUT);
  pinMode(Z_LIMIT, INPUT);

  // ドライバを有効にする
  digitalWrite(xEnablePin, LOW);
  digitalWrite(yEnablePin, LOW);
  digitalWrite(zEnablePin, LOW);

  // シリアル通信の開始
  X_SERIAL_PORT.begin(115200);
  Y_SERIAL_PORT.begin(115200);
  Z_SERIAL_PORT.begin(115200);

  // TMC2209 のセットアップ
  driverX.setup(X_SERIAL_PORT);
  driverY.setup(Y_SERIAL_PORT);
  driverZ.setup(Z_SERIAL_PORT);
  
  // ドライバの有効化
  driverX.setHardwareEnablePin(xEnablePin);
  driverY.setHardwareEnablePin(yEnablePin);
  driverZ.setHardwareEnablePin(zEnablePin);
  
  driverX.enable();
  driverY.enable();
  driverZ.enable();

  // マイクロステップと電流の設定
  driverX.setMicrostepsPerStep(xMicroSteps);  // X軸のマイクロステップ数を設定
  driverX.setRunCurrent(100);                 // X軸の動作時の電流をパーセンテージで設定
  driverX.setHoldCurrent(25);                 // X軸の保持時の電流をパーセンテージで設定
  
  driverY.setMicrostepsPerStep(yMicroSteps);  // Y軸のマイクロステップ数を設定
  driverY.setRunCurrent(100);                 // Y軸の動作時の電流をパーセンテージで設定
  driverY.setHoldCurrent(25);                 // Y軸の保持時の電流をパーセンテージで設定
  
  driverZ.setMicrostepsPerStep(zMicroSteps);  // Z軸のマイクロステップ数を設定
  driverZ.setRunCurrent(100);                 // Z軸の動作時の電流をパーセンテージで設定
  driverZ.setHoldCurrent(25);                 // Z軸の保持時の電流をパーセンテージで設定
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "H") {
      homePosition();
    } else {
      char axis = input.charAt(0);
      float distanceMM = input.substring(1).toFloat();
      moveMotor(axis, distanceMM);
    }
  }
}

void moveMotor(char axis, float distanceMM) {
  int steps;
  bool direction = distanceMM > 0;
  int stepPin, dirPin, enablePin;

  switch (axis) {
    case 'X':
      steps = (distanceMM / xMmPerRevolution) * xStepPerRevolution * xMicroSteps;
      stepPin = xStepPin;
      dirPin = xDirPin;
      enablePin = xEnablePin;
      driverX.enable();
      break;
    case 'Y':
      steps = (distanceMM / yMmPerRevolution) * yStepPerRevolution * yMicroSteps;
      stepPin = yStepPin;
      dirPin = yDirPin;
      enablePin = yEnablePin;
      driverY.enable();
      break;
    case 'Z':
      steps = (distanceMM / zMmPerRevolution) * zStepPerRevolution * zMicroSteps;
      stepPin = zStepPin;
      dirPin = zDirPin;
      enablePin = zEnablePin;
      driverZ.enable();
      break;
    default:
      return; // 無効な軸
  }

  digitalWrite(dirPin, direction);

  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100); // モータの速度とトルクに応じてこの値を調整
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100); // モータの速度とトルクに応じてこの値を調整
  }
  Serial.println("Stopped");
  // ドライバを無効にする
  digitalWrite(enablePin, HIGH);
}

void homePosition() {
  // X軸をホームポジションに移動
  driverX.enable();
  digitalWrite(xDirPin, LOW); // ホームポジションの方向に設定（反時計回り）
  while (analogRead(X_LIMIT) < 900) {
    digitalWrite(xStepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(xStepPin, LOW);
    delayMicroseconds(100);
  }
  driverX.disable();

  // Y軸をホームポジションに移動
  driverY.enable();
  digitalWrite(yDirPin, LOW); // ホームポジションの方向に設定（反時計回り）
  while (analogRead(Y_LIMIT) < 900) {
    digitalWrite(yStepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(yStepPin, LOW);
    delayMicroseconds(100);
  }
  driverY.disable();

  // Z軸をホームポジションに移動
  driverZ.enable();
  digitalWrite(zDirPin, LOW); // ホームポジションの方向に設定（反時計回り）
  while (analogRead(Z_LIMIT) < 900) {
    digitalWrite(zStepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(zStepPin, LOW);
    delayMicroseconds(100);
  }
  driverZ.disable();
}
