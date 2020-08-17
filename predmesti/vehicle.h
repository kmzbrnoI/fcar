#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>
#include "path.h"

const int VEHICLE_COUNT = 10;

enum class VehicleType { car, bus };

class Vehicle
{
  public:
    Vehicle(int id);
    int get_id();
    bool is_active();
    void activate(VehicleType vehicle_type);
    void deactivate();
    void set_position(int probe);

  private:
    const int _id;
    VehicleType _vehicle_type;
    bool _active;

    int recent_probe;
    int previous_probe;
};

int get_new_car(VehicleType vehicle_type);
void move_car(int start, int target, VehicleType vehicle_type);

#endif /* VEHICLE_H */
