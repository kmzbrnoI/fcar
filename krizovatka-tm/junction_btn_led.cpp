#include "junction_btn_led.h"
#include "log.h"

void activateLed(const int pin)
{
    if (pin > 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

Junction::Junction(const String &name, int pin, int plus_angle, int minus_angle, int pinLedPlus, int pinLedMinus, int pinBtn)
    : _name(name)
    , _plus_angle(plus_angle)
    , _minus_angle(minus_angle)
    , _pinLedPlus(pinLedPlus)
    , _pinLedMinus(pinLedMinus)
{
    _servo_handler = new ServoHandler(pin, _plus_angle);
    _state = JDirection::plus;

    activateLed(_pinLedPlus);
    activateLed(_pinLedMinus);

    if (pinBtn > 0) {
        pinMode(pinBtn, INPUT_PULLUP);
        _button = new Bounce(pinBtn, BTN_DEBOUNCE_DELAY_MS);
    }
}

JDirection Junction::getDirection() { return _state; }

void Junction::to_plus()
{
    _servo_handler->move_to(_plus_angle);
    _state = JDirection::plus;

    digitalWrite(_pinLedMinus, LOW);
    digitalWrite(_pinLedPlus, HIGH);

    log(this->_name + " plus");
}

void Junction::to_minus()
{
    _servo_handler->move_to(_minus_angle);
    _state = JDirection::minus;

    digitalWrite(_pinLedPlus, LOW);
    digitalWrite(_pinLedMinus, HIGH);

    log(this->_name + " minus");
}

void Junction::updateButton()
{
    if (_button == nullptr)
        return;

    _button->update();

    if (_button->fell()) {
        if (this->_state == JDirection::plus) {
            this->to_minus();
        }
        else if (this->_state == JDirection::minus) {
            this->to_plus();
        }
    }
}
