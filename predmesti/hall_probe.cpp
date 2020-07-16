#include "hall_probe.h"
#include "switch_junction.h"
#include "coil_semaphore.h"
#include "path.h"
#include "blocks.h"

void probe_event(int id, const char* name, int* occupies, int* releases, int* reserves, int* cancels_reservation) {

  extern VPath* paths[];
  extern Junction *j_a;
  
  Serial.print("-- Hall probe: ");
  Serial.print(name);
  Serial.println("");

  if (id == FH02) {
    if (paths[FH03FH05]->is_clear() && paths[FH03FH06]->is_clear()) {
      // na velky okruh
      j_a->to_plus();
      paths[occupies[0]]->occupy();
      //s02->signal_green();
    } else if (paths[FH12FH13]->is_clear()) {
      // na maly okruh
      j_a->to_minus();
      paths[occupies[1]]->occupy();
      //s02->signal_green();
    } else {
      // na maly okruh
      //s02->signal_red();
    }    
  }

  for (int i=0; i < 2; i++) {

    if (id == FH02) {
      break;
    }

    if (occupies[i] != -1) {
      paths[occupies[i]]->occupy();
    }
  }

  for (int i=0; i < 2; i++) {
    if (releases[i] != -1) {
      paths[releases[i]]->release();
    }
  }

  for (int i=0; i < 2; i++) {
    if (reserves[i] != -1) {
      paths[reserves[i]]->reserve();
    }
  }

  for (int i=0; i < 2; i++) {
    if (cancels_reservation[i] != -1) {
      paths[cancels_reservation[i]]->cancel_reservation();
    }
  }


  
}


HallProbe::HallProbe(int id, int pin, const char* name, int* occupies, int* releases, int* reserves, int* cancels_reservation)
  :
  _id(id),
  _pin(pin),
  _name(name),
  _occupies(occupies),
  _releases(releases),
  _reserves(reserves),
  _cancels_reservation(cancels_reservation)
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
        probe_event(_id, _name, _occupies, _releases, _reserves, _cancels_reservation);
      }
    }
  }

  _lastState = _reading;
}

unsigned long HallProbe::getLastPositive()
{
  return _last_positive_time;
}
