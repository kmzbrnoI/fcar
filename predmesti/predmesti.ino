#include "blocks.h"
#include "hall_probe.h"
#include "coil_semaphore.h"
#include "switch_junction.h"
#include "path.h"

/* All hardware blocks connected to Arduino */

const int PROBE_COUNT = 15;
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
  // "FHaa" -- nepouzito
};

/*
int occupies[PROBE_COUNT][2] = {
  {FH02FH03, FH02FH12},
  {FH03FH05, FH03FH06},
  {FH05FH07, -1},
  {FH06FH07, -1},
  {FH07FH08, -1},
  {FH08FH09, -1},
  {FH09FH10, FH09FH11},
  {FH10FH02, -1},
  {FH11FH22, -1},
  {FH13FH09, -1},
  {FH20FH21, -1},
  {FH21FH22, -1},  
  {FH22FH23, -1},
  {-1, -1},
  {-1, -1} 
};

int releases[PROBE_COUNT][2] = {
  {FH10FH02, -1},
  {FH02FH03, -1},
  {FH03FH05, -1},
  {FH03FH06, -1},
  {FH05FH07, FH06FH07},
  {FH07FH08, -1},
  {FH08FH09, -1},
  {FH09FH10, -1},
  {FH09FH11, -1},
  {FH12FH13, -1},
  {-1, -1},
  {FH20FH21, -1},
  {FH11FH22, FH21FH22},
  {FH22FH23, -1},
  {-1, -1}
};

int reserves[PROBE_COUNT][2] = {
  {-1, FH21FH22},
  {-1, -1},
  {FH06FH07, -1},
  {-1, -1},
  {-1, -1},  
  {FH13FH09, -1},
  {-1, -1},
  {FH22FH23, -1},
  {FH02FH12, -1},
  {FH08FH09, -1},
  {-1, -1},
  {FH11FH22, FH02FH12},
  {FH10FH02, -1},
  {-1, -1},
  {-1, -1}
};

int cancels_reservation[PROBE_COUNT][2] = {
  {FH22FH23, -1},
  {-1, -1},
  {-1, -1}, 
  {FH06FH07, -1},
  {-1, -1},
  {FH08FH09, FH13FH09},
  {-1, -1},  
  {-1, -1},
  {FH02FH12, FH11FH22},
  {-1, -1},
  {-1, -1},
  {FH02FH12, -1},
  {FH10FH02, -1},
  {-1, -1}
};
*/

const int PATH_COUNT = 19;
VPath* paths[PATH_COUNT];

const char* path_names[PATH_COUNT] = {
  "FH02FM02",
  "FM02FH03",
  "FM02FH13",
  "FH03FH06",
  "FH03FH05",
  "FH05FH07",
  "FH06FH07",
  "FH07FH08",
  "FH07FH30",
  "FH08FH09",
  "FH09FH10",
  "FH09FH11",
  "FH10FH02",
  "FH11FH22",
  "FH22FH23",
  "FH12FH13",
  "FH13FH09",
  "FH20FH21",
  "FH21FH22",
};


// pin 19 -- přejezd G ve výstraze
// pin 20 -- přejezd G ve obsazen
// pin 21 -- přejezd H ve výstraze

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

  for (int i=0; i < PROBE_COUNT; i++) {
    // nefunguje FH22
    // FHaa nepouzito

    probes[i] = new HallProbe(i, probe_pins[i], probe_names[i]);
  }

  for (int i=0; i < MAGNET_COUNT; i++) {
    magnets[i] = new CoilSemaphore(magnet_pins[i], magnet_names[i]);
  }

  for (int i=0; i < PATH_COUNT; i++) {
    paths[i] = new VPath(path_names[i]);
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
  
  j_b->to_plus();
  j_c->to_plus();

  for (int i=0; i < MAGNET_COUNT; i++) {
    if (magnets[i]->getSignal() == SSignal::red) {
      magnets[i]->make_decision(i);
    }
  }
}
