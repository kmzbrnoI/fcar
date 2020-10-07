#include "vehicle.h"
#include "path.h"
#include "log.h"

Vehicle::Vehicle(int id, VehicleType type) : id(id), type(type) {
  log("INFO: Vehicle [" + String(id) + "] established.");
  _turn = 0;
}

Vehicle::~Vehicle() {
  log("Vehicle [" + String(id) + "] deactivated.");
}

String Vehicle::type_str() const {
  return (type == VehicleType::bus) ? "bus" : "car";
}

void Vehicle::bus_stop() {
  if (type == VehicleType::bus)
    _stop_time = millis();
}

bool Vehicle::is_bus_ready() const {
  return type == VehicleType::bus ? (millis() - _stop_time > 5000) : true;
}

void Vehicle::add_turn() {
  _turn = _turn + 1;
}

int Vehicle::get_turn() const {
  return _turn;
}

Vehicle& new_vehicle(VehicleType vehicle_type) {
  extern Vehicle* vehicles[];

  for (int i=0; i < VEHICLE_COUNT; i++) {
    if ( vehicles[i] == nullptr ) {
      vehicles[i] = new Vehicle(i, vehicle_type);
      return *vehicles[i];
    }
  }

  log("ERROR: Out of empty cars!");
  return *vehicles[0];
}

void delete_vehicle(Vehicle& vehicle) {
  extern Vehicle* vehicles[];
  int id = vehicle.id;
  delete vehicles[id];
  vehicles[id] = nullptr;
}

void move_car(int start, int target) {
  extern VPath* paths[];
  Vehicle* vehicle;

  if (start > -1) {
    vehicle = paths[start]->vehicle_pull();
    if (vehicle == nullptr) {
      vehicle = &new_vehicle(VehicleType::car);
      log("Path " + paths[target]->name() + " creates new car.");
    }
    log("Moving " + vehicle->type_str() + " " + String(vehicle->id) + " from "
        + paths[start]->name() + " to " + paths[target]->name());
  } else {
    vehicle = &new_vehicle(VehicleType::car);
  }
  paths[target]->vehicle_push(*vehicle);
}

void create_car(int target) {
  move_car(-1, target);
}
