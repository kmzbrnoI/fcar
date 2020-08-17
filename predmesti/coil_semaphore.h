#ifndef COIL_SEMAPHORE_H
#define COIL_SEMAPHORE_H

////////////////////////////////////////////////////////////////////////////////
// Zastavovaci magnet ovladany civkou
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#include "blocks.h"
#include "path.h"
#include "switch_junction.h"
#include "vehicle.h"


/* Constant for signals */
enum class SSignal { green, red };

/* Semaphore logic */
class CoilSemaphore
{
  public:
    /* Constructor */
    CoilSemaphore(int pin, const char* name);

    /* Returns SSignal according to current state */
    SSignal getSignal();

    void make_decision(int id);
    
    /* Turn semaphore to green */
    void signal_green();

    /* Turn semaphore to red */
    void signal_red();

  private:
    const int _pin;
    const char* _name;
    SSignal _state;

};
#endif /* COIL_SEMAPHORE_H */
