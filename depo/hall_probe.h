#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"
#include <Bounce2.h>

/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY_MS = 5;

/* Direct communication with Hall's probe */
struct HallProbe {
    const int id;
    String name;
    Bounce _pin;
    int changeDelayMs;
    unsigned long _changeTime = 0;

    HallProbe(int id, int pin, const String &name, int changeDelayMs = 0);

    void update();
    void _changed();
};

#endif /* PROBE_H */
