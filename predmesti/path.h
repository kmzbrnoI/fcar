#ifndef PATH_H
#define PATH_H

#include <Arduino.h>
#include "vehicle.h"

enum class VPathStatus { clear, clear_soon, occupied, occupied_soon, reserved };

const int PATH_TIMEOUT = 20000;
const int PATH_CLEAR_SOON = 150;

struct VPath {
  const int id;
  const String& name;
  bool expecting_bus = false;
  int _crossingId;
  int _magnetId;
  VPathStatus _state = VPathStatus::clear;
  int _vehicle = -1;

  unsigned long _reservationTime = 0;
  unsigned long _occupiedSoonTime = 0;
  unsigned long _occupiedTime = 0;
  unsigned long _cleanSoonTime = 0;

  VPath(int id, const String& name, int crossingId, int magnetId);

  bool is_clear() const;
  bool is_occupied() const;
  bool is_reserved() const;

  void reserve(bool in_direction);
  void unreserve();
  void timeout();

  void vehicle_push(Vehicle& vehicle);
  Vehicle* vehicle_pull();
  Vehicle* vehicle() const;

  bool is_blocked_by_crossing() const;
  void dump() const;
};

bool paths_are_clear(int pathIda, int pathIdb = -1, int pathIdc = -1, int pathIdd = -1,
                     int pathIde = -1, int pathIdf = -1);
void paths_reserve(bool inDirection, int pathIda, int pathIdb = -1, int pathIdc = -1, int pathIdd = -1);

#endif /* PATH_H */
