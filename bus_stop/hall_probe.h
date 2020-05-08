#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"

#define DEBOUNCE_DELAY 15


class HallProbe
{
  public:
    HallProbe(String name, int pin);
    void updateState();
    
   private: 
    String _name;
    int _pin;

    int _reading;
    int _lastState;
    int _state;
    unsigned long _lastDebounceTime;
};

#endif /* PROBE_H */
