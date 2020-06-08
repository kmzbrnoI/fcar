#include "hall_probe.h"

HallProbe::HallProbe(int pin, const char* name)
  :
  _pin(pin),
  _name(name)
{
  _last_positive_time = 0;

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
        Serial.print("----- Hall probe: ");
        Serial.println(_name);
      }
    }
  }

  _lastState = _reading;
}

unsigned long HallProbe::getLastPositive()
{
  return _last_positive_time;
}
