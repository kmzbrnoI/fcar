#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"

#define DEBOUNCE_DELAY 15


class HallProbe
{
  public:
    HallProbe(int pin);
    void updateState();
    unsigned long getLastPositive();
    
   private: 
    unsigned long _last_positive_time;
    const int _pin;
    int _reading;
    int _lastState;
    int _state;
    unsigned long _lastDebounceTime;
};

#endif /* PROBE_H */
