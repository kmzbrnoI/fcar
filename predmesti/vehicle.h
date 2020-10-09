#ifndef VEHICLE_H
#define VEHICLE_H

#include <Arduino.h>
#include "blocks.h"

const int VEHICLE_COUNT = 10;
const int BUS_STOP_TIME_MS = 5000;

enum class VehicleType { car, bus };

struct Vehicle {
  const int id;
  VehicleType type = VehicleType::car;
  bool should_leave = true;
  bool was_big_circuit = false;
  unsigned long _stop_time = 0;

  Vehicle(int id, VehicleType type);
  ~Vehicle();

  void record_stop_time();
  bool is_bus_ready() const;

  String type_str() const;
};

Vehicle& new_vehicle(VehicleType vehicle_type);
void delete_vehicle(Vehicle& vehicle);
void move_vehicle(int start, int target);
void create_vehicle(int target);

#endif /* VEHICLE_H */
