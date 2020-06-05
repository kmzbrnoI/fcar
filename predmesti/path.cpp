#include "path.h"

VPath::VPath()
  : _name(name)
{
  _state = VPathStatus::clear;
}

void VPath::occupy() {
  _state = VPathStatus::occupied;
  Serial.print(_name);
  Serial.println(" occupied.");
}

void VPath::release() {
  _state = VPathStatus::clear;
  Serial.print(_name);
  Serial.println(" released.");
}
