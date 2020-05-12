#ifndef JUNCTION_H
#define JUNCTION_H

#include <Arduino.h>
#include "servo_handler.h"

/* Constant for PLUS direction */
#define JUNCTION_PLUS_DIRECTION 1

/* Constant for RED direction */
#define JUNCTION_MINUS_DIRECTION 2

/* Junction logic */
class Junction
{
  public:
    /*
     * Constructor
     * int pin (IN) arduino pin number
     * int plus_angle (IN) servo angle for plus direction, DEFAULT after start up
     * int minus_angle (IN) servo angle for minus direction
     */
    Junction(int pin, int plus_angle, int minus_angle);
    
    /* Returns JUNCTION_PLUS_DIRECTION or JUNCTION_MINUS_DIRECTION according to current state */
    int getDirection();
    
    /* Turn junction to PLUS */
    void to_plus();
    
    /* Turn junction to MINUS */
    void to_minus();

  private: 
    ServoHandler* _servo_handler;   // ServoHandler, see servo_handler.h
    const int _plus_angle;          // servo angle for plus direction
    const int _minus_angle;         // servo angle for minus direction
    int _state;                     // state of junction (JUNCTION_PLUS_DIRECTION or JUNCTION_MINUS_DIRECTION)

};
#endif /* JUNCTION_H */
