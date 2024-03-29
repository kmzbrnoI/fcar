#include "junction.h"
#include "log.h"

Junction::Junction(const String &name, int pin, int plus_angle, int minus_angle)
    : _name(name)
    , _plus_angle(plus_angle)
    , _minus_angle(minus_angle)
{
    _servo_handler = new ServoHandler(pin, _plus_angle);
    _state = JDirection::plus;
}

JDirection Junction::getDirection() { return _state; }

void Junction::to_plus()
{
    _servo_handler->move_to(_plus_angle);
    _state = JDirection::plus;
    log(this->_name + " plus");
}

void Junction::to_minus()
{
    _servo_handler->move_to(_minus_angle);
    _state = JDirection::minus;
    log(this->_name + " minus");
}
