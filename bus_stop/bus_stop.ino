#include "hall_probe.h"
#include "switch.h"

/*
HallProbe probes[1] = {
  HallProbe("HP_Black", 2)
};
*/

Switch switches[1] = {
  Switch("Switch", 9, 30, 120)
};


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}
void loop() {

/*
  for (int i=0; i <  sizeof(probes) / sizeof(HallProbe); i++) {
    probes[i].updateState();
  }
*/
  switches[0].to_minus();
  switches[0].to_plus();
}
