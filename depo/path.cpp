#include "path.h"
#include "log.h"

VPath::VPath(const String &name, VPathStatus state, bool timeoutable)
    : name(name)
    , _state(state)
    , _timeoutable(timeoutable)
{
}

bool VPath::is_clear() const { return (_state == VPathStatus::clear); }

bool VPath::is_occupied() const { return (_state == VPathStatus::occupied); }

void VPath::occupy()
{
    _state = VPathStatus::occupied;
    dump();
}

void VPath::clear()
{
    _state = VPathStatus::clear;
    dump();
}

void VPath::timeout()
{
    if (_state == VPathStatus::occupied) {

        if (millis() - _occupiedTime > PATH_TIMEOUT) {
            _state = VPathStatus::clear;
            log("State [occupied] on " + name + " released.");
            dump();
        }
    }
}

void VPath::dump() const { log("Path " + name + ": " + String(int(_state))); }
