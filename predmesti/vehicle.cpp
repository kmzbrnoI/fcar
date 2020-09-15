#include "vehicle.h"
#include "path.h"

Vehicle::Vehicle(int id)
  :
  _id(id),
  _active(false),
  _stop_time(0)
{
  ;
}

int Vehicle::get_id()
{
  return _id;  
}

VehicleType Vehicle::get_type()
{
  return _vehicle_type;  
}

bool Vehicle::is_active()
{
  return _active;  
}

void Vehicle::activate(VehicleType vehicle_type)
{
  _active = true;
  _vehicle_type = vehicle_type;  
}

void Vehicle::deactivate()
{
  _active = false;  
}

void Vehicle::set_position(int probe)
{
    int recent_probe;
    int previous_probe;
  
}

void Vehicle::bus_stop()
{
  _stop_time = millis();
}

bool Vehicle::is_bus_ready()
{
  if (millis() - _stop_time > 10000) {
    return true;
  }
  return false;
}

int get_new_car(VehicleType vehicle_type)
{
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

void move_car(int start, int target)
{
  extern VPath* paths[]; 
  int vehicle = -1; 

  vehicle = paths[start]->vehicle_pull();
  if (vehicle == -1) {
    vehicle = get_new_car(paths[start]->is_expecting());
    Serial.print("Path ");
    Serial.print(paths[start]->get_name());
    Serial.println(" creates new car.");
  }
  paths[target]->vehicle_push(vehicle);
}
