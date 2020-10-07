#include "vehicle.h"
#include "path.h"
#include "log.h"

Vehicle::Vehicle(int id, VehicleType type) : id(id), type(type) {
  log("INFO: Vehicle [" + String(id) + "] established.");
}

Vehicle::~Vehicle() {
  log("Vehicle [" + String(id) + "] deactivated.");
}

String Vehicle::type_str() const {
  return (type == VehicleType::bus) ? "bus" : "car";
}

void Vehicle::record_stop_time() {
  if (type == VehicleType::bus)
    _stop_time = millis();
}

bool Vehicle::is_bus_ready() const {
  return type == VehicleType::bus ? (millis() - _stop_time > BUS_STOP_TIME_MS) : true;
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

void move_vehicle(int start, int target) {
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
  vehicle->record_stop_time(); // record on each moving event
  paths[target]->vehicle_push(*vehicle);
}

void create_vehicle(int target) {
  move_vehicle(-1, target);
}
