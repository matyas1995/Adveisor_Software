#include "hardware.c"
#include <math.h>

void setup() {
  // put your setup code here, to run once:
  hardware_setup();
  Serial.begin(9600);
}

void loop() {
  //delay(1000);
  side_info left = get_side_info(RIGHT_SIDE);
  double angle = atan2(-8.0, 160.0);
  while (!digitalRead(IR_FRONT_RIGHT))
  {
    Serial.println(angle);
    Serial.print("IR 1: ");
    Serial.println(get_ir_dist(IR_1));
    Serial.print("IR 3: ");
    Serial.println(get_ir_dist(IR_3));
    Serial.print("Angle: ");
    Serial.println(left.angle);
    Serial.print("Average distance: ");
    Serial.println(left.average_distance);
    Serial.println("");
    delay(500);
  }
}

