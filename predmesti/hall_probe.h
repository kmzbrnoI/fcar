#ifndef PROBE_H
#define PROBE_H

#include "Arduino.h"

#include "blocks.h"
#include "path.h"
#include "coil_semaphore.h"
#include "switch_junction.h"
#include "vehicle.h"


/* Constant for DEBOUNCE delay */
const int DEBOUNCE_DELAY = 15;

/* Direct communication with Hall's probe */
class HallProbe
{
  public:
     /*
     * Constructor
     * int pin (IN) arduino pin number
     */
    HallProbe(int id, int pin, const String& name);

    /* Update state from probe */
    void updateState();

   private:
    unsigned long _last_positive_time;  // timestamp of last positive state
    const int _id;
    const int _pin;                     // arduino pin wehere probe is connected
    String _name;

    /*
     * The logic of Hall's sond is not straightforward, it cannot remember about
     * it's states in past. Those variables help us to implement it.
     * See updateState() function in hall_probe.cpp for implementation details
     */
    int _reading;                       // helping variable
    int _lastState;                     // helping variable
    int _state;                         // helping variable
    unsigned long _lastDebounceTime;    // helping variable

    void changed();
};

#endif /* PROBE_H */
