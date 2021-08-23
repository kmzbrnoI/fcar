#ifndef SEMAPHORE_H
#define SEMAPHORE_H

////////////////////////////////////////////////////////////////////////////////
// Zastavovaci magnet ovladany servem
////////////////////////////////////////////////////////////////////////////////

#include "servo_handler.h"
#include <Arduino.h>

/* Constant for signals */
enum class SSignal { green, red };

/* Semaphore logic */
class Semaphore {
public:
    /*
     * Constructor
     * int pin (IN) arduino pin number
     * int green_angle (IN) servo angle for green signal, DEFAULT after start up
     * int red_angle (IN) servo angle for red signal
     */
    Semaphore(int pin, int green_angle, int red_angle);

    /* Returns SSignal according to current state */
    SSignal getSignal();

    /* Turn semaphore to green */
    void signal_green();

    /* Turn semaphore to red */
    void signal_red();

private:
    ServoHandler *_servo_handler; // ServoHandler, see servo_handler.h
    const int _green_angle; // servo angle for green signal
    const int _red_angle; // servo angle for red signal
    SSignal _state; // state of semaphore (green or red)
};
#endif /* SEMAPHORE_H */
