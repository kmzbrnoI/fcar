#ifndef CROSSING_H
#define CROSSING_H

#include "Arduino.h"


class Crossing
{
  public:
     /* Constructor */
    Crossing(int id, int pinClosed, int pinOccupied, const String& name);

    /* Update state from probe */
    void updateState();
    bool isRed();

  private:
    const int _id;
    const int _pinClosed;
    const int _pinOccupied;
    const String _name;

    bool _isRed;
};

#endif /* CROSSING_H */
