#ifndef SERVO_HANDLER_H
#define SERVO_HANDLER_H

#include <Arduino.h>
#include <Servo.h>

#define SERVO_MOVE_DELAY 3


class ServoHandler
{
  public:
    ServoHandler(int pin, int basic_angle);
    void move_to(int angle);

  private: 
    Servo _servo;
    int _current_angle;
};
#endif /* SERVO_HANDLER_H */
