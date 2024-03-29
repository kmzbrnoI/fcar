#ifndef PATH_H
#define PATH_H

#include <Arduino.h>
#include <Bounce2.h>

enum class VPathStatus { clear = 0, occupied = 1 };

struct VPath {
    int id;
    const String name;
    VPathStatus _state;

    unsigned long _occupiedTime = 0;

    VPath(int id, const String &name, VPathStatus state);

    bool is_clear() const;
    bool is_occupied() const;

    void occupy();
    void clear();
    void setState(VPathStatus);
    VPathStatus state() const;
    void update();

    void (*onOccupied)(VPath *) = nullptr;

    void dump() const;
};

#endif /* PATH_H */
