#include "coil_semaphore.h"
#include "vehicle.h"

CoilSemaphore::CoilSemaphore(int pin, const char* name)
  :
  _pin(pin),
  _name(name)
{
  pinMode(_pin, OUTPUT);
  signal_green();
}

SSignal CoilSemaphore::getSignal()
{
  return _state;
}

void CoilSemaphore::make_decision(int id, VPath* path) {

  extern VPath* paths[];
  extern Vehicle* vehicles[];
  extern CoilSemaphore* magnets[];
  extern Junction *j_a;
  extern Junction *j_b;
  extern Junction *j_c;

  if (path->is_blocked_by_crossing()) {
    return;
  }

  if (id == FM02) {
    if (paths[FM02FH03]->is_clear() && paths[FH03FH05]->is_clear() && paths[FH05FH07]->is_clear() && paths[FH07FM07]->is_clear()) {
      // na velky okruh
      
      if (paths[FH02FM02]->get_vehicle_type() == VehicleType::bus) {
        Serial.println("Jedu do zastavky");
        j_a->to_plus();
        j_b->to_minus();
        paths[FM02FH03]->reserve(true);
        paths[FH03FH06]->reserve(true);
        paths[FH06FM06]->reserve(true);
        magnets[FM02]->signal_green();
        move_car(FH02FM02, FM02FH03);
        return;
      } else {
        Serial.println("Nejedu do zastavky");
        j_a->to_plus();
        j_b->to_plus();
        paths[FM02FH03]->reserve(true);
        paths[FH03FH05]->reserve(true);
        paths[FH05FH07]->reserve(true);
        paths[FH07FM07]->reserve(true);
        magnets[FM02]->signal_green();
        move_car(FH02FM02, FM02FH03);
        return;
      }
    } else if (paths[FH13FM13]->is_clear() && paths[FM02FH13]->is_clear()) {      
      // na maly okruh
      j_a->to_minus();
      paths[FM02FH13]->reserve(true);
      paths[FH13FM13]->reserve(true);
      magnets[FM02]->signal_green();
      move_car(FH02FM02, FM02FH13);
      return;
    } else {
      magnets[FM02]->signal_red();
      return;
    }
  }

  if (id == FM06) {

    if (paths[FM06FH07]->is_clear() && paths[FH07FM07]->is_clear() &&
        vehicles[paths[FH06FM06]->get_car_id()]->is_bus_ready())
    {
      paths[FM06FH07]->reserve(true);
      paths[FH07FM07]->reserve(true);
      paths[FH05FH07]->reserve(false);
      magnets[FM06]->signal_green();
      move_car(FH06FM06, FM06FH07);
      return;
    } else {
      magnets[FM06]->signal_red();
      return;
    }
  }

  if (id == FM07) {
    if (paths[FM07FH08]->is_clear() && paths[FH08FM08]->is_clear()) {
      paths[FM07FH08]->reserve(true);
      paths[FH08FM08]->reserve(true);
      magnets[FM07]->signal_green();
      move_car(FH07FM07, FM07FH08);
      return;
    } else {
      magnets[FM07]->signal_red();
      return;
    }
  }

  if (id == FM08) {
    if (paths[FM08FH09]->is_clear() && paths[FH09FM09]->is_clear() && paths[FM13FH09]->is_clear()) {
      paths[FM08FH09]->reserve(true);
      paths[FH09FM09]->reserve(true);
      paths[FM13FH09]->reserve(false);
      magnets[FM08]->signal_green();
      move_car(FH08FM08, FM08FH09);
      Serial.println("-1 A");
      return;
    } else {
      magnets[FM08]->signal_red();
      return;
    }
  }

  if (id == FM09) {
    if (paths[FM09FH10]->is_clear() && paths[FH10FM10]->is_clear()) {
      j_c->to_plus();
      paths[FM09FH10]->reserve(true);
      paths[FH10FM10]->reserve(true);
      magnets[FM09]->signal_green();
      move_car(FH09FM09, FM09FH10);
      return;
    } else {
      magnets[FM09]->signal_red();
      return;
    }
  }

  if (id == FM10) {
    if (paths[FM10FH02]->is_clear() &&
        paths[FH02FM02]->is_clear() &&
        paths[FH22FH23]->is_clear() &&
        paths[FHA0FH02]->is_clear() &&
        paths[FHA2FHA0]->is_clear())
    {
      paths[FM10FH02]->reserve(true);
      paths[FH02FM02]->reserve(true);
      paths[FH22FH23]->reserve(false);
      magnets[FM10]->signal_green();
      move_car(FH10FM10, FM10FH02);
      return;
    } else {
      magnets[FM10]->signal_red();
      return;
    }
  }

  if (id == FM13) {
    if (paths[FM13FH09]->is_clear() && paths[FH09FM09]->is_clear() && paths[FM08FH09]->is_clear()) {
      j_c->to_plus();
      paths[FM13FH09]->reserve(true);
      paths[FH09FM09]->reserve(true);
      paths[FM08FH09]->reserve(false);
      magnets[FM13]->signal_green();
      move_car(FH13FM13, FM13FH09);
      Serial.println("-1 B");
      return;
    } else {
      magnets[FM13]->signal_red();
      return;
    }
  }
}

void CoilSemaphore::signal_green()
{
  digitalWrite(_pin, LOW);
  _state = SSignal::green;

  //Serial.print("-- GREEN : ");
  //Serial.println(_name);
}

void CoilSemaphore::signal_red()
{
  digitalWrite(_pin, HIGH);
  _state = SSignal::red;

  //Serial.print("-- RED : ");
  //Serial.println(_name);
}
