#include "coil_semaphore.h"

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

void CoilSemaphore::make_decision(int id) {

  extern VPath* paths[];
  extern CoilSemaphore* magnets[];
  extern Junction *j_a;
  extern Junction *j_b;
  extern Junction *j_c;

  if (id == FM02) {
    if (paths[FM02FH03]->is_clear() && paths[FH03FH05]->is_clear() && paths[FH05FH07]->is_clear() && paths[FH07FM07]->is_clear()) {
      // na velky okruh
      j_a->to_plus();
      j_b->to_plus();
      paths[FM02FH03]->occupy();
      paths[FH03FH05]->occupy();
      paths[FH05FH07]->occupy();
      paths[FH07FM07]->occupy();
      magnets[FM02]->signal_green();
      move_car(FH02FM02, FM02FH03, VehicleType::car);
      paths[FH02FM02]->release_soon();
      return;
    } else if (paths[FH13FM13]->is_clear() && paths[FM02FH13]->is_clear()) {      
      // na maly okruh
      j_a->to_minus();
      paths[FM02FH13]->occupy();
      paths[FH13FM13]->occupy();
      magnets[FM02]->signal_green();
      move_car(FH02FM02, FM02FH13, VehicleType::car);
      paths[FH02FM02]->release_soon();
      return;
    } else {
      magnets[FM02]->signal_red();
      return;
    }
  }

  if (id == FM07) {
    if (paths[FM07FH08]->is_clear() && paths[FH08FM08]->is_clear()) {
      paths[FM07FH08]->occupy();
      paths[FH08FM08]->occupy();
      magnets[FM07]->signal_green();
      move_car(FH07FM07, FM07FH08, VehicleType::car);
      paths[FH07FM07]->release_soon();
      return;
    } else {
      magnets[FM07]->signal_red();
      return;
    }
  }

  if (id == FM08) {
    if (paths[FM08FH09]->is_clear() && paths[FH09FM09]->is_clear() && paths[FM13FH09]->is_clear()) {
      paths[FM08FH09]->occupy();
      paths[FH09FM09]->occupy();
      paths[FM13FH09]->reserve();
      magnets[FM08]->signal_green();
      move_car(FH08FM08, FM08FH09, VehicleType::car);
      paths[FH08FM08]->release_soon();
      return;
    } else {
      magnets[FM08]->signal_red();
      return;
    }
  }

  if (id == FM09) {
    if (paths[FM09FH10]->is_clear() && paths[FH10FM10]->is_clear()) {
      j_c->to_plus();
      paths[FM09FH10]->occupy();
      paths[FH10FM10]->occupy();
      magnets[FM09]->signal_green();
      move_car(FH09FM09, FM09FH10, VehicleType::car);
      paths[FH09FM09]->release_soon();      
      return;
    } else {
      magnets[FM09]->signal_red();
      return;
    }
  }

  if (id == FM10) {
    if (paths[FM10FH02]->is_clear() && paths[FH02FM02]->is_clear() && paths[FH22FH23]->is_clear()) {
      paths[FM10FH02]->occupy();
      paths[FH02FM02]->occupy();
      paths[FH22FH23]->reserve();
      magnets[FM10]->signal_green();
      move_car(FH10FM10, FM10FH02, VehicleType::car);
      paths[FH10FM10]->release_soon();
      return;
    } else {
      magnets[FM10]->signal_red();
      return;
    }
  }

  if (id == FM13) {
    if (paths[FM13FH09]->is_clear() && paths[FH09FM09]->is_clear() && paths[FM08FH09]->is_clear()) {
      j_c->to_plus();
      paths[FM13FH09]->occupy();
      paths[FH09FM09]->occupy();
      paths[FM08FH09]->reserve();
      magnets[FM13]->signal_green();
      move_car(FH13FM13, FM13FH09, VehicleType::car);
      paths[FH13FM13]->release_soon();
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
