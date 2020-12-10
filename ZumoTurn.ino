// Example on how to read from the gyrometer
// built into the Pololu Zumo 32U4 Arduino-based robot.

#include <Zumo32U4.h>
#include "TurnSensor.h"

L3G gyro;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
int targetDegrees = 90;
int motorSpeed;
double kP = 5;
int error;

// --- Helper functions
int32_t getAngle() {
  // turnAngle is a variable defined in TurnSensor.cpp
  // This fancy math converts the number into degrees turned since the
  // last sensor reset.
  return (((int32_t)turnAngle >> 16) * 360) >> 16;
}

// --- Setup Method
void setup() {
  motors.setSpeeds(0, 0);
  Serial.begin(115200);
  turnSensorSetup();
  delay(500);
  turnSensorReset();
  lcd.clear();
}

// --- Main program loop
void loop() {
  
  // Read the sensors
  turnSensorUpdate();
  int32_t angle = getAngle();

  // Update the display
  lcd.gotoXY(0, 0);
  lcd.print(angle);
  lcd.print(" ");

  error = targetDegrees - (int) angle;
  motorSpeed = error * kP;

  //trying to adjust for low power near the target
  if (error < 10 && error > -10) {
    motorSpeed += 30;
  }
  if (error > 2 || error < -2) {
    motors.setSpeeds(-motorSpeed, motorSpeed);
  }
}
