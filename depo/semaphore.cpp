#include "semaphore.h"
#include "log.h"

Semaphore::Semaphore(int pin, int green_angle, int red_angle)
  :
  _green_angle(green_angle),
  _red_angle(red_angle)
{
  _servo_handler = new ServoHandler(pin, _green_angle);
  _state = SSignal::green;
}

SSignal Semaphore::getSignal()
{
  return _state;  
}

void Semaphore::signal_green()
{
  _servo_handler->move_to(_green_angle);
  _state = SSignal::green;
  log("green");
}

void Semaphore::signal_red()
{
  _servo_handler->move_to(_red_angle);
  _state = SSignal::red;
  log("red");
}
