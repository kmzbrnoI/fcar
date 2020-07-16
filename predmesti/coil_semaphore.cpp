#include "coil_semaphore.h"

CoilSemaphore::CoilSemaphore(int pin, const char* name)
  :
  _pin(pin),
  _name(name)
{
  pinMode(_pin, OUTPUT);
  signal_green();
}

SSignal CoilSemaphore::getSignal()
{
  return _state;
}

void CoilSemaphore::make_decision(int id) {

  extern VPath* paths[];
  extern CoilSemaphore* magnets[];
  extern Junction *j_a;

  if (id == FM02) {
    if (paths[FH03FH05]->is_clear() && paths[FM02FH03]->is_clear()) {
      // na velky okruh
      j_a->to_plus();
      paths[FM02FH03]->occupy();
      paths[FH03FH05]->occupy();
      magnets[FM02]->signal_green();
      paths[FH02FM02]->release();
      return;
    } else if (paths[FH13FM13]->is_clear() && paths[FM02FH13]->is_clear()) {      
      // na maly okruh
      j_a->to_minus();
      paths[FM02FH13]->occupy();
      paths[FH13FM13]->occupy();
      magnets[FM02]->signal_green();
      paths[FH02FM02]->release();
      return;
    } else {
      magnets[FM02]->signal_red();
      return;
    }
  }
}

void CoilSemaphore::signal_green()
{
  digitalWrite(_pin, LOW);
  _state = SSignal::green;

  Serial.print("-- GREEN : ");
  Serial.println(_name);
}

void CoilSemaphore::signal_red()
{
  digitalWrite(_pin, HIGH);
  _state = SSignal::red;

  //Serial.print("-- RED : ");
  //Serial.println(_name);
}
