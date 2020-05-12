#include "junction.h"

Junction::Junction(String name, int pin, int plus_angle, int minus_angle)
  :
  _name(name),
  _pin(pin),
  _plus_angle(plus_angle),
  _minus_angle(minus_angle)
{
  _current_angle = _plus_angle;

  _servo.attach(_pin);
  _servo.write(_plus_angle);
  _state = PLUS_STATE; 
}

int Junction::getState()
{
  return _state;  
}

void Junction::to_plus()
{
  move_to(_plus_angle);
  _state = PLUS_STATE;
}

void Junction::to_minus()
{
  move_to(_minus_angle);
  _state = MINUS_STATE;
}

void Junction::move_to(int target)
{
  if (_current_angle < target) {
    for (; _current_angle < target; _current_angle++) {
      _servo.write(_current_angle);
      delay(3);
    }

    _current_angle++;
    _servo.write(_current_angle);

    return;
  }
  if (_current_angle > target) {
    for (; _current_angle > target; _current_angle--) {
      _servo.write(_current_angle);
      delay(3);
    }

    _current_angle--;
    _servo.write(_current_angle);

    return;
  }
}
