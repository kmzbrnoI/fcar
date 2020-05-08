#ifndef SWITCH_H
#define SWITCH_H

#include <Arduino.h>
#include <Servo.h>

class Switch
{
  public:
    Switch(String name, int pin, int plus_angle, int minus_angle);
    void to_plus();
    void to_minus();

  private: 
    Servo _servo;
    const String _name;
    const int _pin;
    const int _plus_angle;
    const int _minus_angle;
    int _current_angle;

    void move_to(int angle);
};
#endif /* SWITCH_H */
