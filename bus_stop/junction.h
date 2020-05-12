#ifndef JUNCTION_H
#define JUNCTION_H

#include <Arduino.h>
#include <Servo.h>

#define PLUS_STATE 0
#define MINUS_STATE 1

class Junction
{
  public:
    Junction(String name, int pin, int plus_angle, int minus_angle);
    int getState();
    void to_plus();
    void to_minus();

  private: 
    Servo _servo;
    const String _name;
    const int _pin;
    const int _plus_angle;
    const int _minus_angle;
    int _state;
    
    int _current_angle;

    void move_to(int angle);
};
#endif /* JUNCTION_H */
