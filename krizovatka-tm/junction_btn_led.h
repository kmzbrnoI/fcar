#ifndef JUNCTION_BTN_LED_H
#define JUNCTION_BTN_LED_H

////////////////////////////////////////////////////////////////////////////////
// Vyhybka ovladana servem, s tlacitkem a dvema ledkami
////////////////////////////////////////////////////////////////////////////////

#include "servo_handler.h"
#include <Arduino.h>
#include <Bounce2.h>

const int BTN_DEBOUNCE_DELAY_MS = 5;

/* Constants for direction */
enum class JDirection { plus, minus };

/* Junction logic */
class Junction {
public:
    /*
     * Constructor
     * int pin (IN) arduino pin number
     * int plus_angle (IN) servo angle for plus direction, DEFAULT after start
     * up int minus_angle (IN) servo angle for minus direction
     */
    Junction(const String &name, int pin, int plus_angle, int minus_angle, int pinLedPlus, int pinLedMinus, int pinBtn);

    /* Returns JunctionDirection according to current state */
    JDirection getDirection();

    /* Turn junction to PLUS */
    void to_plus();

    /* Turn junction to MINUS */
    void to_minus();

    /* React if button was pressed */
    void updateButton();

private:
    String _name; // name
    ServoHandler *_servo_handler; // ServoHandler, see servo_handler.h
    const int _plus_angle; // servo angle for plus direction
    const int _minus_angle; // servo angle for minus direction
    JDirection _state; // state of junction (plus or minus)

    Bounce *_button = nullptr;
    int _pinLedPlus;
    int _pinLedMinus;
    int _ledPlusState = 0;
    int _ledMinusState = 0;
};
#endif /* JUNCTION_BTN_LED_H */
