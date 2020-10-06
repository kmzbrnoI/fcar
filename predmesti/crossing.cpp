#include "crossing.h"
#include "path.h"

void crossing_event(int id, const String& name) {

  extern VPath* paths[];
  extern Crossing* crossings[];

  if (id == CRG1) {
    crossings[id]->isRed() ? paths[FH02FM02]->red_crossing() : paths[FH02FM02]->green_crossing();
    //Serial.print("CRG1 isRed() = ");
    //Serial.println(crossings[id]->isRed());
  }
  if (id == CRG2) {
    crossings[id]->isRed() ? paths[FH02FM02]->red_crossing() : paths[FH02FM02]->green_crossing();
    //Serial.print("CRG2 isRed() = ");
    //Serial.println(crossings[id]->isRed());
  }
  if (id == CRH1) {
    crossings[id]->isRed() ? paths[FH08FM08]->red_crossing() : paths[FH08FM08]->green_crossing();
    //Serial.print("CRH1 isRed() = ");
    //Serial.println(crossings[id]->isRed());
  }
}

Crossing::Crossing(int id, int pinClosed, int pinOccupied, const String& name)
  :
  _id(id),
  _pinClosed(pinClosed),
  _pinOccupied(pinOccupied),
  _name(name),
  _isRed(false) {}

void Crossing::updateState() {
  _isRed = (digitalRead(_pinClosed) == HIGH) ? false : true;
  crossing_event(_id, _name);
}

bool Crossing::isRed() {
  return _isRed;
}
