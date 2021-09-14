#include "blocks.h"
#include "hall_probe.h"
#include "junction.h"
#include "log.h"
#include "path.h"
#include "semaphore.h"

/* All hardware blocks connected to Arduino */

HallProbe *probes[PROBE_COUNT];
Semaphore *semaphores[SEMAPHORE_COUNT];
Junction *junctions[JUNCTION_COUNT];
VPath *paths[PATHS_COUNT];

/* Time constants describing the situation */
const int SWITCH_PASSAGE_TIME = 3000; // projeti vyhybkou
const int TOTAL_PASSAGE_TIME = 12000; // projeti celym usekem zastavky
const int CAR_INTERVAL = 30000; // auticka distribujeme v tomto intervalu

unsigned long last_car_A;
unsigned long last_car_B;

bool is_car_A_bus;
bool is_car_B_bus;

struct HPDef {
    String name;
    int pin;
    int delay;
};

struct ServoDef {
    String name;
    int pin;
    int anglePlus;
    int angleMinus;
};

HPDef probe_defs[PROBE_COUNT] = {
    { "HDA", 5, 0 },
    { "HTA", 4, 0 },
    { "HDB", 3, 0 },
    { "HTB", 2, 0 },
};

ServoDef stop_defs[SEMAPHORE_COUNT] = {
    { "SEMA", 11, 0, 70 },
    { "SEMB", 10, 0, 80 },
};

ServoDef junction_defs[JUNCTION_COUNT] = {
    { "JUNCTA", 9, 55, 20 },
    { "JUNCTB", 6, 45, 5 },
};

void hallProbeOnOccupied(HallProbe *);
void manage_bus_stop(Junction *junction, Semaphore *semaphore, VPath *bay, VPath *direct,
                     unsigned long last_car, bool is_bus);

void setup()
{
    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name, probe_defs[i].delay);
        probes[i]->onOccupied = hallProbeOnOccupied;
    }

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        semaphores[i] = new Semaphore(stop_defs[i].name, stop_defs[i].pin, stop_defs[i].anglePlus,
                                      stop_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin,
                                    junction_defs[i].anglePlus, junction_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    paths[P_DIRECT_A] = new VPath(P_DIRECT_A, "direct_a", VPathStatus::clear);
    paths[P_DIRECT_B] = new VPath(P_DIRECT_B, "direct_b", VPathStatus::clear);
    paths[P_BAY_A] = new VPath(P_BAY_A, "bay_a", VPathStatus::clear);
    paths[P_BAY_B] = new VPath(P_BAY_B, "bay_b", VPathStatus::clear);

    last_car_A = 0;
    last_car_B = 0;

    is_car_A_bus = false;
    is_car_B_bus = false;

    Serial.begin(9600);
    log("Initialized Bus Stop!");
}

void loop()
{
    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i]->update();
    }
}

void hallProbeOnOccupied(HallProbe *hp)
{
    log("Occupied: " + hp->name);

    switch (hp->id) {
    case HDA:
        manage_bus_stop(junctions[JUNCTA], semaphores[SEMA], paths[P_BAY_A], paths[P_DIRECT_A],
                        last_car_A, is_car_A_bus);
        break;
    case HTA:
        is_car_A_bus = true;
        break;
    case HDB:
        manage_bus_stop(junctions[JUNCTB], semaphores[SEMB], paths[P_BAY_B], paths[P_DIRECT_B],
                        last_car_B, is_car_B_bus);
        break;
    case HTB:
        is_car_B_bus = true;
        break;
    }
}

void manage_bus_stop(Junction *junction, Semaphore *semaphore, VPath *bay, VPath *direct,
                     unsigned long last_car, bool is_bus)
{
    unsigned long interval = millis() - last_car;

    if (CAR_INTERVAL < interval) {
        if (bay->is_clear() && is_bus) {
            junction->to_minus();
            semaphore->signal_red();
            bay->occupy();
        } else {
            junction->to_plus();
            direct->occupy();
            last_car = millis();
        }
    } else {
        if (bay->is_clear()) {
            junction->to_minus();
            semaphore->signal_red();
            bay->occupy();
        } else {
            junction->to_plus();
            direct->occupy();
            last_car = millis();
        }
    }

    is_bus = false;
}
