#include "path.h"

VPath::VPath(int id, const char* name)
  : _name(name),
  _id(id),
  _state(VPathStatus::clear),
  _reservationTime(0),
  _occupiedSoonTime(0),
  _occupiedTime(0),
  _cleanSoonTime(0),
  _vehicle(-1),
  _expecting_bus(false),
  _blocked_by_crossing(false)
{
  ;
}

const char* VPath::get_name()
{
  return _name;  
}

bool VPath::is_clear() {
  bool status = false;
  if (_state == VPathStatus::clear) {
    status = true;
  }
  return status;
}

bool VPath::is_occupied() {
  bool status = false;
  if (_state == VPathStatus::clear_soon ||
      _state == VPathStatus::occupied ||
      _state == VPathStatus::occupied_soon) {
    status = true;
  }
  return status;
}
bool VPath::is_reserved() {
  bool status = false;
  if (_state == VPathStatus::reserved) {
    status = true;
  }
  return status;  
}

void VPath::reserve(bool in_direction) {
  if (in_direction) {
    _state = VPathStatus::occupied_soon;
    _occupiedSoonTime = millis();
  } else {
    _state = VPathStatus::reserved;
    _reservationTime = millis();
  }
}

void VPath::unreserve() {
  _state = VPathStatus::clear;
}

void VPath::timeout() {

  if (_blocked_by_crossing) {
    return;
  }

  if (_state == VPathStatus::occupied_soon) {
    if ( millis() - _occupiedSoonTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      //Serial.print("State [occupied_soon] on ");
      //Serial.print(_name);
      //Serial.println(" released.");
    }
  }

  if (_state == VPathStatus::occupied) {
    if ( millis() - _occupiedTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      //Serial.print("State [occupied] on ");
      //Serial.print(_name);
      //Serial.println(" released.");
    }
  }

  if (_state == VPathStatus::clear_soon) {
    if ( millis() - _cleanSoonTime > PATH_CLEAR_SOON) {
      _state = VPathStatus::clear;
      //Serial.print("State [clear_soon] on ");
      //Serial.print(_name);
      //Serial.println(" released.");
    }
  }

  if (_state == VPathStatus::reserved) {
    if ( millis() - _reservationTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      //Serial.print("State [reserved] on ");
      //Serial.print(_name);
      //Serial.println(" released.");
    }
  }
}

void VPath::vehicle_push(int vehicle)
{
  if (_state != VPathStatus::occupied_soon) {
    Serial.print("ERROR: Path is not ready for car [");
    Serial.print(_name);
    Serial.println("]!");        
  }
  
  if (_vehicle != -1) {
    Serial.print("ERROR: Two cars on one position [");
    Serial.print(_name);
    Serial.println("]!");
  }

  _state = VPathStatus::occupied;
  _occupiedTime = millis();
  _vehicle = vehicle;
}

int VPath::vehicle_pull()
{
  int vehicle = _vehicle;

  if (_state != VPathStatus::occupied) {
    Serial.print("ERROR: Car didn't occupied its path on position [");
    Serial.print(_name);
    Serial.println("]!");        
  }

  if (vehicle == -1) {
    Serial.print("ERROR: No car on position [");
    Serial.print(_name);
    Serial.println("]!");    
  }
  
  _vehicle = -1;
  _state = VPathStatus::clear_soon;
  _cleanSoonTime = millis();
  return vehicle;
}

int VPath::get_car_id()
{
  return _vehicle;
}

VehicleType VPath::get_vehicle_type()
{
  extern Vehicle* vehicles[];
  return vehicles[_vehicle]->get_type();
}

void VPath::expect_bus()
{
  _expecting_bus = true;
}

VehicleType VPath::is_expecting()
{
  VehicleType car_type = VehicleType::car;
  if (_expecting_bus) {
    car_type = VehicleType::bus;
    _expecting_bus = false;
  }

  return car_type;
}

void VPath::red_crossing()
{
  _blocked_by_crossing = true;
}

void VPath::green_crossing()
{
  _blocked_by_crossing = false;
}

bool VPath::is_blocked_by_crossing()
{
  return _blocked_by_crossing;
}
