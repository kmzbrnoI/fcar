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

void HallProbe::_changed()
{
    /* VÝJEZD */
    if (id == HSSV1) {
        path_circuit->clear();
    }

    /* VJEZDY NA STANOVIŠTĚ */
    if (id == HS11) {
        path_circuit->clear();
        path_stand11->occupy();
    }
    if (id == HS21) {
        path_circuit->clear();
        path_stand21->occupy();
    }
    if (id == HS31) {
        path_circuit->clear();
        path_stand31->occupy();
    }

    /* VÝJEZDY ZE STANOVIŠŤ */
    if (id == HS12) {
        path_stand12->clear();
        path_circuit->occupy();
    }
    if (id == HS22) {
        path_stand22->clear();
        path_circuit->occupy();
    }
    if (id == HS32) {
        path_stand32->clear();
        path_circuit->occupy();
    }

    /* VJEZD */
    if (id == HSSVJ) {
        path_entrance->occupy();
    }
}

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

    if (_changeTime > 0 && millis() >= _changeTime) {
        _changeTime = 0;
        _changed();
    }
}
