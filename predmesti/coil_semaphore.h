#ifndef COIL_SEMAPHORE_H
#define COIL_SEMAPHORE_H

////////////////////////////////////////////////////////////////////////////////
// Zastavovaci magnet ovladany civkou
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#include "blocks.h"
#include "path.h"
#include "switch_junction.h"
#include "vehicle.h"


/* Constant for signals */
enum class SSignal { stop = 0, go = 1 };

/* Semaphore logic */
struct CoilSemaphore {
  const int id;
  const String name;
  const int _pin;
  SSignal state = SSignal::stop;

  CoilSemaphore(int id, const String& name, int pin);

  void make_decision();
  void go();
  void stop();
  void dump() const;
};
#endif /* COIL_SEMAPHORE_H */
