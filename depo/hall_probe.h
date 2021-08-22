#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"
#include <Bounce2.h>

/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY_MS = 5;
const int MIN_EVENT_DELAY = 3000;

/* Direct communication with Hall's probe */
struct HallProbe {
    const int id;
    String name;
    Bounce _pin;
    int changeDelayMs;
    unsigned long _lastEventMs = 0;
    unsigned long _changeTime = 0;
    void (*onOccupied)(HallProbe *) = nullptr;

    HallProbe(int id, int pin, const String &name, int changeDelayMs = 0);

    void update();
};

#endif /* PROBE_H */
