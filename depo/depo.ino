#include "blocks.h"
#include "hall_probe.h"
#include "junction.h"
#include "log.h"
#include "path.h"
#include "semaphore.h"

HallProbe *probes[PROBE_COUNT];
Semaphore *semaphores[SEMAPHORE_COUNT];
Junction *junctions[JUNCTION_COUNT];

VPath *path_entrance;
VPath *path_circuit;
VPath *path_stand11;
VPath *path_stand12;
VPath *path_stand21;
VPath *path_stand22;
VPath *path_stand31;
VPath *path_stand32;

struct IODef {
    String name;
    int pin;
};

IODef probe_defs[PROBE_COUNT] = {
    { "HSSV1", 25 }, { "HS11", 27 }, { "HS12", 0 },  { "HS21", 29 },
    { "HS22", 22 },  { "HS31", 31 }, { "HS32", 24 }, { "HSSVJ", 33 },
};

IODef stop_defs[SEMAPHORE_COUNT] = {
    { "SVJ", 12 }, { "S11", 3 },  { "S12", 4 },  { "S21", 8 },
    { "S22", 9 },  { "S31", 10 }, { "S32", 11 },
};

IODef junction_defs[JUNCTION_COUNT] = {
    { "V1", 7 },
    { "V2", 6 },
    { "V3", 5 },
};

/* -------------------------------------------------------------------------- */
// Function prototypes

void hallProbeOnOccupied(int id);
void incomingCar();

/* -------------------------------------------------------------------------- */

void setup()
{

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name);
        probes[i]->onOccupied = hallProbeOnOccupied;
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

    path_entrance = new VPath("entrance", VPathStatus::clear);
    path_circuit = new VPath("circuit", VPathStatus::clear, true);
    path_stand11 = new VPath("stand11", VPathStatus::unknown);
    path_stand12 = new VPath("stand12", VPathStatus::unknown);
    path_stand21 = new VPath("stand21", VPathStatus::unknown);
    path_stand22 = new VPath("stand22", VPathStatus::unknown);
    path_stand31 = new VPath("stand31", VPathStatus::unknown);
    path_stand32 = new VPath("stand32", VPathStatus::unknown);

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


void hallProbeOnOccupied(int id)
{
    /* VÝJEZD */
    if (id == HSSV1) {
        path_circuit->clear();
    }

    /* VJEZDY NA STANOVIŠTĚ */
    if (id == HS11) {
        path_circuit->clear();
        path_stand11->occupy();
    }
    if (id == HS21) {
        path_circuit->clear();
        path_stand21->occupy();
    }
    if (id == HS31) {
        path_circuit->clear();
        path_stand31->occupy();
    }

    /* VÝJEZDY ZE STANOVIŠŤ */
    if (id == HS12) {
        path_stand12->clear();
        path_circuit->occupy();
    }
    if (id == HS22) {
        path_stand22->clear();
        path_circuit->occupy();
    }
    if (id == HS32) {
        path_stand32->clear();
        path_circuit->occupy();
    }

    /* VJEZD */
    if (id == HSSVJ) {

        incomingCar();
    }
}

void incomingCar()
{
    path_entrance->occupy();
    // TODO
}
