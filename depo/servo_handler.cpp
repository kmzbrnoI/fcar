#include "servo_handler.h"

ServoHandler::ServoHandler(int pin, int basic_angle)
    : _pin(pin)
{
    _servo.attach(_pin);
    _servo.write(basic_angle);
    _current_angle = basic_angle;
}

void ServoHandler::move_to(int target)
{
    _current_angle = target;
    _servo.write(_current_angle);
}
