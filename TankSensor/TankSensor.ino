#include <NewPing.h>

#define TRIGGER_PIN   12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN      11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE  150 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define TANK_HEIGHT   120
#define OUTPUT_PIN    9

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop()
{
  //wait between pings
  delay(30000);

  //ping the distance 10 times, discards out of range results and returns microsecond result
  int timeForPing = sonar.ping_median(10);

  //convert the microsecond result to centimeters
  int distanceCM = sonar.convert_cm(timeForPing);

  //check to ensure we've not got a result over the max height of the tank
  if(distanceCM > TANK_HEIGHT)
  {
    distanceCM = TANK_HEIGHT;
  }

  //output PWM is on a scale of 0-255 for 0-5v
  //need to cast both values to float for division to work
  float mod = (float)distanceCM / (float)TANK_HEIGHT;

  //measurements are in reverse for the UI, needs to be higher result for lower measurements
  int outputPWM = 255 - (int)(mod * 255);
  
  Serial.print("Distance ");
  Serial.print(distanceCM);

  Serial.print(" / 150 = ");
  Serial.println(mod);

  Serial.print(" * 255 = ");
  Serial.println(outputPWM);

  //set the output pin to its new value
  analogWrite(OUTPUT_PIN, outputPWM);
}
