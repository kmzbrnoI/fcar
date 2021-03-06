#ifndef CROSSING_H
#define CROSSING_H

#include <Bounce2.h>
#include "Arduino.h"

class Crossing
{
  public:
    Crossing(int id, int pinClosed, int pinOccupied, const String& name);

    void updateState();
    bool isRed();

  private:
    const int _id;
    const String _name;
    Bounce _bClosed;
    Bounce _bOccupied;

    void changed();
};

#endif /* CROSSING_H */
