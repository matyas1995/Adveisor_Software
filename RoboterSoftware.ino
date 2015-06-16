#include <AFMotor.h>
#include <Arduino.h>
#include "subroutinen.h"
#include "hardware.h"

void setup() {
  hardware_setup();
  pinMode(43, OUTPUT);
  pinMode(37, OUTPUT);
  digitalWrite(43, HIGH);
  digitalWrite(37, LOW);
  //Serial.begin(9600);
}

void loop() {
  //drive_straight(100, LEFT_SIDE, 255);
  /*struct side_info Seite = get_side_info(RIGHT_SIDE);
  Serial.println(Seite.average_distance);
  Serial.println(Seite.angle);
  Serial.println(get_dist(sensor1));
  Serial.println(get_dist(sensor2));
  delay(1000);*/
  drive(1, 255, 255);
}

