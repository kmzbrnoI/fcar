#ifndef PATH_H
#define PATH_H

#include <Arduino.h>

enum class VPathStatus { clear = 0, occupied = 1, unknown = 2 };

const int PATH_TIMEOUT = 20000;

struct VPath {
    const String name;
    VPathStatus _state;
    int _pinLed;
    bool _timeoutable;
    int _ledState = 0;

    // unsigned long _reservationTime = 0;
    unsigned long _occupiedTime = 0;

    VPath(const String &name, VPathStatus state, int pinLed = 0, bool timeoutable = false);

    bool is_clear() const;
    bool is_occupied() const;
    bool is_unknown() const;

    void occupy();
    void clear();
    void timeout();

    void dump() const;
    void ledUpdate();
};

#endif /* PATH_H */
