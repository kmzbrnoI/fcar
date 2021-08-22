#include "path.h"
#include "log.h"

VPath::VPath(int id, const String &name, VPathStatus state, int pinLed, int pinBtn,
             bool timeoutable)
    : id(id)
    , name(name)
    , _state(state)
    , _pinLed(pinLed)
    , _timeoutable(timeoutable)
{
    if (pinLed > 0) {
        pinMode(pinLed, OUTPUT);
        digitalWrite(pinLed, LOW);
    }
    if (pinBtn > 0) {
        pinMode(pinBtn, INPUT_PULLUP);
        _button = new Bounce(pinBtn, BTN_DEBOUNCE_DELAY_MS);
    }
}

VPathStatus VPath::state() const { return _state; }

bool VPath::is_clear() const { return (_state == VPathStatus::clear); }

bool VPath::is_occupied() const { return (_state == VPathStatus::occupied); }

void VPath::setState(VPathStatus state)
{
    if (state == _state)
        return;
    _state = state;
    digitalWrite(_pinLed, (state == VPathStatus::occupied));
    dump();
}

void VPath::occupy()
{
    _occupiedTime = millis();
    setState(VPathStatus::occupied);
}

void VPath::clear() { setState(VPathStatus::clear); }

void VPath::timeout()
{
    if (_state == VPathStatus::occupied && _timeoutable) {
        if (millis() - _occupiedTime > PATH_TIMEOUT) {
            this->clear();
        }
    }
}

void VPath::dump() const { log("Path " + name + ": " + String(int(_state))); }

void VPath::ledUpdate()
{
    if ((_state == VPathStatus::unknown) && (_pinLed > 0)) {
        _ledState = !_ledState;
        digitalWrite(_pinLed, _ledState);
    }
}

void VPath::btnUpdate()
{
    if (_button == nullptr)
        return;

    _button->update();

    if (_button->fell()) {
        if ((_state == VPathStatus::occupied) || (_state == VPathStatus::unknown))
            this->clear();
        else
            this->occupy();

        if (onBtnChanged != nullptr)
            onBtnChanged(this);
    }
}
