#include "coil_semaphore.h"
#include "vehicle.h"

CoilSemaphore::CoilSemaphore(int id, int pin, const String& name)
  :
  _id(id),
  _pin(pin),
  _name(name) {
  pinMode(_pin, OUTPUT);
  signal_green();
}

SSignal CoilSemaphore::getSignal() {
  return _state;
}

void CoilSemaphore::make_decision(VPath* path) {

  extern VPath* paths[];
  extern Vehicle* vehicles[];
  extern CoilSemaphore* magnets[];
  extern Junction *j_a;
  extern Junction *j_b;
  extern Junction *j_c;

  if (path->is_blocked_by_crossing()) {
    return;
  }

  if (_id == FM02) {
    if (paths_are_clear(FM02FH03, FH03FH05, FH05FH07, FH07FM07, FH06FM06)) {
      // na velky okruh

      if (paths[FH02FM02]->get_vehicle_type() == VehicleType::bus) {
        j_a->to_plus();
        j_b->to_minus();
        paths[FM02FH03]->reserve(true);
        paths[FH03FH06]->reserve(true);
        paths[FH06FM06]->reserve(true);
        magnets[FM02]->signal_green();
        vehicles[paths[FH02FM02]->get_car_id()]->add_turn();
        move_car(FH02FM02, FM02FH03);
      } else {
        j_a->to_plus();
        j_b->to_plus();
        paths[FM02FH03]->reserve(true);
        paths[FH03FH05]->reserve(true);
        paths[FH05FH07]->reserve(true);
        paths[FH07FM07]->reserve(true);
        magnets[FM02]->signal_green();
        vehicles[paths[FH02FM02]->get_car_id()]->add_turn();
        move_car(FH02FM02, FM02FH03);
      }
    } else if (paths_are_clear(FH13FM13, FM02FH13)) {
      // na maly okruh
      j_a->to_minus();
      paths[FM02FH13]->reserve(true);
      paths[FH13FM13]->reserve(true);
      paths[FM11FH22]->reserve(false);
      magnets[FM02]->signal_green();
      vehicles[paths[FH02FM02]->get_car_id()]->add_turn();
      move_car(FH02FM02, FM02FH13);
    } else {
      magnets[FM02]->signal_red();
    }
  } else if (_id == FM06) {
    if (paths_are_clear(FM06FH07, FH07FM07) && vehicles[paths[FH06FM06]->get_car_id()]->is_bus_ready()) {
      paths[FM06FH07]->reserve(true);
      paths[FH07FM07]->reserve(true);
      paths[FH05FH07]->reserve(false);
      magnets[FM06]->signal_green();
      move_car(FH06FM06, FM06FH07);
    } else {
      magnets[FM06]->signal_red();
    }
  } else if (_id == FM07) {
    if (paths_are_clear(FM07FH08, FH08FM08)) {
      paths[FM07FH08]->reserve(true);
      paths[FH08FM08]->reserve(true);
      magnets[FM07]->signal_green();
      move_car(FH07FM07, FM07FH08);
    } else {
      magnets[FM07]->signal_red();
    }
  } else if (_id == FM08) {
    if (paths_are_clear(FM08FH09, FH09FM09, FM13FH09)) {
      paths[FM08FH09]->reserve(true);
      paths[FH09FM09]->reserve(true);
      paths[FM13FH09]->reserve(false);
      magnets[FM08]->signal_green();
      move_car(FH08FM08, FM08FH09);
    } else {
      magnets[FM08]->signal_red();
    }
  } else if (_id == FM09) {
    if (vehicles[paths[FH09FM09]->get_car_id()]->get_turn() > 1) {
      if (paths_are_clear(FM09FH11, FH11FM11)) {
        j_c->to_minus();
        paths[FM09FH11]->reserve(true);
        paths[FH11FM11]->reserve(true);
        paths[FM09FH10]->reserve(false);
        magnets[FM09]->signal_green();
        move_car(FH09FM09, FM09FH11);
      } else {
        magnets[FM09]->signal_red();
      }
    } else {
      if (paths_are_clear(FM09FH10, FH10FM10)) {
        j_c->to_plus();
        paths[FM09FH10]->reserve(true);
        paths[FH10FM10]->reserve(true);
        paths[FM09FH11]->reserve(false);
        magnets[FM09]->signal_green();
        move_car(FH09FM09, FM09FH10);
      } else {
        magnets[FM09]->signal_red();
      }
    }
  } else  if (_id == FM10) {
    if (paths_are_clear(FM10FH02, FH02FM02, FH22FH23, FHA0FH02, FHA2FHA0)) {
      paths[FM10FH02]->reserve(true);
      paths[FH02FM02]->reserve(true);
      paths[FH22FH23]->reserve(false);
      magnets[FM10]->signal_green();
      move_car(FH10FM10, FM10FH02);
    } else {
      magnets[FM10]->signal_red();
    }
  } else if (_id == FM11) {
    if (paths_are_clear(FM11FH22, FH22FH23, FM10FH02, FM02FH13)) {
      j_c->to_plus();
      paths[FM11FH22]->reserve(true);
      paths[FH22FH23]->reserve(true);
      paths[FM02FH13]->reserve(false);
      paths[FM10FH02]->reserve(false);
      magnets[FM11]->signal_green();
      move_car(FH11FM11, FM11FH22);
    } else {
      magnets[FM11]->signal_red();
    }
  } else if (_id == FM13) {
    if (paths_are_clear(FM13FH09, FH09FM09, FM08FH09)) {
      j_c->to_plus();
      paths[FM13FH09]->reserve(true);
      paths[FH09FM09]->reserve(true);
      paths[FM08FH09]->reserve(false);
      magnets[FM13]->signal_green();
      move_car(FH13FM13, FM13FH09);
    } else {
      magnets[FM13]->signal_red();
    }
  }
}

void CoilSemaphore::signal_green() {
  digitalWrite(_pin, LOW);
  _state = SSignal::green;

  //Serial.print("-- GREEN : ");
  //Serial.println(_name);
}

void CoilSemaphore::signal_red() {
  digitalWrite(_pin, HIGH);
  _state = SSignal::red;

  //Serial.print("-- RED : ");
  //Serial.println(_name);
}
