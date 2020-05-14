#include "hall_probe.h"
#include "junction.h"
#include "semaphore.h"

/* All hardware blocks connected to Arduino */

// direction A (Předměstí --> Hrad)
HallProbe* drive_probe_a;   // hardware: HSS1
HallProbe* type_probe_a;    // hardware: HSR1
Junction* junction_a;       // hardware: SV1
Semaphore* semaphore_a;     // hardware: ST1

// direction B (Hrad --> Předměstí)
HallProbe* drive_probe_b;   // hardware: HSS2
HallProbe* type_probe_b;    // hardware: HSR2
Junction* junction_b;       // hardware: SV2
Semaphore* semaphore_b;     // hardware: ST2


/* Time constants describing the situation */
const int SWITCH_PASSAGE_TIME = 3000;  // projeti vyhybkou
const int TOTAL_PASSAGE_TIME = 9000;  // projeti celym usekem zastavky


void setup() {

  /* Start uo of hardware blocks*/
  drive_probe_a = new HallProbe(5);
  type_probe_a = new HallProbe(4);
  junction_a = new Junction(9, 0, 30);
  semaphore_a = new Semaphore(11, 0, 90);

  drive_probe_b = new HallProbe(3);
  type_probe_b = new HallProbe(2);
  junction_b = new Junction(6, 0, 30);
  semaphore_b = new Semaphore(10, 0, 90);
}

void manage_bus_stop(HallProbe* drive_probe, HallProbe* type_probe, Junction* junction, Semaphore* semaphore)
{
  // Read signals from probes
  drive_probe->updateState();
  type_probe->updateState();

  // departure from the bus stop if the way is clear
  unsigned long drive_delta = millis() - drive_probe->getLastPositive();
  if (drive_delta > TOTAL_PASSAGE_TIME && semaphore->getSignal() == SSignal::red) {
    semaphore->signal_green();
  }

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
}

void loop() {
  manage_bus_stop(drive_probe_a, type_probe_a, junction_a, semaphore_a);
  manage_bus_stop(drive_probe_b, type_probe_b, junction_b, semaphore_b);
}
