#ifndef COIL_SEMAPHORE_H
#define COIL_SEMAPHORE_H

////////////////////////////////////////////////////////////////////////////////
// Zastavovaci magnet ovladany civkou
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

/* Constant for signals */
enum class SSignal { green, red };

/* Semaphore logic */
class CoilSemaphore
{
  public:
    /* Constructor */
    CoilSemaphore(int pin);

    /* Returns SSignal according to current state */
    SSignal getSignal();

    /* Turn semaphore to green */
    void signal_green();

    /* Turn semaphore to red */
    void signal_red();

  private:
    const int _pin;
    SSignal _state;

};
#endif /* COIL_SEMAPHORE_H */
