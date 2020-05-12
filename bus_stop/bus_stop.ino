#include "hall_probe.h"
#include "junction.h"

HallProbe* controll_probe;
HallProbe* type_probe;
Junction* junction_a;
Junction* stop_signal;

//TODO: Rename Junction to servo_handler and define which position means what


#define BUS_TRANSIT 5000


void setup() {

  // controll_probe = new HallProbe("Controll Probe", 2);
  type_probe = new HallProbe("Type Probe", 5);
  junction_a = new Junction("Switch A", 9, 30, 120);
  // stop_signal = new Junction("Stop Signal", 10, 0, 90);

  junction_a->to_plus();

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {

  // controll_probe->updateState();
  type_probe->updateState();

  // Switch junction if bus (and return it back)
  unsigned long delta = millis() - type_probe->getLastPositive();
  if (delta < BUS_TRANSIT && (junction_a->getState() == PLUS_STATE)) {
    junction_a->to_minus();

    Serial.print("A last_positive: ");
    Serial.print(type_probe->getLastPositive());
    Serial.print(" millis: ");
    Serial.print(millis());
    Serial.print(" :: ");
    Serial.println(delta);
  }

  if (delta > BUS_TRANSIT && (junction_a->getState() == MINUS_STATE)) {
    junction_a->to_plus();

    Serial.print("B last_positive: ");
    Serial.print(type_probe->getLastPositive());
    Serial.print(" millis: ");
    Serial.print(millis());
    Serial.print(" :: ");
    Serial.println(delta);
  }



}
