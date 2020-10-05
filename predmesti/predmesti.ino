#include "blocks.h"
#include "hall_probe.h"
#include "coil_semaphore.h"
#include "switch_junction.h"
#include "path.h"
#include "vehicle.h"
#include "crossing.h"

/* All hardware blocks connected to Arduino */


/*
novy hallovky na predmesti (vjezd od depa)
A0  blize k mestu
A1  autobus nebo ne
A2  zacina usek

*/

Vehicle* vehicles[VEHICLE_COUNT];

const int PROBE_COUNT = 21;
HallProbe* probes[PROBE_COUNT];

int probe_pins[PROBE_COUNT] = {
  4,
  6,
  11,
  9,
  14,
  15,
  18,
  17,
  23,
  22,
  2,
  7,
  5,
  10,
  16,
  A0,
  A1,
  A2,
  19,
  20,
  21,
//  8 FHaa, nepouzito
};

const char* probe_names[PROBE_COUNT] = {
  "FH02",
  "FH03",
  "FH05",
  "FH06",
  "FH07",
  "FH08",
  "FH09",
  "FH10",
  "FH11",
  "FH13",
  "FH20",
  "FH21",
  "FH22",
  "FH23",
  "FH30",
  "FHA0",
  "FHA1",
  "FHA2",
  // "FHaa" -- nepouzito
};

const int CROSSING_COUNT = 3;
Crossing* crossings[CROSSING_COUNT];
int crossing_pins[CROSSING_COUNT] = {
  19,
  20,
  21,
};
const char* crossing_names[CROSSING_COUNT] = {
  "CRG1",
  "CRG2",
  "CRH1",
};
// CRG1 pin 19 -- přejezd G ve výstraze
// CRG2 pin 20 -- přejezd G ve obsazen
// CRH1 pin 21 -- přejezd H ve výstraze

const int PATH_COUNT = 29;
VPath* paths[PATH_COUNT];

const char* path_names[PATH_COUNT] = {
  "FH02FM02",
  "FM02FH03",
  "FM02FH13",
  "FH03FH06",
  "FH03FH05",
  "FH05FH07",
  "FH06FM06",
  "FM06FH07",
  "FH06FH07",
  "FH07FM07",
  "FM07FH08",
  "FM07FH30",
  "FH08FM08",
  "FM08FH09",
  "FH09FM09",
  "FM09FH10",
  "FM09FH11",
  "FH10FM10",
  "FM10FH02",
  "FH11FM11",
  "FH22FH23",
  "FH13FM13",
  "FM13FH09",
  "FH20FH21",
  "FH21FH22",
  "FHA2FHA0",
  "FHA0FH02",
  "FM11FH22",
  "FH22FH23",
};

Junction* j_a;
Junction* j_b;
Junction* j_c;


const int MAGNET_COUNT = 12;
CoilSemaphore* magnets[MAGNET_COUNT];

int magnet_pins[MAGNET_COUNT] = {
  29,
  36,
  37,
  31,
  33,
  26,
  30,
  28,
  38,
  34,
  32,
  35,
};

const char* magnet_names[MAGNET_COUNT] = {
  "FM02",
  "FM06",
  "FM07",
  "FM08",
  "FM09",
  "FM10",
  "FM11",
  "FM12",
  "FM13",
  "FM14",
  "FM20",
  "FM29",
};

void setup() {

  for (int i=0; i < VEHICLE_COUNT; i++) {
    vehicles[i] = new Vehicle(i);
  }
  
  for (int i=0; i < PROBE_COUNT; i++) {
    // nefunguje FH22
    // FHaa nepouzito

    probes[i] = new HallProbe(i, probe_pins[i], probe_names[i]);
  }

  for (int i=0; i < CROSSING_COUNT; i++) {
    crossings[i] = new Crossing(i, crossing_pins[i], crossing_names[i]);
  }

  for (int i=0; i < MAGNET_COUNT; i++) {
    magnets[i] = new CoilSemaphore(magnet_pins[i], magnet_names[i]);
  }

  for (int i=0; i < PATH_COUNT; i++) {
    paths[i] = new VPath(i, path_names[i]);
  }

  j_a = new Junction(40, 41);
  j_b = new Junction(42, 43);
  j_c = new Junction(27, 39);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {

  for (int i=0; i < PROBE_COUNT; i++) {
    probes[i]->updateState();
  }

  for (int i=0; i < CROSSING_COUNT; i++) {
    crossings[i]->updateState();
  }

  if (magnets[FM02]->getSignal() == SSignal::red) {
    magnets[FM02]->make_decision(FM02, paths[FH02FM02]);
  }
  if (magnets[FM06]->getSignal() == SSignal::red) {
    magnets[FM06]->make_decision(FM06, paths[FH06FM06]);
  }
  if (magnets[FM07]->getSignal() == SSignal::red) {
    magnets[FM07]->make_decision(FM07, paths[FH07FM07]);
  }
  if (magnets[FM08]->getSignal() == SSignal::red) {
    magnets[FM08]->make_decision(FM08, paths[FH08FM08]);
  }
  if (magnets[FM13]->getSignal() == SSignal::red) {
    magnets[FM13]->make_decision(FM13, paths[FH13FM13]);
  }
  if (magnets[FM09]->getSignal() == SSignal::red) {
    magnets[FM09]->make_decision(FM09, paths[FH09FM09]);
  }
  if (magnets[FM10]->getSignal() == SSignal::red) {
    magnets[FM10]->make_decision(FM10, paths[FH10FM10]);
  }

  for (int i=0; i < PATH_COUNT; i++) {
    if (! paths[i]->is_clear()) {
      paths[i]->timeout();
    }
  }
}
