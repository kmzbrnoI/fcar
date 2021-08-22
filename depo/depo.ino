#include "blocks.h"
#include "hall_probe.h"
#include "junction.h"
#include "log.h"
#include "path.h"
#include "semaphore.h"

HallProbe *probes[PROBE_COUNT];
Semaphore *semaphores[SEMAPHORE_COUNT];
Junction *junctions[JUNCTION_COUNT];
VPath *paths[PATHS_COUNT];

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

void hallProbeOnOccupied(HallProbe *);
void outgoingCar();
void incomingCar();
void incomingCarGo(int stand);
int freeStand();

/* -------------------------------------------------------------------------- */

void setup()
{
    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name);
        probes[i]->onOccupied = hallProbeOnOccupied;
    }

    paths[P_ENTRANCE] = new VPath("entrance", VPathStatus::clear);
    paths[P_CIRCUIT] = new VPath("circuit", VPathStatus::clear, 0, 0, true);
    paths[P_STAND11] = new VPath("stand11", VPathStatus::unknown, 48, 40);
    paths[P_STAND12] = new VPath("stand12", VPathStatus::unknown, 42, 2);
    paths[P_STAND21] = new VPath("stand21", VPathStatus::unknown, 43, 38);
    paths[P_STAND22] = new VPath("stand22", VPathStatus::unknown, 44, 32);
    paths[P_STAND31] = new VPath("stand31", VPathStatus::unknown, 47, 34);
    paths[P_STAND32] = new VPath("stand32", VPathStatus::unknown, 46, 30);

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        semaphores[i] = new Semaphore(stop_defs[i].name, stop_defs[i].pin, 10, 90);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin, 0, 30);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    Serial.begin(9600);
    log("Initialized Faller Depo!");
}

void loop()
{
    static int ledCounter = 0;
    constexpr int LED_PERIOD = 200;

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i]->update();
    }

    for (int i = 0; i < PATHS_COUNT; i++) {
        paths[i]->btnUpdate();
    }

    ledCounter++;
    if (ledCounter == LED_PERIOD) {
        ledCounter = 0;
        for (int i = 0; i < PATHS_COUNT; i++) {
            paths[i]->ledUpdate();
        }
    }

    delay(1);
}

void hallProbeOnOccupied(HallProbe *hp)
{
    log("Occupied: " + hp->name);

    switch (hp->id) {
    /* VÝJEZD */
    case HSSV1:
        outgoingCar();
        break;
    /* VJEZDY NA STANOVIŠTĚ */
    case HS11:
        paths[P_CIRCUIT]->clear();
        paths[P_STAND11]->occupy();
        break;
    case HS21:
        paths[P_CIRCUIT]->clear();
        paths[P_STAND21]->occupy();
        break;
    case HS31:
        paths[P_CIRCUIT]->clear();
        paths[P_STAND31]->occupy();
        break;
    /* VÝJEZDY ZE STANOVIŠŤ */
    case HS12:
        paths[P_STAND12]->clear();
        paths[P_CIRCUIT]->occupy();
        break;
    case HS22:
        paths[P_STAND22]->clear();
        paths[P_CIRCUIT]->occupy();
        break;
    case HS32:
        paths[P_STAND32]->clear();
        paths[P_CIRCUIT]->occupy();
        break;
    /* VJEZD */
    case HSSVJ:
        incomingCar();
        break;
    }
}

void incomingCar()
{
    // TODO: make car go if stand in manully freed (by button)
    int stand = freeStand();
    if ((paths[P_CIRCUIT]->is_clear()) && (stand > 0)) {
        incomingCarGo(stand);
    } else {
        paths[P_ENTRANCE]->occupy();
        semaphores[SVJ]->signal_red();
    }
}

void outgoingCar()
{
    paths[P_CIRCUIT]->clear();

    int stand = freeStand();
    if ((paths[P_ENTRANCE]->is_occupied()) && (stand > 0)) {
        incomingCarGo(stand);
    }
}

void incomingCarGo(int stand)
{
    // assert stand > 0
    log("Incoming car going to " + String(stand));
    paths[P_CIRCUIT]->occupy();
    semaphores[SVJ]->signal_green();

    switch (stand) {
    case 1:
        junctions[VH1]->to_minus();
        break;
    case 2:
        junctions[VH1]->to_plus();
        junctions[VH2]->to_minus();
        break;
    case 3:
        junctions[VH1]->to_plus();
        junctions[VH2]->to_plus();
        junctions[VH3]->to_minus();
        break;
    }
}

int freeStand()
{
    if (paths[P_STAND31]->is_clear())
        return 3;
    if (paths[P_STAND21]->is_clear())
        return 2;
    if (paths[P_STAND11]->is_clear())
        return 1;
    return 0;
}
