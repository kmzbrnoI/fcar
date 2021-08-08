#include <Arduino.h>
#include "log.h"

void log(const String& message) {
  Serial.print(String(millis()) + ": ");
  Serial.print(message);
  Serial.print('\n');
}

void dump_all() {
  dump_probes();
}

void dump_probes() {
}
