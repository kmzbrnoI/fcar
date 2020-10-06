#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>
#include "blocks.h"


const int VEHICLE_COUNT = 10;

enum class VehicleType { car, bus };

class Vehicle
{
  public:
    VehicleType type = VehicleType::car;

    Vehicle(int id);
    int get_id() const;

    bool is_active() const;
    void activate(VehicleType vehicle_type);
    void deactivate();

    void bus_stop();
    bool is_bus_ready() const;

    void add_turn();
    int get_turn() const;
    String type_str() const;

  private:
    const int _id;

    bool _active = false;

    unsigned long _stop_time = 0;
    int _turn = 0;
};

int get_new_car(VehicleType vehicle_type);
void move_car(int start, int target);
void create_car(int target);

#endif /* VEHICLE_H */
