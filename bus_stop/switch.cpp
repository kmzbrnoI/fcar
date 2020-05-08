#include "switch.h"

Switch::Switch(String name, int pin, int plus_angle, int minus_angle)
  :
  _name(name),
  _pin(pin),
  _plus_angle(plus_angle),
  _minus_angle(minus_angle)
{
  _current_angle = _plus_angle;

  _servo = Servo();
  _servo.attach(_pin);
  _servo.write(_current_angle);
}

void Switch::to_plus()
{
  move_to(_plus_angle);
}

void Switch::to_minus()
{
  move_to(_minus_angle);
}

void Switch::move_to(int target)
{
  if (_current_angle < target) {
    for (; _current_angle < target; _current_angle += 1) {
      _servo.write(_current_angle);

      Serial.print("A ");
      Serial.print(_current_angle);
      Serial.print(" ");
      Serial.println(target);

      delay(500);
    }
    Serial.println("A done");
    delay(2000);
    return;
  }
  if (_current_angle > target) {
    for (; _current_angle > target; _current_angle -= 1) {
      _servo.write(_current_angle);

      Serial.print("B ");
      Serial.print(_current_angle);
      Serial.print(" ");
      Serial.println(target);

      delay(500);
    }
    
    Serial.println("B done");
    delay(2000);
    return;
  }
}
