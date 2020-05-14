#include "hall_probe.h"
#include "junction.h"
#include "semaphore.h"

/* All hardware blocks connected to Arduino */
// direction A
HallProbe* drive_probe_a;
HallProbe* type_probe_a;
Junction* junction_a;
Semaphore* semaphore_a;
// direction B
HallProbe* drive_probe_b;
HallProbe* type_probe_b;
Junction* junction_b;
Semaphore* semaphore_b;

/* Time constants describing the situation */
const int SWITCH_PASSAGE_TIME = 3000;  // projeti vyhybkou
const int TOTAL_PASSAGE_TIME = 9000;  // projeti celym usekem zastavky


void setup() {

  /* Start uo of hardware blocks*/
  drive_probe_a = new HallProbe(2);
  type_probe_a = new HallProbe(5);
  junction_a = new Junction(9, 30, 120);
  semaphore_a = new Semaphore(10, 0, 90);

  drive_probe_b = new HallProbe(3);
  type_probe_b = new HallProbe(6);
  junction_b = new Junction(7, 30, 120);
  semaphore_b = new Semaphore(8, 0, 90);

  /* set up defaults */
  junction_a->to_plus();
  semaphore_a->signal_green();

  junction_b->to_plus();
  semaphore_b->signal_green();

  /* use this if you want degug */
  /*
    Serial.begin(9600);
    while (!Serial) {
    ;
    }
  */

}

void manage_bus_stop(HallProbe* drive_probe, HallProbe* type_probe, Junction* junction, Semaphore* semaphore)
{
  // Read signals from probes
  drive_probe->updateState();
  type_probe->updateState();

  // switch junction if the vehicle is a bus (and return it back)
  unsigned long switch_delta = millis() - type_probe->getLastPositive();
  if (switch_delta < SWITCH_PASSAGE_TIME &&
      junction->getDirection() == JDirection::plus &&
      semaphore->getSignal() == SSignal::green) {
    junction->to_minus();
    semaphore->signal_red();
  } else if (switch_delta > SWITCH_PASSAGE_TIME && (junction->getDirection() == JDirection::minus)) {
    junction->to_plus();
  }

  // departure from the bus stop if the way is clear
  unsigned long drive_delta = millis() - drive_probe->getLastPositive();
  if (drive_delta > TOTAL_PASSAGE_TIME && semaphore->getSignal() == SSignal::red) {
    semaphore->signal_green();
  }
}

void loop() {

  manage_bus_stop(drive_probe_a, type_probe_a, junction_a, semaphore_a);
  manage_bus_stop(drive_probe_b, type_probe_b, junction_b, semaphore_b);

}
