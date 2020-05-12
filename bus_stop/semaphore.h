#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <Arduino.h>
#include "servo_handler.h"

/* Constant for GREEN signal */
#define SEMAPHORE_GREEN 1

/* Constant for RED signal */
#define SEMAPHORE_RED 2

/* Semaphore logic */
class Semaphore
{
  public:
    /*
     * Constructor
     * int pin (IN) arduino pin number
     * int green_angle (IN) servo angle for green signal, DEFAULT after start up
     * int red_angle (IN) servo angle for red signal
     */
    Semaphore(int pin, int green_angle, int red_angle);
    
    /* Returns SEMAPHORE_GREEN or SEMAPHORE_RED according to current state */
    int getSignal();
    
    /* Turn semaphore to GREEN */
    void signal_green();
    
    /* Turn semaphore to RED */
    void signal_red();

  private: 
    ServoHandler* _servo_handler;   // ServoHandler, see servo_handler.h
    const int _green_angle;         // servo angle for green signal
    const int _red_angle;           // servo angle for red signal
    int _state;                     // state of semaphore (SEMAPHORE_GREEN or SEMAPHORE_RED)

};
#endif /* SEMAPHORE_H */
