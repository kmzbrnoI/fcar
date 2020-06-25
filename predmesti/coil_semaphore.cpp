#include "coil_semaphore.h"

CoilSemaphore::CoilSemaphore(int pin)
  :
  _pin(pin)
{
  pinMode(_pin, OUTPUT);
  signal_green();
}

SSignal CoilSemaphore::getSignal()
{
  return _state;
}

void CoilSemaphore::signal_green()
{
  digitalWrite(_pin, LOW);
  _state = SSignal::green;
}

void CoilSemaphore::signal_red()
{
  digitalWrite(_pin, HIGH);
  _state = SSignal::red;
}
