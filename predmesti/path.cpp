#include "path.h"

VPath::VPath(const char* name)
  : _name(name),
  _state(VPathStatus::clear)
{
  _state = VPathStatus::clear;
}

void VPath::car_in() {

  
}

void VPath::car_out() {
  
  
}

bool VPath::is_clear() {
  if (_state == VPathStatus::clear) {
    return true;
  }
  
  return false;
}

void VPath::occupy() {
  _state = VPathStatus::occupied;
  Serial.print("---- ");
  Serial.print(_name);
  Serial.println(" occupied.");
}

void VPath::release() {
  _state = VPathStatus::clear;
  Serial.print("---- ");
  Serial.print(_name);
  Serial.println(" released.");
}

void VPath::reserve() {
  _state = VPathStatus::reserved;
  Serial.print("---- ");
  Serial.print(_name);
  Serial.println(" reserved.");
}

void VPath::cancel_reservation() {
  _state = VPathStatus::clear;
  Serial.print("---- ");
  Serial.print(_name);
  Serial.println(" reservation cancelled.");
}
