#include "servo_handler.h"

ServoHandler::ServoHandler(int pin, int basic_angle)
    : _pin(pin)
{
    _servo.attach(_pin);
    _servo.write(basic_angle);
    _current_angle = basic_angle;
    _servo.detach();
}

void ServoHandler::move_to(int target)
{
    _servo.attach(_pin);
    if (_current_angle < target) {
        for (; _current_angle < target; _current_angle++) {
            _servo.write(_current_angle);
            delay(SERVO_MOVE_DELAY);
        }
    } else if (_current_angle > target) {
        for (; _current_angle > target; _current_angle--) {
            _servo.write(_current_angle);
            delay(SERVO_MOVE_DELAY);
        }
    }
    _servo.detach();

    return;
}
