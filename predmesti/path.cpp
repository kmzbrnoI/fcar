#include <stdarg.h>
#include "path.h"
#include "crossing.h"

VPath::VPath(int id, const String& name, int crossingId)
  : _id(id),
  _name(name),
  _crossingId(crossingId),
  _state(VPathStatus::clear),
  _reservationTime(0),
  _occupiedSoonTime(0),
  _occupiedTime(0),
  _cleanSoonTime(0),
  _vehicle(-1),
  _expecting_bus(false)
{}

const String& VPath::name() const {
  return _name;
}

bool VPath::is_clear() const {
  return (_state == VPathStatus::clear && !is_blocked_by_crossing());
}

bool VPath::is_occupied() const {
  return (_state == VPathStatus::clear_soon ||
          _state == VPathStatus::occupied ||
          _state == VPathStatus::occupied_soon);
}

bool VPath::is_reserved() const {
  return (_state == VPathStatus::reserved);
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
  if (is_blocked_by_crossing()) {
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

void VPath::vehicle_push(int vehicle) {
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

int VPath::vehicle_pull() {
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

int VPath::get_car_id() {
  return _vehicle;
}

VehicleType VPath::get_vehicle_type() {
  extern Vehicle* vehicles[];
  return vehicles[_vehicle]->get_type();
}

void VPath::expect_bus() {
  _expecting_bus = true;
}

VehicleType VPath::is_expecting() {
  VehicleType car_type = VehicleType::car;
  if (_expecting_bus) {
    car_type = VehicleType::bus;
    _expecting_bus = false;
  }

  return car_type;
}

bool VPath::is_blocked_by_crossing() {
  extern Crossing* crossings[];
  if (_crossingId == CRUNDEF)
    return false;
  return crossings[_crossingId]->isRed();
}

bool paths_are_clear(int pathIda, int pathIdb, int pathIdc, int pathIdd, int pathIde) {
  extern VPath* paths[];
  bool result;
  result = paths[pathIda]->is_clear();
  if (result && pathIdb > -1) result &= paths[pathIdb]->is_clear();
  if (result && pathIdc > -1) result &= paths[pathIdc]->is_clear();
  if (result && pathIdd > -1) result &= paths[pathIdd]->is_clear();
  if (result && pathIde > -1) result &= paths[pathIde]->is_clear();
  return result;
}

void paths_reserve(bool inDirection, int pathIda, int pathIdb, int pathIdc, int pathIdd) {
  extern VPath* paths[];
  paths[pathIda]->reserve(inDirection);
  if (pathIdb > -1) paths[pathIdb]->reserve(inDirection);
  if (pathIdc > -1) paths[pathIdc]->reserve(inDirection);
  if (pathIdd > -1) paths[pathIdd]->reserve(inDirection);
}
