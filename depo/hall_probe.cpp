#include "hall_probe.h"
#include "blocks.h"
#include "log.h"
#include "path.h"

extern VPath *path_entrance;
extern VPath *path_circuit;
extern VPath *path_stand11;
extern VPath *path_stand12;
extern VPath *path_stand21;
extern VPath *path_stand22;
extern VPath *path_stand31;
extern VPath *path_stand32;

HallProbe::HallProbe(int id, int pin, const String &name, int changeDelayMs)
    : id(id)
    , name(name)
    , _pin(pin, DEBOUNCE_DELAY_MS)
    , changeDelayMs(changeDelayMs)
{
    pinMode(pin, INPUT_PULLUP);
}

void HallProbe::update()
{
    _pin.update();

    if (_pin.fell() && _changeTime == 0)
        _changeTime = millis() + changeDelayMs;

    if ((!_ready) && (millis() >= _soonestEvent))
        _ready = true;

    if ((_changeTime > 0) && (millis() >= _changeTime)) {
        _changeTime = 0;
        if ((this->onOccupied != nullptr) && (_ready)) {
            _ready = false;
            _soonestEvent = millis() + MIN_EVENT_DELAY;
            onOccupied(this);
        }
    }
}
