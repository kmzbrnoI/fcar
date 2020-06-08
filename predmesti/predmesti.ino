#include "hall_probe.h"
#include "path.h"

/* All hardware blocks connected to Arduino */

// Hall's probes             // tyto komentare neplati
HallProbe* type_probe_01;    // hardware: FH....
HallProbe* drive_probe_01;   // hardware: FH01  
HallProbe* drive_probe_02;   // hardware: FH02  // 4
HallProbe* drive_probe_03;   // hardware: FH03  // 6
HallProbe* drive_probe_05;   // hardware: FH05  // 11
HallProbe* drive_probe_06;   // hardware: FH06  // 9
HallProbe* drive_probe_07;   // hardware: FH07  // 14
HallProbe* drive_probe_08;   // hardware: FH08  // 16
HallProbe* drive_probe_09;   // hardware: FH09  // 
HallProbe* drive_probe_10;   // hardware: FH10  // 17
HallProbe* drive_probe_11;   // hardware: FH11  // 23
HallProbe* drive_probe_12;   // hardware: FH12  // 18
HallProbe* drive_probe_13;   // hardware: FH13  // 22
HallProbe* drive_probe_21;   // hardware: FH21  // 7  nepouzito, opacny smer
HallProbe* drive_probe_22;   // hardware: FH22  // 5
HallProbe* drive_probe_23;   // hardware: FH23  // 
HallProbe* drive_probe_30;   // hardware: FH23  //

HallProbe* drive_probe_aa; // 8 
HallProbe* drive_probe_bb; // 10 


// FH?? pin 8
// FH13b pin 10
// FH30 pin 15
// pin 19 -- přejezd G ve výstraze
// pin 20 -- přejezd G ve obsazen
// pin 21 -- přejezd H ve výstraze

VPath* dp02_s02;
VPath* s02_dp03;
VPath* s02_dp12;
VPath* dp12_dp13;
VPath* dp13_s13;
VPath* s13_dp09;
VPath* s08_dp09;
VPath* dp09_s09;
VPath* s09_dp10;
VPath* s09_dp11;
VPath* dp10_s10;
VPath* dp11_s11;
VPath* s11_dp22;

// odjezdova krizovatka
VPath* dp22_dp23;
VPath* s10_dp02;

// velky okruh -- tohle nema podminky
VPath* dp03_dp06;
VPath* dp03_dp05;
VPath* dp06_s06;
VPath* s06_dp07;
VPath* dp05_dp07;
VPath* dp07_s07;
VPath* s07_dp08;
VPath* dp08_s08;


const int TECHNOLOGY_TIME = 600;

void setup() {
  /* Start up of hardware blocks*/
  type_probe_01 = new HallProbe(1, "tp_01");
  drive_probe_01 = new HallProbe(2, "FH01");  
  drive_probe_02 = new HallProbe(4, "FH02"); // OK
  drive_probe_03 = new HallProbe(6, "FH03");  // OK
  drive_probe_05 = new HallProbe(11, "FH05"); // OK
  drive_probe_06 = new HallProbe(9, "FH06"); // OK
  drive_probe_07 = new HallProbe(14, "FH07");  // OK
  drive_probe_09 = new HallProbe(18, "FH09");  // OK 
  drive_probe_10 = new HallProbe(17, "FH10"); // OK
  drive_probe_11 = new HallProbe(23, "FH11"); // OK
  drive_probe_13 = new HallProbe(22, "FH13");  // OK
  drive_probe_21 = new HallProbe(7, "FH21"); // nefunguje FH21
  drive_probe_22 = new HallProbe(5, "FH22"); // nefunguje FH22
  drive_probe_23 = new HallProbe(10, "FH23"); // OK
  drive_probe_30 = new HallProbe(16, "FH30");  // OK
  
  drive_probe_aa = new HallProbe(8, "FHaa"); // nepouzito
  drive_probe_bb = new HallProbe(15, "FHbb"); // 
  
  dp02_s02 = new VPath("dp02_s02");
  s02_dp03 = new VPath("s02_dp03");
  s02_dp12 = new VPath("s02_dp12");
  dp12_dp13 = new VPath("dp12_dp13");
  dp13_s13 = new VPath("dp13_s13");
  s13_dp09 = new VPath("s13_dp09");
  s08_dp09 = new VPath("s08_dp09");
  dp09_s09 = new VPath("dp09_s09");
  s09_dp10 = new VPath("s09_dp10");
  s09_dp11 = new VPath("s09_dp11");
  dp10_s10 = new VPath("dp10_s10");
  dp11_s11 = new VPath("dp11_s11");
  s11_dp22 = new VPath("s11_dp22");
  dp22_dp23 = new VPath("dp22_dp23");
  s10_dp02 = new VPath("s10_dp02");
  dp03_dp06 = new VPath("dp03_dp06");
  dp03_dp05 = new VPath("dp03_dp05");
  dp06_s06 = new VPath("dp06_s06");
  s06_dp07 = new VPath("s06_dp07");
  dp05_dp07 = new VPath("dp05_dp07");
  dp07_s07 = new VPath("dp07_s07");
  s07_dp08 = new VPath("s07_dp08");
  dp08_s08 = new VPath("dp08_s08");
  s08_dp09 = new VPath("s08_dp09");

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  // Read signals from probes
  type_probe_01->updateState();
  drive_probe_01->updateState();
  drive_probe_02->updateState();
  drive_probe_03->updateState();
  drive_probe_05->updateState();
  drive_probe_06->updateState();
  drive_probe_07->updateState();
  drive_probe_08->updateState();
  drive_probe_09->updateState();
  drive_probe_10->updateState();
  drive_probe_11->updateState();
  drive_probe_12->updateState(); 
  drive_probe_13->updateState();
  drive_probe_22->updateState();
  drive_probe_23->updateState();

  drive_probe_aa->updateState();
  drive_probe_bb->updateState();
  drive_probe_30->updateState();

  unsigned long drive_delta_02 = millis() - drive_probe_02->getLastPositive();
  if (drive_delta_02 < TECHNOLOGY_TIME) {
    dp02_s02->occupy();
    // logika s02
    s02_dp03->occupy();
    s02_dp12->occupy();
  }

  unsigned long drive_delta_12 = millis() - drive_probe_12->getLastPositive();
  if (drive_delta_12 < TECHNOLOGY_TIME) {
    dp02_s02->release();
    s02_dp03->release();
    s02_dp12->release();
    dp12_dp13->occupy();
    // logika s13
    dp13_s13->occupy();
    s13_dp09->occupy();
    s08_dp09->occupy();
  }

  unsigned long drive_delta_09 = millis() - drive_probe_09->getLastPositive();
  if (drive_delta_09 < TECHNOLOGY_TIME) {
    dp12_dp13->release();
    dp13_s13->release();
    s13_dp09->release();
    s08_dp09->release();
    // logika s13
    dp09_s09->occupy();
    s09_dp10->occupy();
    s09_dp11->occupy();
  }

  unsigned long drive_delta_10 = millis() - drive_probe_10->getLastPositive();
  if (drive_delta_10 < TECHNOLOGY_TIME) {
    dp09_s09->release();
    s09_dp10->release();
    s09_dp11->release();
    // logika s10
    dp10_s10->occupy();


    s10_dp02->occupy();
    dp10_s10->occupy();
    dp10_s10->occupy();    
  }

  unsigned long drive_delta_11 = millis() - drive_probe_11->getLastPositive();
  if (drive_delta_11 < TECHNOLOGY_TIME) {
    dp09_s09->release();
    s09_dp10->release();
    s09_dp11->release();
    // logika s11
    dp11_s11->occupy();    
    s11_dp22->occupy();
    dp22_dp23->occupy();
    s10_dp02->occupy();
  }

  unsigned long drive_delta_22 = millis() - drive_probe_22->getLastPositive();
  if (drive_delta_22 < TECHNOLOGY_TIME) {
    dp11_s11->release();    
    s11_dp22->release();
  }

  unsigned long drive_delta_23 = millis() - drive_probe_23->getLastPositive();
  if (drive_delta_23 < TECHNOLOGY_TIME) {
    dp22_dp23->release();
    s10_dp02->release();
  }
}
