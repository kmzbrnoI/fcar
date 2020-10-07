#include "hall_probe.h"
#include "log.h"

void HallProbe::changed() {
  extern VPath* paths[];
  extern CoilSemaphore* magnets[];

  log("-- Hall probe: " + _name);

  if (_id == FHA2) {
    paths_reserve(true, FHA2FHA0, FHA0FH02, FH02FM02);
    paths[FM10FH02]->reserve(false);
    create_vehicle(FHA2FHA0);
  }
  if (_id == FHA0) {
    if (paths[FHA2FHA0]->vehicle() != nullptr) {
      if (paths[FHA2FHA0]->expecting_bus) {
        paths[FHA2FHA0]->vehicle()->type = VehicleType::bus;
        paths[FHA2FHA0]->expecting_bus = false;
      }
      paths[FHA2FHA0]->vehicle()->should_leave = false; // valid incoming vehicle -> do not leave yet
    }
    move_vehicle(FHA2FHA0, FHA0FH02);
    paths[FHA2FHA0]->unreserve();
  }
  if (_id == FHA1) {
    paths[FHA2FHA0]->expecting_bus = true;
    log("Detekovan autobus");
  }

  // prijezd od depa
  if (_id == FH02) {
    if (paths[FM10FH02]->is_occupied())
      move_vehicle(FM10FH02, FH02FM02);
    else
      move_vehicle(FHA0FH02, FH02FM02);
    paths[FM10FH02]->unreserve();
    paths[FM11FH23]->unreserve();
    magnets[FM02]->make_decision();
  }

  // velky okruh
  if (_id == FH03) {
    if (paths[FM02FH03]->vehicle() != nullptr && paths[FM02FH03]->vehicle()->type == VehicleType::bus)
      move_vehicle(FM02FH03, FH03FH06);
    else
      move_vehicle(FM02FH03, FH03FH05);
  }
  if (_id == FH05) {
    move_vehicle(FH03FH05, FH05FH07);
  }
  // autobusova zastavka
  if (_id == FH06) {
    move_vehicle(FH03FH06, FH06FM06);
    magnets[FM06]->make_decision();
  }
  if (_id == FH07) {
    if(paths[FH05FH07]->is_reserved()) {
      paths[FH05FH07]->unreserve();
      move_vehicle(FM06FH07, FH07FM07);
    } else {
      move_vehicle(FH05FH07, FH07FM07);
    }
    magnets[FM07]->make_decision();
  }
  if (_id == FH08) {
    move_vehicle(FM07FH08, FH08FM08);
    magnets[FM08]->make_decision();
  }

  // maly okruh
  if (_id == FH13) {
    move_vehicle(FM02FH13, FH13FM14);
    paths[FM11FH23]->unreserve();
    magnets[FM14]->make_decision();
  }

  // opousteni oblasti
  if (_id == FH09) {
    if (paths[FM08FH09]->is_occupied() && paths[FM14FH09]->is_reserved()) {
      move_vehicle(FM08FH09, FH09FM09);
      paths[FM14FH09]->unreserve();
    }
    else if (paths[FM08FH09]->is_reserved() && paths[FM14FH09]->is_occupied()) {
      move_vehicle(FM14FH09, FH09FM09);
      paths[FM08FH09]->unreserve();
    } else {
      create_vehicle(FH09FM09);
    }
    magnets[FM09]->make_decision();
  }
  if (_id == FH10) {
    move_vehicle(FM09FH10, FH10FM10);
    paths[FH10FM10]->vehicle()->should_leave = true;
    paths[FM09FH11]->unreserve();
    magnets[FM10]->make_decision();
  }

  // opousteci trasa k Hradu
  if (_id == FH11) {
    move_vehicle(FM09FH11, FH11FM11);
    paths[FM09FH10]->unreserve();
    magnets[FM11]->make_decision();
  }
  if (_id == FH23) {
    paths[FM10FH02]->unreserve();
    paths[FM02FH13]->unreserve();
    Vehicle* vehicle = paths[FM11FH23]->vehicle_pull();
    if (vehicle != nullptr)
      delete_vehicle(*vehicle);
  }
}

HallProbe::HallProbe(int id, int pin, const String& name)
  :
  _id(id),
  _pin(pin),
  _name(name)
{
  _last_positive_time = millis() - 1000;
  pinMode(_pin, INPUT_PULLUP);
}

void HallProbe::updateState()
{
  _reading = digitalRead(_pin);

  if (_reading != _lastState) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) {
    if (_reading != _state) {
      _state = _reading;
      if (_state == LOW) {
        _last_positive_time = millis();
        extern HallProbe* probes[];
        probes[_id]->changed();
      }
    }
  }

  _lastState = _reading;
}
