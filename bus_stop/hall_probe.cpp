#include "hall_probe.h"

HallProbe::HallProbe(String name, int pin)
{
  _name = name;  
  _pin = pin;

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
      if (_state == HIGH) {
        Serial.print(_name);
        Serial.print(":");
        Serial.println(_pin);
      }
    }
  }

  _lastState = _reading;
}
