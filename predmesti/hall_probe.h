#ifndef PROBE_H
#define PROBE_H

#include <Bounce2.h>
#include "Arduino.h"

/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY = 15;

/* Direct communication with Hall's probe */
struct HallProbe {
  const int id;
  String name;
  Bounce _pin;

  HallProbe(int id, int pin, const String& name);

  void update();
  void changed();
};

#endif /* PROBE_H */
