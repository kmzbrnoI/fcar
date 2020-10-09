#include "coil_semaphore.h"
#include "vehicle.h"
#include "log.h"

CoilSemaphore::CoilSemaphore(int id, const String& name, int pin)
  : id(id), name(name), _pin(pin) {
  pinMode(_pin, OUTPUT);
  go();
}

void CoilSemaphore::make_decision() {
  extern VPath* paths[];
  extern CoilSemaphore* magnets[];
  extern Junction *j_a;
  extern Junction *j_b;
  extern Junction *j_c;

  if (id == FM02) {
    bool can_big_circuit = (paths_are_clear(FM02FH03, FH03FH05, FH05FH07, FH07FM07, FH06FM06, FH03FH06) &&
                           (paths[FH02FM02]->vehicle()->is_bus_ready() || paths[FHA2FHA0]->is_occupied()));
    bool can_small_circuit = (paths_are_clear(FH13FM14, FM02FH13, FM11FH23) &&
                             (paths[FH02FM02]->vehicle()->is_bus_ready() || paths[FHA2FHA0]->is_occupied()));
    bool was_big_circuit = paths[FH02FM02]->vehicle()->was_big_circuit;
    if (can_big_circuit && (!was_big_circuit || !can_small_circuit)) {
      j_a->to_plus();

      if (paths[FH02FM02]->vehicle()->type == VehicleType::bus) {
        j_b->to_minus();
        paths_reserve(true, FM02FH03, FH03FH06, FH06FM06);
      } else {
        j_b->to_plus();
        paths_reserve(true, FM02FH03, FH03FH05, FH05FH07, FH07FM07);
      }

      magnets[FM02]->go();
      move_vehicle(FH02FM02, FM02FH03);
    } else if (can_small_circuit) {
      j_a->to_minus();
      paths_reserve(true, FM02FH13, FH13FM14);
      paths[FM11FH23]->reserve(false);
      magnets[FM02]->go();
      move_vehicle(FH02FM02, FM02FH13);
    } else {
      magnets[FM02]->stop();
    }

  } else if (id == FM06) {
    if (paths_are_clear(FM06FH07, FH07FM07) && paths[FH06FM06]->vehicle()->is_bus_ready()) {
      paths_reserve(true, FM06FH07, FH07FM07);
      paths[FH05FH07]->reserve(false);
      magnets[FM06]->go();
      move_vehicle(FH06FM06, FM06FH07);
    } else {
      magnets[FM06]->stop();
    }

  } else if (id == FM07) {
    if (paths_are_clear(FM07FH08, FH08FM08)) {
      paths_reserve(true, FM07FH08, FH08FM08);
      magnets[FM07]->go();
      move_vehicle(FH07FM07, FM07FH08);
    } else {
      magnets[FM07]->stop();
    }

  } else if (id == FM08) {
    if (paths_are_clear(FM08FH09, FH09FM09, FM14FH09)) {
      paths_reserve(true, FM08FH09, FH09FM09);
      paths[FM14FH09]->reserve(false);
      magnets[FM08]->go();
      move_vehicle(FH08FM08, FM08FH09);
    } else {
      magnets[FM08]->stop();
    }

  } else if (id == FM09) {
    if (!paths[FH09FM09]->vehicle()->is_bus_ready()) {
      magnets[FM09]->stop();
    } else if (paths[FH09FM09]->vehicle()->should_leave && paths_are_clear(FM09FH11, FH11FM11)) {
      j_c->to_minus();
      paths_reserve(true, FM09FH11, FH11FM11);
      paths[FM09FH10]->reserve(false);
      magnets[FM09]->go();
      move_vehicle(FH09FM09, FM09FH11);
    } else if (paths_are_clear(FM09FH10, FH10FM10)) {
      j_c->to_plus();
      paths_reserve(true, FM09FH10, FH10FM10);
      paths[FM09FH11]->reserve(false);
      magnets[FM09]->go();
      move_vehicle(FH09FM09, FM09FH10);
    } else {
      magnets[FM09]->stop();
    }

  } else  if (id == FM10) {
    if (paths_are_clear(FM10FH02, FH02FM02, FM11FH23, FHA0FH02, FHA2FHA0)) {
      paths_reserve(true, FM10FH02, FH02FM02);
      paths[FM11FH23]->reserve(false);
      magnets[FM10]->go();
      move_vehicle(FH10FM10, FM10FH02);
    } else {
      magnets[FM10]->stop();
    }

  } else if (id == FM11) {
    if (paths_are_clear(FM11FH23, FM10FH02, FM02FH13)) {
      paths_reserve(true, FM11FH23);
      paths_reserve(false, FM02FH13, FM10FH02);
      magnets[FM11]->go();
      move_vehicle(FH11FM11, FM11FH23);
    } else {
      magnets[FM11]->stop();
    }

  } else if (id == FM14) {
    if (paths_are_clear(FM14FH09, FH09FM09, FM08FH09)) {
      paths_reserve(true, FM14FH09, FH09FM09);
      paths[FM08FH09]->reserve(false);
      magnets[FM14]->go();
      move_vehicle(FH13FM14, FM14FH09);
    } else {
      magnets[FM14]->stop();
    }
  }
}

void CoilSemaphore::go() {
  if (state == SSignal::go) return;
  digitalWrite(_pin, LOW);
  state = SSignal::go;
  log("-- GREEN : " + name);
}

void CoilSemaphore::stop() {
  if (state == SSignal::stop) return;
  digitalWrite(_pin, HIGH);
  state = SSignal::stop;
  log("-- RED : " + name);
}
