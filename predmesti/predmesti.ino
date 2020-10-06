#include "blocks.h"
#include "hall_probe.h"
#include "coil_semaphore.h"
#include "switch_junction.h"
#include "path.h"
#include "vehicle.h"
#include "crossing.h"

///////////////////////////////////////////////////////////////////////////////
// Vehicles

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
  {"FHA0", A0},  // blize predmesti
  {"FHA1", A1},  // detekce autobusu
  {"FHA2", A2},  // dale od predmesti
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

struct VPathDef {
  String name;
  int crossingId;
};

const int PATH_COUNT = 29;
VPath* paths[PATH_COUNT];

VPathDef path_defs[PATH_COUNT] = {
  {"FH02FM02", CRG},
  {"FM02FH03", CRUNDEF},
  {"FM02FH13", CRUNDEF},
  {"FH03FH06", CRUNDEF},
  {"FH03FH05", CRUNDEF},
  {"FH05FH07", CRUNDEF},
  {"FH06FM06", CRUNDEF},
  {"FM06FH07", CRUNDEF},
  {"FH06FH07", CRUNDEF},
  {"FH07FM07", CRUNDEF},
  {"FM07FH08", CRUNDEF},
  {"FM07FH30", CRUNDEF},
  {"FH08FM08", CRH},
  {"FM08FH09", CRUNDEF},
  {"FH09FM09", CRUNDEF},
  {"FM09FH10", CRUNDEF},
  {"FM09FH11", CRUNDEF},
  {"FH10FM10", CRUNDEF},
  {"FM10FH02", CRUNDEF},
  {"FH11FM11", CRUNDEF},
  {"FH22FH23", CRUNDEF},
  {"FH13FM13", CRUNDEF},
  {"FM13FH09", CRUNDEF},
  {"FH20FH21", CRUNDEF},
  {"FH21FH22", CRUNDEF},
  {"FHA2FHA0", CRUNDEF},
  {"FHA0FH02", CRUNDEF},
  {"FM11FH22", CRUNDEF},
  {"FH22FH23", CRUNDEF},
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
    paths[i] = new VPath(i, path_defs[i].name, path_defs[i].crossingId);

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

  for (size_t i = 0; i < MAGNET_COUNT; i++)
    if (magnets[i]->getSignal() == SSignal::red)
      magnets[i]->make_decision();

  for (int i=0; i < PATH_COUNT; i++)
    if (!paths[i]->is_clear())
      paths[i]->timeout();
}
