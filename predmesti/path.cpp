#include <stdarg.h>
#include "path.h"
#include "crossing.h"
#include "log.h"
#include "coil_semaphore.h"

VPath::VPath(int id, const String& name, int crossingId, int magnetId)
  : id(id), name(name), _crossingId(crossingId), _magnetId(magnetId) {}

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
  dump();
}

void VPath::unreserve() {
  if (_state == VPathStatus::reserved) {
    _state = VPathStatus::clear;
    dump();
  }
}

void VPath::timeout() {
  extern CoilSemaphore* magnets[];

  if (is_blocked_by_crossing())
    _occupiedTime = millis();

  if (_state == VPathStatus::occupied_soon) {
    if ( millis() - _occupiedSoonTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      log("State [occupied_soon] on " + name + " released.");
      dump();
    }
  }

  if (_state == VPathStatus::occupied) {
    if (_magnetId > -1)
      if (magnets[_magnetId]->getSignal() == SSignal::red)
        _occupiedTime = millis(); // do not timoeut if coil enabled
    if ( millis() - _occupiedTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      if (_vehicle > -1)
        delete_vehicle(*vehicle());
      log("State [occupied] on " + name + " released.");
      dump();
    }
  }

  if (_state == VPathStatus::clear_soon)
    if ( millis() - _cleanSoonTime > PATH_CLEAR_SOON)
      _state = VPathStatus::clear;

  if (_state == VPathStatus::reserved) {
    if ( millis() - _reservationTime > PATH_TIMEOUT) {
      _state = VPathStatus::clear;
      log("State [reserved] on " + name + " released.");
      dump();
    }
  }
}

void VPath::vehicle_push(Vehicle& vehicle) {
  if (_state != VPathStatus::occupied_soon)
    log("ERROR: Path is not ready for car [" + name + "]!");

  if (_vehicle != -1)
    log("ERROR: Two cars on one position [" + name + "]!");

  _state = VPathStatus::occupied;
  _occupiedTime = millis();
  _vehicle = vehicle.id;
  dump();
}

Vehicle* VPath::vehicle_pull() {
  extern Vehicle* vehicles[];
  int vehicle = _vehicle;

  if (_state != VPathStatus::occupied)
    log("ERROR: Car didn't occupy its path on position [" + name + "]!");

  if (vehicle == -1)
    log("ERROR: No car on position [" + name + "]!");

  _vehicle = -1;
  _state = VPathStatus::clear_soon;
  _cleanSoonTime = millis();
  dump();
  return vehicle > -1 ? vehicles[vehicle] : nullptr;
}

Vehicle* VPath::vehicle() const {
  extern Vehicle* vehicles[];
  if (_vehicle < 0)
    log("ERROR: invalid vehicle, returning nullptr!");
  return (_vehicle > -1) ? vehicles[_vehicle] : nullptr;
}

bool VPath::is_blocked_by_crossing() const {
  extern Crossing* crossings[];
  if (_crossingId == CRUNDEF)
    return false;
  return crossings[_crossingId]->isRed();
}

void VPath::dump() const {
  log("Path " + String(id) + ": " + String(int(_state)) + ", " + String(_vehicle));
}

bool paths_are_clear(int pathIda, int pathIdb, int pathIdc, int pathIdd, int pathIde, int pathIdf) {
  extern VPath* paths[];
  bool result;
  result = paths[pathIda]->is_clear();
  if (result && pathIdb > -1) result &= paths[pathIdb]->is_clear();
  if (result && pathIdc > -1) result &= paths[pathIdc]->is_clear();
  if (result && pathIdd > -1) result &= paths[pathIdd]->is_clear();
  if (result && pathIde > -1) result &= paths[pathIde]->is_clear();
  if (result && pathIdf > -1) result &= paths[pathIdf]->is_clear();
  return result;
}

void paths_reserve(bool inDirection, int pathIda, int pathIdb, int pathIdc, int pathIdd) {
  extern VPath* paths[];
  paths[pathIda]->reserve(inDirection);
  if (pathIdb > -1) paths[pathIdb]->reserve(inDirection);
  if (pathIdc > -1) paths[pathIdc]->reserve(inDirection);
  if (pathIdd > -1) paths[pathIdd]->reserve(inDirection);
}
