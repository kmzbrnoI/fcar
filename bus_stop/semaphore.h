#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <Arduino.h>
#include "servo_handler.h"

#define SEMAPHORE_GREEN 1
#define SEMAPHORE_RED 2

class Semaphore
{
  public:
    Semaphore(int pin, int green_angle, int red_angle);
    int getSignal();
    void signal_green();
    void signal_red();

  private: 
    ServoHandler* _servo_handler;
    const int _green_angle;
    const int _red_angle;
    int _state;

};
#endif /* SEMAPHORE_H */
