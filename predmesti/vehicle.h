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
    int get_id() const;
    VehicleType type() const;

    bool is_active() const;
    void activate(VehicleType vehicle_type);
    void deactivate();

    void bus_stop();
    bool is_bus_ready() const;

    void add_turn();
    int get_turn() const;

  private:
    const int _id;
    VehicleType _vehicle_type;
    bool _active;

    unsigned long _stop_time;
    int _turn;
};

int get_new_car(VehicleType vehicle_type);
void move_car(int start, int target);

#endif /* VEHICLE_H */
