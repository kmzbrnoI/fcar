#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>
#include "blocks.h"


const int VEHICLE_COUNT = 10;

enum class VehicleType { car, bus };

class Vehicle {
  public:
    const int id;
    VehicleType type = VehicleType::car;

    Vehicle(int id, VehicleType type);
    ~Vehicle();
    int get_id() const;

    void bus_stop();
    bool is_bus_ready() const;

    void add_turn();
    int get_turn() const;
    String type_str() const;

  private:
    unsigned long _stop_time = 0;
    int _turn = 0;
};

Vehicle& new_vehicle(VehicleType vehicle_type);
void delete_vehicle(Vehicle& vehicle);
void move_car(int start, int target);
void create_car(int target);

#endif /* VEHICLE_H */
