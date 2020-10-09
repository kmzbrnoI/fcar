#include <Arduino.h>
#include "log.h"
#include "path.h"

void log(const String& message) {
  Serial.print(String(millis()) + ": ");
  Serial.print(message);
  Serial.print('\n');
}

void dump_all() {
  dump_probes();
  dump_magnets();
  dump_vehicles();
  dump_junctions();
  dump_paths();
}

void dump_probes() {
}

void dump_magnets() {
}

void dump_vehicles() {
}

void dump_junctions() {
}

void dump_paths() {
  extern VPath* paths[];
  for (size_t i = 0; i < PATH_COUNT; i++)
    paths[i]->dump();
}
