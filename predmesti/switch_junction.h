#ifndef SWITCH_JUNCTION_H
#define SWITCH_JUNCTION_H

////////////////////////////////////////////////////////////////////////////////
// Vyhybka ovladana prestavnikem
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

/* Constants for direction */
enum class SJDirection { released, plus, minus };

/* Junction logic */
class Junction
{
  public:
    /* Constructor */
    Junction(int pin_plus, int pin_minus);

    /* Returns JunctionDirection according to current state */
    SJDirection getDirection();

    /* Turn junction to ZERO (uvoleni zaveru) */
    void release();

    /* Turn junction to PLUS */
    void to_plus();

    /* Turn junction to MINUS */
    void to_minus();

  private:
    const int _plus_pin;          // servo angle for plus direction
    const int _minus_pin;         // servo angle for minus direction
    SJDirection _state;           // state of junction (plus or minus)

};
#endif /* SWITCH_JUNCTION_H */
