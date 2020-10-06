#include "vehicle.h"
#include "path.h"
#include "log.h"

Vehicle::Vehicle(int id) : _id(id) {}

int Vehicle::get_id() const {
  return _id;
}

String Vehicle::type_str() const {
  return (type == VehicleType::bus) ? "bus" : "car";
}

bool Vehicle::is_active() const {
  return _active;
}

void Vehicle::activate(VehicleType vehicle_type) {
  _active = true;
  type = vehicle_type;
  _turn = 0;
  log("INFO: Car [" + String(_id) + "] established as " + type_str());
}

void Vehicle::deactivate() {
  _active = false;
}

void Vehicle::bus_stop() {
  if (type == VehicleType::bus)
    _stop_time = millis();
}

bool Vehicle::is_bus_ready() const {
  return type == VehicleType::bus ? (millis() - _stop_time > 10000) : true;
}

void Vehicle::add_turn() {
  _turn = _turn + 1;
}

int Vehicle::get_turn() const {
  return _turn;
}

int get_new_car(VehicleType vehicle_type) {
  extern Vehicle* vehicles[];

  for (int i=0; i < VEHICLE_COUNT; i++) {
    if ( ! vehicles[i]->is_active() ) {
      vehicles[i]->activate(vehicle_type);
      return i;
    }
  }

  log("ERROR: Out of empty cars!");
  return -1;
}

void move_car(int start, int target) {
  extern Vehicle* vehicles[];
  extern VPath* paths[];
  int vehicle = -1;

  if (start > -1) {
    vehicle = paths[start]->vehicle_pull();
    if (vehicle == -1) {
      vehicle = get_new_car(VehicleType::car);
      log("Path " + paths[target]->name() + " creates new car.");
    }
    log("Moving " + vehicles[vehicle]->type_str() + " " + String(vehicle) + " from "
        + paths[start]->name() + " to " + paths[target]->name());
  } else {
    vehicle = get_new_car(VehicleType::car);
  }
  paths[target]->vehicle_push(vehicle);
}

void create_car(int target) {
  move_car(-1, target);
}
