#ifndef SERVO_HANDLER_H
#define SERVO_HANDLER_H

#include <Arduino.h>
#include <Servo.h>

/* to reduce the power consumption, we move the servo slowly */
/* const int SERVO_MOVE_DELAY = 12; */

/* Direct communication with the servo */
class ServoHandler {
public:
    /*
     * ServerHandler Constructor
     * int pin (IN) arduino pin number
     * int basic_angle (IN) default servo angle after start up
     */
    ServoHandler(int pin, int basic_angle);

    /*
     * Move the servo to given angle
     * int angle (IN) the given angle
     */
    void move_to(int angle);

private:
    Servo _servo; // servo object from Servo library
    const int _pin;
    int _current_angle; // current angle
};
#endif /* SERVO_HANDLER_H */
