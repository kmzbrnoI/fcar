#ifndef CROSSING_H
#define CROSSING_H

#include "Arduino.h"

/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY = 15;

class Crossing
{
  public:
     /* Constructor */
    Crossing(int caution_pin, int occupation_pin);

    /* Update state from probe */
    void updateState();

    /* Returns the last timestamp (milliseconds) when the state was positive */
    unsigned long getLastPositive();

   private:
    unsigned long _last_positive_time;  // timestamp of last positive state
    const int _pin;                     // arduino pin wehere probe is connected
    const char* _name;
    /*
     * The logic of Hall's sond is not straightforward, it cannot remember about
     * it's states in past. Those variables help us to implement it.
     * See updateState() function in hall_probe.cpp for implementation details
     */
    int _reading;                       // helping variable
    int _lastState;                     // helping variable
    int _state;                         // helping variable
    unsigned long _lastDebounceTime;    // helping variable
};

#endif /* CROSSING_H */
