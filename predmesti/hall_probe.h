#ifndef PROBE_H
#define PROBE_H

#include <Bounce2.h>
#include "Arduino.h"

/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY_MS = 5;

/* Direct communication with Hall's probe */
struct HallProbe {
  const int id;
  String name;
  Bounce _pin;
  int changeDelayMs;
  unsigned long _changeTime = 0;

  HallProbe(int id, int pin, const String& name, int changeDelayMs = 0);

  void update();
  void changed();
};

#endif /* PROBE_H */
