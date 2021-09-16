#include "path.h"
#include "log.h"

VPath::VPath(int id, const String &name, VPathStatus state)
    : id(id)
    , name(name)
    , _state(state)
{
    ;
}

VPathStatus VPath::state() const { return _state; }

bool VPath::is_clear() const { return (_state == VPathStatus::clear); }

bool VPath::is_occupied() const { return (_state == VPathStatus::occupied); }

void VPath::setState(VPathStatus state)
{
    if (state == _state)
        return;
    _state = state;
    dump();
}

void VPath::occupy()
{
    _occupiedTime = millis();
    setState(VPathStatus::occupied);
}

void VPath::clear() { setState(VPathStatus::clear); }

void VPath::update()
{
    if (_state == VPathStatus::occupied) {
        onOccupied(this);
    }
}

void VPath::dump() const { log("Path " + name + ": " + String(int(_state))); }
