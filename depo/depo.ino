#include "blocks.h"
#include "hall_probe.h"
#include "junction.h"
#include "log.h"
#include "semaphore.h"

HallProbe *probes[PROBE_COUNT];
Semaphore *semaphores[SEMAPHORE_COUNT];
Junction *junctions[JUNCTION_COUNT];

struct IODef {
    String name;
    int pin;
};

IODef probe_defs[PROBE_COUNT] = {
    { "HSSV1", 25 }, { "HS11", 27 }, { "HS12", 0 },  { "HS21", 29 },
    { "HS22", 22 },  { "HS31", 31 }, { "HS32", 24 },
};

IODef stop_defs[SEMAPHORE_COUNT] = {
    { "SV1", 12 }, { "S11", 3 },  { "S12", 4 },  { "S21", 8 },
    { "S22", 9 },  { "S31", 10 }, { "S32", 11 },
};

IODef junction_defs[JUNCTION_COUNT] = {
    { "V1", 7 },
    { "V2", 6 },
    { "V3", 5 },
};

void setup()
{

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name);
    }

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name);
    }

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        semaphores[i] = new Semaphore(stop_defs[i].name, stop_defs[i].pin, 100, 10);
    }

    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin, 0, 30);
    }

    Serial.begin(9600);
    while (!Serial)
        ;
    log("Initialized Faller Depo!");
    dump_all();
}

void loop()
{

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i]->update();
    }
}
