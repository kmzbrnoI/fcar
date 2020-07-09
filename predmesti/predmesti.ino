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


const int PATH_COUNT = 18;
VPath* paths[PATH_COUNT];


// pin 19 -- přejezd G ve výstraze
// pin 20 -- přejezd G ve obsazen
// pin 21 -- přejezd H ve výstraze

Junction* j_a;
Junction* j_b;
Junction* j_c;

CoilSemaphore* s02;
CoilSemaphore* s06;
CoilSemaphore* s07;
CoilSemaphore* s08;
CoilSemaphore* s09;
CoilSemaphore* s10;
CoilSemaphore* s11;
CoilSemaphore* s12;
CoilSemaphore* s13;
CoilSemaphore* s14;
CoilSemaphore* s20;
CoilSemaphore* s29;


void setup() {

  for (int i=0; i < PROBE_COUNT; i++) {
    // nefunguje FH22
    // FHaa nepouzito

    probes[i] = new HallProbe(i, probe_pins[i], probe_names[i], occupies[i], releases[i], reserves[i], cancels_reservation[i]);
  }

  paths[0] = new VPath("FH02_FH03");
  paths[1] = new VPath("FH02_FH12");
  paths[2] = new VPath("FH03_FH06");
  paths[3] = new VPath("FH03_FH05");
  paths[4] = new VPath("FH05_FH07");
  paths[5] = new VPath("FH06_FH07");
  paths[6] = new VPath("FH07_FH08");
  paths[7] = new VPath("FH07_FH30");
  paths[8] = new VPath("FH08_FH09");
  paths[9] = new VPath("FH09_FH10");
  paths[10] = new VPath("FH09_FH11");
  paths[11] = new VPath("FH10_FH02");
  paths[12] = new VPath("FH11_FH12");
  paths[13] = new VPath("FH12_FH22");
  paths[14] = new VPath("FH22_FH23");
  paths[15] = new VPath("FH12_FH13");
  paths[16] = new VPath("FH13_FH09");
  paths[17] = new VPath("FH20_FH21");
  paths[18] = new VPath("FH21_FH22");

  j_a = new Junction(40, 41);
  j_b = new Junction(42, 43);
  j_c = new Junction(27, 39);

  s02 = new CoilSemaphore(29);
  s06 = new CoilSemaphore(36);
  s07 = new CoilSemaphore(37);
  s08 = new CoilSemaphore(31);
  s09 = new CoilSemaphore(33);
  s10 = new CoilSemaphore(26);
  s11 = new CoilSemaphore(30);
  s12 = new CoilSemaphore(28);
  s13 = new CoilSemaphore(38);
  s14 = new CoilSemaphore(34);
  s20 = new CoilSemaphore(32);
  s29 = new CoilSemaphore(35);
  

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {

  for (int i=0; i < PROBE_COUNT; i++) {
    probes[i]->updateState();
  }


  j_a->to_minus();
  j_b->to_plus();
  j_c->to_plus();

}
