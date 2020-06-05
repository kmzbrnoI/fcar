#include "hall_probe.h"

HallProbe::HallProbe(int pin)
  :
  _pin(pin)
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
      }
    }
  }

  _lastState = _reading;
}

unsigned long HallProbe::getLastPositive()
{
  return _last_positive_time;
}
