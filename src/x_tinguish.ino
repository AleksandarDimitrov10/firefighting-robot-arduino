#include <Servo.h>

// ----- Pin Definitions -----
#define SENS 2           // Flame sensor pin
#define RELE 7           // Relay module pin for water pump
#define SERVO 11         // Servo motor pin
#define WATER_SENSOR 3   // Water level sensor pin
#define LED 13           // LED pin for low-water warning

// Relay states
#define RON LOW          // Relay ON: active when pin is LOW
#define ROF HIGH         // Relay OFF: inactive when pin is HIGH

Servo servo;

// ----- Flame Detection Variables -----
bool flameDetected = false;
bool flamePreviouslyDetected = false;

unsigned long flameStartTime = 0;
const int debounceTime = 0;

// ----- Servo Movement Variables -----
int position = 0;
bool movingUp = true;

// ----- Low-Water LED Blinking Variables -----
unsigned long previousBlinkTime = 0;
const unsigned long blinkInterval = 500;
bool ledState = false;

void setup() {
  pinMode(SENS, INPUT_PULLUP);
  pinMode(RELE, OUTPUT);
  pinMode(WATER_SENSOR, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  digitalWrite(RELE, ROF);

  servo.attach(SERVO);
  servo.write(0);
}

void loop() {
  checkWaterLevel();
  checkFlameSensor();
  handleFireResponse();
}

void checkWaterLevel() {
  int waterSensorState = digitalRead(WATER_SENSOR);

  if (waterSensorState == HIGH) {
    // No water detected: blink warning LED
    if (millis() - previousBlinkTime >= blinkInterval) {
      previousBlinkTime = millis();
      ledState = !ledState;
      digitalWrite(LED, ledState ? HIGH : LOW);
    }
  } else {
    // Water detected: keep warning LED off
    digitalWrite(LED, LOW);
    ledState = false;
  }
}

void checkFlameSensor() {
  int sensorState = digitalRead(SENS);

  // LOW means flame detected for this sensor setup
  if (sensorState == LOW) {
    if (flameStartTime == 0) {
      flameStartTime = millis();
    }

    if (millis() - flameStartTime > debounceTime) {
      flameDetected = true;
    }
  } else {
    flameStartTime = 0;
    flameDetected = false;
  }
}

void handleFireResponse() {
  if (flameDetected && !flamePreviouslyDetected) {
    flamePreviouslyDetected = true;

    // Activate water pump
    digitalWrite(RELE, RON);

    // Short pause after first detection
    delay(500);
    return;
  }

  flamePreviouslyDetected = flameDetected;

  if (flameDetected) {
    // Keep spraying and hold the servo position
    digitalWrite(RELE, RON);
    servo.write(position);
  } else {
    // No flame detected: stop pump and continue scanning
    digitalWrite(RELE, ROF);
    scanEnvironment();
  }
}

void scanEnvironment() {
  if (movingUp) {
    position += 1;

    if (position >= 180) {
      movingUp = false;
    }
  } else {
    position -= 1;

    if (position <= 0) {
      movingUp = true;
    }
  }

  servo.write(position);
  delay(15);
}
