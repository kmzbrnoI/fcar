#include "hall_probe.h"
#include "coil_semaphore.h"
#include "switch_junction.h"
#include "path.h"

/* All hardware blocks connected to Arduino */

// Hall's probes
HallProbe* type_probe_01;
HallProbe* fh01;
HallProbe* fh02;
HallProbe* fh03;
HallProbe* fh05;
HallProbe* fh06;
HallProbe* fh07;
HallProbe* fh08;
HallProbe* fh09;
HallProbe* fh10;
HallProbe* fh11;
HallProbe* fh12;
HallProbe* fh13;
HallProbe* fh21;
HallProbe* fh22;
HallProbe* fh23;
HallProbe* fh30;

HallProbe* fhaa;
HallProbe* fhbb;

// pin 19 -- přejezd G ve výstraze
// pin 20 -- přejezd G ve obsazen
// pin 21 -- přejezd H ve výstraze

VPath* fh02_s02;
VPath* s02_fh03;
VPath* s02_fh12;
VPath* fh12_fh13;
VPath* fh13_s13;
VPath* s13_fh09;
VPath* s08_fh09;
VPath* fh09_s09;
VPath* s09_fh10;
VPath* s09_fh11;
VPath* fh10_s10;
VPath* fh11_s11;
VPath* s11_fh22;

// odjezdova krizovatka
VPath* fh22_fh23;
VPath* s10_fh02;

// velky okruh -- tohle nema podminky
VPath* fh03_fh06;
VPath* fh03_fh05;
VPath* fh06_s06;
VPath* s06_fh07;
VPath* fh05_fh07;
VPath* fh07_s07;
VPath* s07_fh08;
VPath* fh08_s08;


const int TECHNOLOGY_TIME = 600;

void setup() {
  /* Start up of hardware blocks*/
  type_probe_01 = new HallProbe(1, "tp_01");
  fh01 = new HallProbe(2, "FH01");
  fh02 = new HallProbe(4, "FH02");
  fh03 = new HallProbe(6, "FH03");
  fh05 = new HallProbe(11, "FH05");
  fh06 = new HallProbe(9, "FH06");
  fh07 = new HallProbe(14, "FH07");
  fh09 = new HallProbe(18, "FH09");
  fh10 = new HallProbe(17, "FH10");
  fh11 = new HallProbe(23, "FH11");
  fh13 = new HallProbe(22, "FH13");
  fh21 = new HallProbe(7, "FH21"); // nefunguje FH21
  fh22 = new HallProbe(5, "FH22"); // nefunguje FH22
  fh23 = new HallProbe(10, "FH23");
  fh30 = new HallProbe(16, "FH30");

  fhaa = new HallProbe(8, "FHaa"); // nepouzito
  fhbb = new HallProbe(15, "FHbb"); //

  fh02_s02 = new VPath("fh02_s02");
  s02_fh03 = new VPath("s02_fh03");
  s02_fh12 = new VPath("s02_fh12");
  fh12_fh13 = new VPath("fh12_fh13");
  fh13_s13 = new VPath("fh13_s13");
  s13_fh09 = new VPath("s13_fh09");
  s08_fh09 = new VPath("s08_fh09");
  fh09_s09 = new VPath("fh09_s09");
  s09_fh10 = new VPath("s09_fh10");
  s09_fh11 = new VPath("s09_fh11");
  fh10_s10 = new VPath("fh10_s10");
  fh11_s11 = new VPath("fh11_s11");
  s11_fh22 = new VPath("s11_fh22");
  fh22_fh23 = new VPath("fh22_fh23");
  s10_fh02 = new VPath("s10_fh02");
  fh03_fh06 = new VPath("fh03_fh06");
  fh03_fh05 = new VPath("fh03_fh05");
  fh06_s06 = new VPath("fh06_s06");
  s06_fh07 = new VPath("s06_fh07");
  fh05_fh07 = new VPath("fh05_fh07");
  fh07_s07 = new VPath("fh07_s07");
  s07_fh08 = new VPath("s07_fh08");
  fh08_s08 = new VPath("fh08_s08");
  s08_fh09 = new VPath("s08_fh09");

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  // Read signals from probes
  type_probe_01->updateState();
  fh01->updateState();
  fh02->updateState();
  fh03->updateState();
  fh05->updateState();
  fh06->updateState();
  fh07->updateState();
  fh08->updateState();
  fh09->updateState();
  fh10->updateState();
  fh11->updateState();
  fh12->updateState();
  fh13->updateState();
  fh22->updateState();
  fh23->updateState();

  fhaa->updateState();
  fhbb->updateState();
  fh30->updateState();

  unsigned long drive_delta_02 = millis() - fh02->getLastPositive();
  if (drive_delta_02 < TECHNOLOGY_TIME) {
    fh02_s02->occupy();
    // logika s02
    s02_fh03->occupy();
    s02_fh12->occupy();
  }

  unsigned long drive_delta_12 = millis() - fh12->getLastPositive();
  if (drive_delta_12 < TECHNOLOGY_TIME) {
    fh02_s02->release();
    s02_fh03->release();
    s02_fh12->release();
    fh12_fh13->occupy();
    // logika s13
    fh13_s13->occupy();
    s13_fh09->occupy();
    s08_fh09->occupy();
  }

  unsigned long drive_delta_09 = millis() - fh09->getLastPositive();
  if (drive_delta_09 < TECHNOLOGY_TIME) {
    fh12_fh13->release();
    fh13_s13->release();
    s13_fh09->release();
    s08_fh09->release();
    // logika s13
    fh09_s09->occupy();
    s09_fh10->occupy();
    s09_fh11->occupy();
  }

  unsigned long drive_delta_10 = millis() - fh10->getLastPositive();
  if (drive_delta_10 < TECHNOLOGY_TIME) {
    fh09_s09->release();
    s09_fh10->release();
    s09_fh11->release();
    // logika s10
    fh10_s10->occupy();


    s10_fh02->occupy();
    fh10_s10->occupy();
    fh10_s10->occupy();
  }

  unsigned long drive_delta_11 = millis() - fh11->getLastPositive();
  if (drive_delta_11 < TECHNOLOGY_TIME) {
    fh09_s09->release();
    s09_fh10->release();
    s09_fh11->release();
    // logika s11
    fh11_s11->occupy();
    s11_fh22->occupy();
    fh22_fh23->occupy();
    s10_fh02->occupy();
  }

  unsigned long drive_delta_22 = millis() - fh22->getLastPositive();
  if (drive_delta_22 < TECHNOLOGY_TIME) {
    fh11_s11->release();
    s11_fh22->release();
  }

  unsigned long drive_delta_23 = millis() - fh23->getLastPositive();
  if (drive_delta_23 < TECHNOLOGY_TIME) {
    fh22_fh23->release();
    s10_fh02->release();
  }
}
