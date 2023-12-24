// Define the connections between the Arduino and A4988 driver
// for max speed of motor ML17A3, delay is 350us with full step (LLL) and 1us with 1/16th step (HHH).
#include<math.h>
#define DIR_PIN 2
#define STEP_PIN 3
#define SLEEP_PIN 4
#define FINE_TUNE_PIN 8
#define SET_ZERO_PIN 9

#define POTENTIOMETER_PIN A0
#define delayMin 1
#define delayMax 500

int D;
int potValue;
bool dir = 1;
float absPos = 0;
float targetPos = 0;

void movetoAngle(float finalPos) {
  //finalPos = fmodf(fmodf(finalPos + 180, 360.0) + 360.0, 360.0) - 180;
  //if(finalPos == -180){finalPos = 180;}
  //finalPos = fmodf(finalPos, 360.0);
  float x = fmodf(fmodf(finalPos - absPos, 360.0) + 360.0, 360.0);
  dir = (0 < x && x < 180) ? 1 : 0;
  digitalWrite(DIR_PIN, dir);
//  static int prevMicros = 0;
//  D = 30;
//  if (micros() - prevMicros >= D && abs(finalPos - absPos) > 0.05) {
//    digitalWrite(STEP_PIN, HIGH);
//    delayMicroseconds(D);
//    digitalWrite(STEP_PIN, LOW);
//    prevMicros = micros();
//    absPos = fmodf((absPos + (2 * dir - 1) * 1.8 / 16.0) - 540, 360.0) + 180;
//  }

      int stepstoMove = (180.0 - abs(x - 180.0))/(1.8/16.0);
    int D = 300;
    
    for(int i = 0; i < stepstoMove; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(D);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(D);
    }
    absPos = finalPos;
}

void setup() {
  // Set up the motor control pins
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(SLEEP_PIN, OUTPUT);
  pinMode(FINE_TUNE_PIN, INPUT);
  pinMode(SET_ZERO_PIN, INPUT);
  digitalWrite(SLEEP_PIN, !LOW);
  Serial.begin(9600);
}

void loop() {
  bool moving = 1;
  if (digitalRead(SET_ZERO_PIN)) {
    absPos = 0;
  }
  if (Serial.available()) {
    targetPos = Serial.parseFloat();
    targetPos = fmodf(fmodf(targetPos, 360.0) + 360.0, 360.0);
    movetoAngle(targetPos);
  }
  static long count = 0;
    if(millis() - count >= 500){
      Serial.print("absPos: ");
      Serial.println(absPos);
      count = millis();
      }

  while (moving) {
    potValue = analogRead(POTENTIOMETER_PIN);
    // Set the direction based on the potentiometer value
    dir = (potValue > 512) ? HIGH : LOW;
    digitalWrite(DIR_PIN, dir);
    if (potValue > 480 && potValue < 524) {
      moving = 0;
      //digitalWrite(SLEEP_PIN, !HIGH);
      break;
    }
    else {
      //digitalWrite(SLEEP_PIN, !LOW);
      moving = 1;
      if (digitalRead(FINE_TUNE_PIN)) {
        D = 10000;
      }
      else {
        D = (delayMin - delayMax) / 512.0 * abs(potValue - 512) + delayMax;
      }
    }
    // Step the motor at the desired speed
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(D);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(D);
    absPos = fmodf((absPos + (2 * dir - 1) * 1.8 / 16.0) - 540, 360.0) + 180;
  }
  // Print the speed value for debugging
  //  Serial.print("Delay, PotValue, SLEEP_status, absPos: ");
  //  Serial.print(D);
  //  Serial.print(", ");
  //  Serial.print(potValue);
  //  Serial.print(", ");
  //  Serial.print(digitalRead(SLEEP_PIN));
  //  Serial.print(", ");
  //  Serial.println(absPos);

  //  static long count = 0;
  //  if(millis() - count >= 500){
  //    Serial.print("absPos: ");
  //    Serial.println(absPos);
  //    count = millis();
  //    }

  //  delay(2);  // Add a small delay for stability
}
