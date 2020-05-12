#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"

#define DEBOUNCE_DELAY 15


class HallProbe
{
  public:
    HallProbe(String name, int pin);
    void updateState();
    unsigned long getLastPositive();
    
   private: 
    String _name;
    int _pin;

    unsigned long _last_positive_time;

    int _reading;
    int _lastState;
    int _state;
    unsigned long _lastDebounceTime;
};

#endif /* PROBE_H */
