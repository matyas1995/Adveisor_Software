#include "DueTimer.h"
#include "subroutinen.h"
#include "hardware.h"
#include <math.h>

void setup() {
  hardware_setup();
}

void loop() {
  drive_straight(0, LEFT_SIDE, 150);
}

