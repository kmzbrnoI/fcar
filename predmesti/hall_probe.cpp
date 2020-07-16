#include "hall_probe.h"

void probe_event(int id, const char* name) {

  extern VPath* paths[];
  extern CoilSemaphore* magnets[];

  Serial.print("-- Hall probe: ");
  Serial.println(name);

  if (id == FH02) {
    paths[FH02FM02]->occupy();
    magnets[FM02]->make_decision(FM02);
  }
  
  if (id == FH03) {
    paths[FM02FH03]->release();
  }
  if (id == FH05) {
    paths[FH03FH05]->release();
  }
  if (id == FH06) {
    paths[FH03FH06]->release();
  }
  if (id == FH07) {
    paths[FH03FH05]->release();
  }

  if (id == FH13) {
    paths[FM02FH13]->release();
    
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

unsigned long HallProbe::getLastPositive()
{
  return _last_positive_time;
}
