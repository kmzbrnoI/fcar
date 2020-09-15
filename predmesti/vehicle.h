#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>
#include "blocks.h"


const int VEHICLE_COUNT = 10;

enum class VehicleType { car, bus };

class Vehicle
{
  public:
    Vehicle(int id);
    int get_id();
    VehicleType get_type();
    bool is_active();
    void activate(VehicleType vehicle_type);
    void deactivate();
    void set_position(int probe);
    
    void bus_stop();
    bool is_bus_ready();

  private:
    const int _id;
    VehicleType _vehicle_type;
    bool _active;

    unsigned long _stop_time;

    int recent_probe;
    int previous_probe;
};

int get_new_car(VehicleType vehicle_type);
void move_car(int start, int target);

#endif /* VEHICLE_H */
