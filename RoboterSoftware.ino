#include "hardware.c"
#include <math.h>


void setup() {
  // put your setup code here, to run once:
  hardware_setup();
  //Serial.begin(9600);
}

char x = 1;

void loop() {
  //delay(1000);
  //side_info left = get_side_info(RIGHT_SIDE);
  //double angle = atan2(-8.0, 160.0);
  /*while (!digitalRead(IR_FRONT_RIGHT) || !digitalRead(IR_FRONT_LEFT))
  {
    stop_motors();
    delay(500);
  }*/
  //drive_straight(80, LEFT_SIDE, 200);
  for (int i = 0; i < 256; i++)
  {
    drive(x, i, i);
    delay(2);
  }
  delay(500);
  for (int i = 255; i >= 0; i--)
  {
    drive(x, i, i);
    delay(2);
  }
  if (x)
  {
    x = 0;
  }
  else
  {
    x = 1;
  }
  delay(500);
}

