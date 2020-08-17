#include "vehicle.h"

Vehicle::Vehicle(int id)
  :
  _id(id),
  _active(false)
{
  ;
}

int Vehicle::get_id()
{
  return _id;  
}

bool Vehicle::is_active()
{
  return _active;  
}

void Vehicle::activate(VehicleType vehicle_type)
{
  _active = true;  
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

void move_car(int start, int target, VehicleType vehicle_type)
{
  extern VPath* paths[]; 

  int vehicle = -1; 
  
  vehicle = paths[start]->vehicle_pull();
  if (vehicle == -1) {
    vehicle = get_new_car(VehicleType::car);      
      Serial.print("Path ");
      Serial.print(paths[start]->get_name());
      Serial.println(" creates new car.");
  }
  paths[target]->vehicle_push(vehicle);
}
