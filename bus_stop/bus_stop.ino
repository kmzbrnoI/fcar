#include "hall_probe.h"
#include "junction.h"
#include "semaphore.h"

/* All hardware blocks connected to Arduino */
HallProbe* drive_probe;
HallProbe* type_probe;
Junction* junction;
Semaphore* semaphore;

/* Time constants describing the situation */
#define SWITCH_PASSAGE_TIME 4000  // projeti vyhybkou
#define TOTAL_PASSAGE_TIME  8000  // projeti celym usekem zastavky


void setup() {

  /* Start uo of hardware blocks*/
  drive_probe = new HallProbe(2);
  type_probe = new HallProbe(5);
  junction = new Junction(9, 30, 120);
  semaphore = new Semaphore(10, 0, 90);

  /* set up defaults */
  junction->to_plus();
  semaphore->signal_green();

  /* use this if you want degug */
  /*
    Serial.begin(9600);
    while (!Serial) {
    ;
    }
  */

}

void loop() {

  // Read signals from probes
  drive_probe->updateState();
  type_probe->updateState();

  // switch junction if the vehicle is a bus (and return it back)
  unsigned long switch_delta = millis() - type_probe->getLastPositive();
  if (switch_delta < SWITCH_PASSAGE_TIME &&
      junction->getDirection() == JUNCTION_PLUS_DIRECTION &&
      semaphore->getSignal() == SEMAPHORE_GREEN) {
    junction->to_minus();
    semaphore->signal_red();
  } else if (switch_delta > SWITCH_PASSAGE_TIME && (junction->getDirection() == JUNCTION_MINUS_DIRECTION)) {
    junction->to_plus();
  }

  // departure from the bus stop if the way is clear
  unsigned long drive_delta = millis() - drive_probe->getLastPositive();
  if (drive_delta > TOTAL_PASSAGE_TIME && semaphore->getSignal() == SEMAPHORE_RED) {
    semaphore->signal_green();
  }
}
