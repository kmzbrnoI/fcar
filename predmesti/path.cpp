#include "path.h"

VPath::VPath(int id, const char* name)
  : _name(name),
  _id(id),
  _state(VPathStatus::clear),
  _lastOccupiedTime(0),
  _vehicle(-1)
{
  ;
}

VPathStatus VPath::get_state()
{
  return _state;
}

bool VPath::is_clear() {
  if (_state == VPathStatus::clear) {
    return true;
  }
  
  return false;
}

void VPath::occupy() {
  _state = VPathStatus::occupied;
  _lastOccupiedTime = millis();
}

void VPath::release() {
  _state = VPathStatus::clear;
}

void VPath::release_soon() {
  _state = VPathStatus::clear_soon;
  _lastOccupiedTime = millis();
}


void VPath::reserve() {
  _state = VPathStatus::reserved;
  _lastOccupiedTime = millis();
}

void VPath::cancel_reservation() {
  _state = VPathStatus::clear;
}

void VPath::occupation_timeout() {

  if (_state == VPathStatus::occupied || _state == VPathStatus::reserved) {
    if ( millis() - _lastOccupiedTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
    }
  }

  if (_state == VPathStatus::clear_soon) {
    if ( millis() - _lastOccupiedTime > PATH_CLEAR_SOON) {
      _state = VPathStatus::clear;
    }
  }
}

int VPath::vehicle_pull()
{
  int vehicle = _vehicle;
  if (vehicle == -1) {
    Serial.print("ERROR: No car on position [");
    Serial.print(_name);
    Serial.println("]!");    
  }
  _vehicle = -1;
  return vehicle;
}

void VPath::vehicle_push(int vehicle)
{
  if (_vehicle != -1) {
    Serial.print("ERROR: Two cars on one position [");
    Serial.print(_name);
    Serial.println("]!");
  }

  _vehicle = vehicle;
}

const char* VPath::get_name()
{
  return _name;  
}
