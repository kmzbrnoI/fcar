#include "hall_probe.h"

void probe_event(int id, const char* name) {

  extern HallProbe* probes[];
  extern VPath* paths[];
  extern CoilSemaphore* magnets[];

  //Serial.print("-- Hall probe: ");
  //Serial.println(name);

  // prijezd od depa
  if (id == FH02) {
    move_car(FM10FH02, FH02FM02, VehicleType::car);
    paths[FM10FH02]->release();
    paths[FH22FH23]->cancel_reservation();
    magnets[FM02]->make_decision(FM02);
  }

  // velky okruh
  if (id == FH03) {
    move_car(FM02FH03, FH03FH05, VehicleType::car);    
    paths[FM02FH03]->release();
  }
  if (id == FH05) {
    move_car(FH03FH05, FH05FH07, VehicleType::car);
    paths[FH03FH05]->release();
  }
  // autobusova zastavka
  if (id == FH06) {
    move_car(FH03FH06, FH06FH07, VehicleType::car);
    paths[FH03FH06]->release();
  }
  if (id == FH07) {
    move_car(FH05FH07, FH07FM07, VehicleType::car);
    paths[FH05FH07]->release();
    magnets[FM07]->make_decision(FM07);
  }
  if (id == FH08) {
    move_car(FM07FH08, FH08FM08, VehicleType::car);
    paths[FM07FH08]->release();
    magnets[FM08]->make_decision(FM08);
  }


  // maly okruh
  if (id == FH13) {
    move_car(FM02FH13, FH13FM13, VehicleType::car);
    paths[FM02FH13]->release();
    magnets[FM13]->make_decision(FM13);
  }


  // opousteni oblasti
  if (id == FH09) {
    if (
      paths[FM08FH09]->get_state() == VPathStatus::occupied && 
      paths[FM13FH09]->get_state() == VPathStatus::reserved
    ) {
      move_car(FM08FH09, FH09FM09, VehicleType::car);
      paths[FM08FH09]->release();
    }
    if (
      paths[FM08FH09]->get_state() == VPathStatus::reserved && 
      paths[FM13FH09]->get_state() == VPathStatus::occupied
    ) {
      move_car(FM13FH09, FH09FM09, VehicleType::car);
      paths[FM13FH09]->release();
    }

    magnets[FM09]->make_decision(FM09);
  }
  if (id == FH10) {
    move_car(FM09FH10, FH10FM10, VehicleType::car);
    paths[FM09FH10]->release();
    magnets[FM10]->make_decision(FM10);
  }

}

HallProbe::HallProbe(int id, int pin, const char* name)
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
        probe_event(_id, _name);
      }
    }
  }

  _lastState = _reading;
}
