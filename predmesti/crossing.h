#ifndef CROSSING_H
#define CROSSING_H

#include "Arduino.h"


class Crossing
{
  public:
     /* Constructor */
    Crossing(int id, int pin, const char* name);

    /* Update state from probe */
    void updateState();
    bool isRed();  

  private:
    const int _id; 
    const int _pin;
    const char* _name;

    bool _isRed;
};

#endif /* CROSSING_H */
