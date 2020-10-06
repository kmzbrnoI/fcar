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

///////////////////////////////////////////////////////////////////////////////
// Hall probes

const int PROBE_COUNT = 21;
HallProbe* probes[PROBE_COUNT];

struct IODef {
  String name;
  int pin;
};

IODef probe_defs[PROBE_COUNT] = {
  {"FH02", 4},
  {"FH03", 6},
  {"FH05", 11},
  {"FH06", 9},
  {"FH07", 14},
  {"FH08", 15},
  {"FH09", 18},
  {"FH10", 17},
  {"FH11", 23},
  {"FH13", 22},
  {"FH20", 2},
  {"FH21", 7},
  {"FH22", 5},
  {"FH23", 10},
  {"FH30", 16},
  {"FHA0", A0},
  {"FHA1", A1},
  {"FHA2", A2},
  {"FHaa", 19},
  {"FHab", 20},
  {"FHac", 21},
};

///////////////////////////////////////////////////////////////////////////////
// Crossings

struct CrossingDef {
  String name;
  int pinClosed;
  int pinOccupied;
};

const int CROSSING_COUNT = 3;
Crossing* crossings[CROSSING_COUNT];
CrossingDef crossing_defs[CROSSING_COUNT] = {
  {"CRG", 19, 20},
  {"CRH", 21, 22}, // TODO: is 22 correct?
};

///////////////////////////////////////////////////////////////////////////////
// Paths

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

///////////////////////////////////////////////////////////////////////////////
// Junctions

Junction* j_a;
Junction* j_b;
Junction* j_c;

///////////////////////////////////////////////////////////////////////////////
// Magnets

const int MAGNET_COUNT = 12;
CoilSemaphore* magnets[MAGNET_COUNT];

IODef magnet_defs[MAGNET_COUNT] = {
  {"FM02", 29},
  {"FM06", 36},
  {"FM07", 37},
  {"FM08", 31},
  {"FM09", 33},
  {"FM10", 26},
  {"FM11", 30},
  {"FM12", 28},
  {"FM13", 38},
  {"FM14", 34},
  {"FM20", 32},
  {"FM29", 35},
};

///////////////////////////////////////////////////////////////////////////////

void setup() {
  for (int i=0; i < VEHICLE_COUNT; i++)
    vehicles[i] = new Vehicle(i);

  for (int i=0; i < PROBE_COUNT; i++)
    probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name);

  for (int i=0; i < CROSSING_COUNT; i++)
    crossings[i] = new Crossing(i, crossing_defs[i].pinClosed, crossing_defs[i].pinOccupied, crossing_defs[i].name);

  for (int i=0; i < MAGNET_COUNT; i++)
    magnets[i] = new CoilSemaphore(i, magnet_defs[i].pin, magnet_defs[i].name);

  for (int i=0; i < PATH_COUNT; i++)
    paths[i] = new VPath(i, path_names[i]);

  j_a = new Junction(40, 41);
  j_b = new Junction(42, 43);
  j_c = new Junction(27, 39);

  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  for (int i=0; i < PROBE_COUNT; i++)
    probes[i]->updateState();

  for (int i=0; i < CROSSING_COUNT; i++)
    crossings[i]->updateState();

  if (magnets[FM02]->getSignal() == SSignal::red)
    magnets[FM02]->make_decision(paths[FH02FM02]);
  if (magnets[FM06]->getSignal() == SSignal::red)
    magnets[FM06]->make_decision(paths[FH06FM06]);
  if (magnets[FM07]->getSignal() == SSignal::red)
    magnets[FM07]->make_decision(paths[FH07FM07]);
  if (magnets[FM08]->getSignal() == SSignal::red)
    magnets[FM08]->make_decision(paths[FH08FM08]);
  if (magnets[FM13]->getSignal() == SSignal::red)
    magnets[FM13]->make_decision(paths[FH13FM13]);
  if (magnets[FM09]->getSignal() == SSignal::red)
    magnets[FM09]->make_decision(paths[FH09FM09]);
  if (magnets[FM10]->getSignal() == SSignal::red)
    magnets[FM10]->make_decision(paths[FH10FM10]);

  for (int i=0; i < PATH_COUNT; i++)
    if (! paths[i]->is_clear())
      paths[i]->timeout();
}
