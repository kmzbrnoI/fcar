#include "switch_junction.h"

Junction::Junction(int plus_pin, int minus_pin)
  :
  _plus_pin(plus_pin),
  _minus_pin(minus_pin)
{
  pinMode(_plus_pin, OUTPUT);
  pinMode(_minus_pin, OUTPUT);

  to_zero();
}

SJDirection Junction::getDirection()
{
  return _state;
}

void Junction::to_zero()
{
  digitalWrite(_plus_pin, LOW);
  digitalWrite(_minus_pin, LOW);
  _state = SJDirection::zero;
}

void Junction::to_plus()
{
  digitalWrite(_plus_pin, HIGH);
  digitalWrite(_minus_pin, LOW);
  _state = SJDirection::plus;
}

void Junction::to_minus()
{
  digitalWrite(_plus_pin, LOW);
  digitalWrite(_minus_pin, HIGH);
  _state = SJDirection::minus;
}
