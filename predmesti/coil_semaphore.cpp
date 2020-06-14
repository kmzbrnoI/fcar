#include "coil_semaphore.h"

Semaphore::Semaphore(int pin)
  :
  _pin(pin)
{
  pinMode(_pin, OUTPUT);
  signal_green();
}

SSignal Semaphore::getSignal()
{
  return _state;
}

void Semaphore::signal_green()
{
  digitalWrite(_pin, LOW);
  _state = SSignal::green;
}

void Semaphore::signal_red()
{
  digitalWrite(_pin, HIGH);
  _state = SSignal::red;
}
