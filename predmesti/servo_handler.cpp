#include "servo_handler.h"

ServoHandler::ServoHandler(int pin, int basic_angle)
{
  _servo.attach(pin);
  _servo.write(basic_angle);
  _current_angle = basic_angle;
}

void ServoHandler::move_to(int target)
{
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

  return;
}
