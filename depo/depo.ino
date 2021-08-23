#include <Bounce2.h>
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

struct ServoDef {
    String name;
    int pin;
    int anglePlus;
    int angleMinus;
};

struct HPDef {
    String name;
    int pin;
    int delay;
};

HPDef probe_defs[PROBE_COUNT] = {
    { "HSSV1", 25, 0 }, { "HS11", 27, 0 }, { "HS12", 26, 500 },  { "HS21", 29, 0 },
    { "HS22", 22, 500 },  { "HS31", 31, 0 }, { "HS32", 24, 500 }, { "HSSVJ", 33, 0 },
};

ServoDef stop_defs[SEMAPHORE_COUNT] = {
    { "SVJ", 12, 10, 90 }, { "S11", 3, 10, 90 },  { "S12", 4, 10, 90 },  { "S21", 8, 10, 90 },
    { "S22", 9, 20, 80 },  { "S31", 10, 10, 90 }, { "S32", 11, 10, 90 },
};

ServoDef junction_defs[JUNCTION_COUNT] = {
    { "V1", 7, 0, 40 },
    { "V2", 6, 0, 40 },
    { "V3", 5, 0, 40 },
};

/* -------------------------------------------------------------------------- */
// Function prototypes

void hallProbeOnOccupied(HallProbe *);
void outgoingCar();
void incomingCar();
void incomingCarGo(int stand);
void incomingCarCheck();
int freeStand();
void pathBtnChanged(VPath *);
void onCarFromStop(int stop);
void randomCarGo();
bool canCarGoOut();
VPath* standToPath(int stand);
Semaphore* standToSemaphore(int stand, int pos);
void moveCarsToPos2();
void moveCarToPos2(int stand);
size_t noFreePositions();
void circuitFree();

/* -------------------------------------------------------------------------- */
// Variables

constexpr unsigned long CAR_LEAVE_HALL_TIMEOUT = 2000;

Bounce *runButton;
int carLeftStand = -1;
unsigned long carLeftHallTimeout;

/* -------------------------------------------------------------------------- */

void setup()
{
    pinMode(28, INPUT_PULLUP);
    runButton = new Bounce(28, 5);

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name, probe_defs[i].delay);
        probes[i]->onOccupied = hallProbeOnOccupied;
    }

    paths[P_ENTRANCE] = new VPath(P_ENTRANCE, "entrance", VPathStatus::clear);
    paths[P_CIRCUIT] = new VPath(P_CIRCUIT, "circuit", VPathStatus::clear, 0, 0, true);
    paths[P_STAND11] = new VPath(P_STAND11, "stand11", VPathStatus::unknown, 48, 40);
    paths[P_STAND12] = new VPath(P_STAND12, "stand12", VPathStatus::unknown, 42, 2);
    paths[P_STAND21] = new VPath(P_STAND21, "stand21", VPathStatus::unknown, 43, 38);
    paths[P_STAND22] = new VPath(P_STAND22, "stand22", VPathStatus::unknown, 44, 32);
    paths[P_STAND31] = new VPath(P_STAND31, "stand31", VPathStatus::unknown, 47, 34);
    paths[P_STAND32] = new VPath(P_STAND32, "stand32", VPathStatus::unknown, 46, 30);

    for (int i = 0; i < PATHS_COUNT; i++) {
        paths[i]->onBtnChanged = pathBtnChanged;
    }

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        semaphores[i] = new Semaphore(stop_defs[i].name, stop_defs[i].pin,
                                      stop_defs[i].anglePlus, stop_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin,
                                    junction_defs[i].anglePlus, junction_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    Serial.begin(9600);
    log("Initialized Faller Depo!");
    randomCarGo();
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

    if (!paths[P_CIRCUIT]->is_clear()) {
        paths[P_CIRCUIT]->timeout();
    }
    runButton->update();
    if ((runButton->fell()) && (canCarGoOut())) {
        randomCarGo();
    }

    if ((carLeftStand > -1) && (millis() >= carLeftHallTimeout)) {
        log("Car did not actually left!");
        standToPath(carLeftStand, 2)->clear();
        moveCarToPos2(carLeftStand);
        carLeftStand = -1;
        circuitFree();
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

        if (paths[P_STAND12]->is_clear()) {
            paths[P_STAND12]->occupy();
            semaphores[S11]->signal_green();
            semaphores[S12]->signal_red();
        } else {
            paths[P_STAND11]->occupy();
            semaphores[S11]->signal_red();
        }

        circuitFree();
        break;

    case HS21:
        paths[P_CIRCUIT]->clear();

        if (paths[P_STAND22]->is_clear()) {
            paths[P_STAND22]->occupy();
            semaphores[S21]->signal_green();
            semaphores[S22]->signal_red();
        } else {
            paths[P_STAND21]->occupy();
            semaphores[S21]->signal_red();
        }

        circuitFree();
        break;

    case HS31:
        paths[P_CIRCUIT]->clear();

        if (paths[P_STAND32]->is_clear()) {
            paths[P_STAND32]->occupy();
            semaphores[S31]->signal_green();
            semaphores[S32]->signal_red();
        } else {
            paths[P_STAND31]->occupy();
            semaphores[S31]->signal_red();
        }

        circuitFree();
        break;

    /* VÝJEZDY ZE STANOVIŠŤ */
    case HS12:
        paths[P_STAND12]->clear();
        onCarFromStop(1);
        break;

    case HS22:
        paths[P_STAND22]->clear();
        onCarFromStop(2);
        break;

    case HS32:
        paths[P_STAND32]->clear();
        onCarFromStop(3);
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

void incomingCarCheck()
{
    int stand = freeStand();
    if ((paths[P_ENTRANCE]->is_occupied()) && (stand > 0) && (paths[P_CIRCUIT]->is_clear())) {
        incomingCarGo(stand);
    }
}

void outgoingCar()
{
    paths[P_CIRCUIT]->clear();
    circuitFree();
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

void pathBtnChanged(VPath *path)
{
    if ((path->id == P_STAND11) || (path->id == P_STAND21) || (path->id == P_STAND31)) {
        incomingCarCheck();
    }

    if (path->is_occupied()) {
        switch (path->id) {
        case P_STAND11:
            semaphores[S11]->signal_red();
            break;
        case P_STAND12:
            semaphores[S12]->signal_red();
            break;
        case P_STAND21:
            semaphores[S21]->signal_red();
            break;
        case P_STAND22:
            semaphores[S22]->signal_red();
            break;
        case P_STAND31:
            semaphores[S31]->signal_red();
            break;
        case P_STAND32:
            semaphores[S32]->signal_red();
            break;
        }
    }

    moveCarsToPos2();
}

void onCarFromStop(int stop)
{
    // assert paths[P_CIRCUIT]->is_clear()
    paths[P_CIRCUIT]->occupy();

    moveCarToPos2(stop);

    junctions[VH1]->to_plus();
    junctions[VH2]->to_plus();
    junctions[VH3]->to_plus();
}

VPath* standToPath(int stand, int pos) {
    switch (stand) {
    case 1: return (pos == 1) ? paths[P_STAND11] : paths[P_STAND12];
    case 2: return (pos == 1) ? paths[P_STAND21] : paths[P_STAND22];
    case 3: return (pos == 1) ? paths[P_STAND31] : paths[P_STAND32];
    default:
        log("Invalid stand!");
        return nullptr;
    }
}

Semaphore* standToSemaphore(int stand, int pos) {
    switch (stand) {
    case 1: return (pos == 1) ? semaphores[S11] : semaphores[S12];
    case 2: return (pos == 1) ? semaphores[S21] : semaphores[S22];
    case 3: return (pos == 1) ? semaphores[S31] : semaphores[S32];
    default:
        log("Invalid stand!");
        return nullptr;
    }
}

void randomCarGo()
{
    if ((paths[P_STAND12]->is_clear()) && (paths[P_STAND22]->is_clear()) &&
        (paths[P_STAND32]->is_clear()))
        return;

    // Pick a stand
    static int lastStand = 0;
    do {
        lastStand++;
        if (lastStand == 4)
            lastStand = 1;
    } while (standToPath(lastStand, 2)->is_clear());

    log("Going out with car " + String(lastStand));
    standToSemaphore(lastStand, 2)->signal_green();
    carLeftStand = lastStand;
    carLeftHallTimeout = millis() + CAR_LEAVE_HALL_TIMEOUT;
}

bool canCarGoOut()
{
    return (paths[P_CIRCUIT]->is_clear()) && (carLeftStand == -1);
}

void moveCarsToPos2()
{
    for (size_t stand = 1; stand < 3; stand++) {
        moveCarToPos2(stand);
    }
}

void moveCarToPos2(int stand)
{
    if (standToPath(stand, 1)->is_clear()) {
        standToSemaphore(stand, 1)->signal_red();
    } else {
        if (standToPath(stand, 2)->is_clear()) {
            standToPath(stand, 2)->setState(standToPath(stand, 1)->state());
            standToPath(stand, 1)->clear();
            standToSemaphore(stand, 1)->signal_green();
            standToSemaphore(stand, 2)->signal_red();
        }
    }
    incomingCarCheck();
}

size_t noFreePositions()
{
    size_t count = 0;
    for (size_t i = P_STAND11; i <= P_STAND32; i++) {
        if (paths[i]->is_clear())
            count++;
    }
    return count;
}

void circuitFree()
{
    incomingCarCheck();
    if ((paths[P_CIRCUIT]->is_clear()) && (noFreePositions() < 2)) {
        randomCarGo();
    }
}
