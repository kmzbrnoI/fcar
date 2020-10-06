#include "hall_probe.h"

void probe_event(int id, String& name) {

  extern HallProbe* probes[];
  extern VPath* paths[];
  extern Vehicle* vehicles[];
  extern CoilSemaphore* magnets[];

  //Serial.print("-- Hall probe: ");
  //Serial.println(name);

  if (id == FHA2) {
    paths[FHA2FHA0]->reserve(true);
    paths[FHA0FH02]->reserve(true);
    paths[FH02FM02]->reserve(true);
    paths[FM10FH02]->reserve(false);
    move_car(FHA2FHA0, FHA2FHA0);
    Serial.println("Neco prijelo od Depa...");
  }
  if (id == FHA0) {
    move_car(FHA2FHA0, FHA0FH02);
    paths[FHA2FHA0]->unreserve();
    Serial.println("...a uz to vjizdi do Predmesti");
  }

  if (id == FHA1) {
    paths[FHA2FHA0]->expect_bus();
    Serial.println("... a je to autobus... ");
  }

  // prijezd od depa
  if (id == FH02) {
    if (paths[FM10FH02]->is_occupied()) {
      move_car(FM10FH02, FH02FM02);
      paths[FH22FH23]->unreserve();
    } else {
      move_car(FHA0FH02, FH02FM02);
      paths[FH22FH23]->unreserve();
    }
    magnets[FM02]->make_decision(paths[FH02FM02]);
  }

  // velky okruh
  if (id == FH03) {
    if(paths[FM02FH03]->get_vehicle_type() == VehicleType::bus) {
      move_car(FM02FH03, FH03FH06);
    } else {
      move_car(FM02FH03, FH03FH05);
    }
  }
  if (id == FH05) {
    move_car(FH03FH05, FH05FH07);
  }
  // autobusova zastavka
  if (id == FH06) {
    move_car(FH03FH06, FH06FM06);
    vehicles[paths[FH06FM06]->get_car_id()]->bus_stop();
    magnets[FM06]->make_decision(paths[FH06FM06]);
  }
  if (id == FH07) {
    if(paths[FH05FH07]->is_reserved()) {
      paths[FH05FH07]->unreserve();
      move_car(FM06FH07, FH07FM07);
    } else {
      move_car(FH05FH07, FH07FM07);
    }
    magnets[FM07]->make_decision(paths[FH07FM07]);
  }
  if (id == FH08) {
    move_car(FM07FH08, FH08FM08);
    magnets[FM08]->make_decision(paths[FH08FM08]);
  }


  // maly okruh
  if (id == FH13) {
    move_car(FM02FH13, FH13FM13);
    paths[FM11FH22]->unreserve();
    magnets[FM13]->make_decision(paths[FH13FM13]);
  }


  // opousteni oblasti
  if (id == FH09) {
    if (paths[FM08FH09]->is_occupied() && paths[FM13FH09]->is_reserved()) {
      move_car(FM08FH09, FH09FM09);
      paths[FM13FH09]->unreserve();
    }
    else if (paths[FM08FH09]->is_reserved() && paths[FM13FH09]->is_occupied()) {
      move_car(FM13FH09, FH09FM09);
      paths[FM08FH09]->unreserve();
    }
    magnets[FM09]->make_decision(paths[FH09FM09]);
  }
  if (id == FH10) {
    move_car(FM09FH10, FH10FM10);
    paths[FM09FH11]->unreserve();
    magnets[FM10]->make_decision(paths[FH10FM10]);
  }

  // opousteci trasa k Hradu
  if (id == FH11) {
    move_car(FM09FH11, FH11FM11);
    paths[FM09FH10]->unreserve();
    magnets[FM11]->make_decision(paths[FH11FM11]);
  }
  if (id == FH22) {
    paths[FM02FH13]->unreserve();
    move_car(FM11FH22, FH22FH23);
  }
  if (id == FH23) {
    paths[FM10FH02]->unreserve();
    vehicles[paths[FH22FH23]->vehicle_pull()]->deactivate();
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
        probe_event(_id, _name);
      }
    }
  }

  _lastState = _reading;
}
