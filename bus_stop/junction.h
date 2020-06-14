#ifndef JUNCTION_H
#define JUNCTION_H

////////////////////////////////////////////////////////////////////////////////
// Vyhybka ovladana servem
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "servo_handler.h"

/* Constants for direction */
enum class JDirection { plus, minus };

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

    /* Returns JunctionDirection according to current state */
    JDirection getDirection();

    /* Turn junction to PLUS */
    void to_plus();

    /* Turn junction to MINUS */
    void to_minus();

  private:
    ServoHandler* _servo_handler;   // ServoHandler, see servo_handler.h
    const int _plus_angle;          // servo angle for plus direction
    const int _minus_angle;         // servo angle for minus direction
    JDirection _state;              // state of junction (plus or minus)

};
#endif /* JUNCTION_H */
