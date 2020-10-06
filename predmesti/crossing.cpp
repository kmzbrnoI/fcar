#include "crossing.h"
#include "path.h"

Crossing::Crossing(int id, int pinClosed, int pinOccupied, const String& name)
  : _id(id), _name(name), _bClosed(pinClosed, 50), _bOccupied(pinOccupied, 50) {
  pinMode(pinClosed, INPUT_PULLUP);
  pinMode(pinOccupied, INPUT_PULLUP);
}

void Crossing::changed() {
  // You can propagate event here if you want
}

void Crossing::updateState() {
  _bClosed.update();
  _bOccupied.update();

  if (_bClosed.fell() || _bClosed.rose() || _bOccupied.fell() || _bOccupied.rose())
    this->changed();
}

bool Crossing::isRed() {
  return (_bClosed.read() == LOW) || (_bOccupied.read() == LOW);
}
