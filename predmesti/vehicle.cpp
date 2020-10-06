#include "vehicle.h"
#include "path.h"

Vehicle::Vehicle(int id)
  :
  _id(id),
  _active(false),
  _stop_time(0),
  _turn(0)
{}

int Vehicle::get_id() const {
  return _id;
}

VehicleType Vehicle::type() const {
  return _vehicle_type;
}

bool Vehicle::is_active() const {
  return _active;
}

void Vehicle::activate(VehicleType vehicle_type) {
  _active = true;
  _vehicle_type = vehicle_type;
  _turn = 0;
}

void Vehicle::deactivate() {
  _active = false;
}

void Vehicle::bus_stop() {
  if (_vehicle_type == VehicleType::bus)
    _stop_time = millis();
}

bool Vehicle::is_bus_ready() const {
  return _vehicle_type == VehicleType::bus ? (millis() - _stop_time > 10000) : true;
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
      Serial.print("INFO: Car [");
      Serial.print(i);
      Serial.println("] established.");
      return i;
    }
  }

  Serial.println("ERROR: Out of empty cars!");
  return -1;
}

void move_car(int start, int target) {
  extern VPath* paths[];
  int vehicle = -1;

  vehicle = paths[start]->vehicle_pull();
  if (vehicle == -1) {
    vehicle = get_new_car(paths[start]->is_expecting());
    Serial.print("Path ");
    Serial.print(paths[start]->name());
    Serial.println(" creates new car.");
  }
  paths[target]->vehicle_push(vehicle);
}
