#ifndef PATH_H
#define PATH_H

#include <Arduino.h>
#include "vehicle.h"

enum class VPathStatus { clear, clear_soon, occupied, occupied_soon, reserved };

const int PATH_TIMEOUT = 20000;
const int PATH_CLEAR_SOON = 150;

class VPath
{
  public:
    VPath(int id, const String& name, int crossingId);
    const String& name() const;

    bool is_clear() const;
    bool is_occupied() const;
    bool is_reserved() const;

    void reserve(bool in_direction);
    void unreserve();
    void timeout();

    void vehicle_push(int vehicle);
    int vehicle_pull();
    int get_car_id();
    VehicleType get_vehicle_type();

    void expect_bus();
    VehicleType is_expecting();

    bool is_blocked_by_crossing();

  private:
    const int _id;
    const String& _name;
    int _crossingId;

    VPathStatus _state;
    int _vehicle;
    bool _expecting_bus;

    unsigned long _reservationTime;
    unsigned long _occupiedSoonTime;
    unsigned long _occupiedTime;
    unsigned long _cleanSoonTime;
};

bool paths_are_clear(int pathIda, int pathIdb = -1, int pathIdc = -1, int pathIdd = -1, int pathIde = -1);
void paths_reserve(bool inDirection, int pathIda, int pathIdb = -1, int pathIdc = -1, int pathIdd = -1);

#endif /* PATH_H */
