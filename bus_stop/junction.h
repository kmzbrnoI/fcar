#ifndef JUNCTION_H
#define JUNCTION_H

#include <Arduino.h>
#include "servo_handler.h"

#define JUNCTION_PLUS_DIRECTION 1
#define JUNCTION_MINUS_DIRECTION 2

class Junction
{
  public:
    Junction(int pin, int plus_angle, int minus_angle);
    int getDirection();
    void to_plus();
    void to_minus();

  private: 
    ServoHandler* _servo_handler;
    const int _plus_angle;
    const int _minus_angle;
    int _state;

};
#endif /* JUNCTION_H */
