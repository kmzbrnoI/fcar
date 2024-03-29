#include "blocks.h"
#include "hall_probe.h"
#include "junction.h"
#include "log.h"
#include "path.h"
#include "semaphore.h"
#include <Bounce2.h>

HallProbe *probes[PROBE_COUNT];
Semaphore *semaphores[SEMAPHORE_COUNT];
Junction *junctions[JUNCTION_COUNT];
VPath *paths[PATHS_COUNT];

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
    { "HSSV1", 25, 0 },  { "HS11", 27, 0 }, { "HS12", 26, 500 }, { "HS21", 29, 0 },
    { "HS22", 22, 500 }, { "HS31", 31, 0 }, { "HS32", 24, 500 }, { "HSSVJ", 33, 0 },
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
VPath *standToPath(int stand);
Semaphore *standToSemaphore(int stand, int pos);
void moveCarsToPos2();
void moveCarToPos2(int stand);
size_t noFreePositions();
void circuitFree();
void timeLeavingLedUpdate();
void setNextRunTime();
bool crossingFree();

/* -------------------------------------------------------------------------- */
// Variables

constexpr unsigned long CAR_LEAVE_HALL_TIMEOUT = 3000;
constexpr int TIME_LEAVING_LED = 45;
constexpr int TIME_LEAVING_BUTTON = 36;
constexpr int LEAVE_BUTTON = 28;
constexpr int CROSSING_PIN = 53;
constexpr int CROSSING_INDICATION_PIN = 50;
constexpr int CIRCUIT_INDICATION_PIN = 49;

constexpr unsigned long LEAVE_PERIOD_SEC_MIN = 60;
constexpr unsigned long LEAVE_PERIOD_SEC_MAX = 100;

Bounce *runButton;
Bounce *timeLeavingButton;
int carLeftStand = -1;
unsigned long carLeftHallTimeout;
unsigned long nextRunTime = 30;
bool timeLeavingActive = true;
bool initLeaving = true;

/* -------------------------------------------------------------------------- */

void setup()
{
    setNextRunTime();

    pinMode(LEAVE_BUTTON, INPUT_PULLUP);
    runButton = new Bounce(LEAVE_BUTTON, 5);
    pinMode(TIME_LEAVING_BUTTON, INPUT_PULLUP);
    timeLeavingButton = new Bounce(TIME_LEAVING_BUTTON, 5);
    pinMode(TIME_LEAVING_LED, OUTPUT);
    pinMode(CROSSING_PIN, INPUT_PULLUP);
    pinMode(CROSSING_INDICATION_PIN, OUTPUT);
    pinMode(CIRCUIT_INDICATION_PIN, OUTPUT);

    for (int i = 0; i < PROBE_COUNT; i++) {
        probes[i] = new HallProbe(i, probe_defs[i].pin, probe_defs[i].name, probe_defs[i].delay);
        probes[i]->onOccupied = hallProbeOnOccupied;
    }

    paths[P_ENTRANCE] = new VPath(P_ENTRANCE, "entrance", VPathStatus::clear);
    paths[P_CIRCUIT] = new VPath(P_CIRCUIT, "circuit", VPathStatus::clear, 0, 0);
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
        semaphores[i] = new Semaphore(stop_defs[i].name, stop_defs[i].pin, stop_defs[i].anglePlus,
                                      stop_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin,
                                    junction_defs[i].anglePlus, junction_defs[i].angleMinus);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    Serial.begin(9600);
    log("Initialized Faller Depo!");
    initLeaving = true;
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
        timeLeavingLedUpdate();
    }

    if (paths[P_CIRCUIT]->is_occupied()) {
        if ((millis() - paths[P_CIRCUIT]->_occupiedTime) >= PATH_TIMEOUT) {
            log("Circuit timeout!");
            paths[P_CIRCUIT]->clear();
            circuitFree();
        }
    }

    runButton->update();
    if ((runButton->fell()) && (canCarGoOut())) {
        initLeaving = false;
        randomCarGo();
    }

    timeLeavingButton->update();
    if (timeLeavingButton->fell()) {
        timeLeavingActive = !timeLeavingActive;
        if (timeLeavingActive) {
            setNextRunTime();
        } else {
            digitalWrite(TIME_LEAVING_LED, LOW);
        }
    }

    if ((carLeftStand > -1) && (millis() >= carLeftHallTimeout)) {
        log("Car did not actually leave!");
        standToPath(carLeftStand, 2)->clear();
        moveCarToPos2(carLeftStand);
        carLeftStand = -1;
        circuitFree();
        if ((paths[P_CIRCUIT]->is_clear()) && (!initLeaving) && (crossingFree())) {
            randomCarGo();
        }
    }

    // Call circuitFree event caused by crossing got free
    static bool crossingFreeOld = true;
    if ((!crossingFreeOld) && (crossingFree())) {
        if (paths[P_CIRCUIT]->is_clear()) {
            circuitFree();
        }
    }
    crossingFreeOld = crossingFree();

    if ((timeLeavingActive) && ((millis() / 1000) >= nextRunTime) && (paths[P_CIRCUIT]->is_clear())
        && (crossingFree())) {
        log("Time elapsed, going out with car...");
        initLeaving = false;
        randomCarGo();
    }

    digitalWrite(CROSSING_INDICATION_PIN, digitalRead(CROSSING_PIN) == LOW);
    digitalWrite(CIRCUIT_INDICATION_PIN, !paths[P_CIRCUIT]->is_clear());

    if (millis() % 10000 < 2)
        log("");

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
    paths[P_ENTRANCE]->clear();
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
    carLeftStand = -1;
    moveCarToPos2(stop);

    junctions[VH1]->to_plus();
    junctions[VH2]->to_plus();
    junctions[VH3]->to_plus();
}

VPath *standToPath(int stand, int pos)
{
    switch (stand) {
    case 1:
        return (pos == 1) ? paths[P_STAND11] : paths[P_STAND12];
    case 2:
        return (pos == 1) ? paths[P_STAND21] : paths[P_STAND22];
    case 3:
        return (pos == 1) ? paths[P_STAND31] : paths[P_STAND32];
    default:
        log("Invalid stand!");
        return nullptr;
    }
}

Semaphore *standToSemaphore(int stand, int pos)
{
    switch (stand) {
    case 1:
        return (pos == 1) ? semaphores[S11] : semaphores[S12];
    case 2:
        return (pos == 1) ? semaphores[S21] : semaphores[S22];
    case 3:
        return (pos == 1) ? semaphores[S31] : semaphores[S32];
    default:
        log("Invalid stand!");
        return nullptr;
    }
}

void randomCarGo()
{
    setNextRunTime();

    if (!crossingFree())
        return;
    if (!paths[P_CIRCUIT]->is_clear())
        return;
    if ((paths[P_STAND12]->is_clear()) && (paths[P_STAND22]->is_clear())
        && (paths[P_STAND32]->is_clear()))
        return;

    // Pick a stand
    static int lastDoubleStand = -1;
    do {
        lastDoubleStand++;
        if (lastDoubleStand == 6)
            lastDoubleStand = 0;
    } while (standToPath((lastDoubleStand / 2) + 1, 2)->is_clear());
    int lastStand = (lastDoubleStand / 2) + 1;

    log("Going out with car " + String(lastStand));
    standToSemaphore(lastStand, 2)->signal_green();
    carLeftStand = lastStand;
    carLeftHallTimeout = millis() + CAR_LEAVE_HALL_TIMEOUT;
}

bool canCarGoOut() { return (paths[P_CIRCUIT]->is_clear()) && (carLeftStand == -1); }

void moveCarsToPos2()
{
    for (size_t stand = 1; stand <= 3; stand++) {
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
    if ((paths[P_CIRCUIT]->is_clear()) && (noFreePositions() < 2) && (crossingFree())) {
        randomCarGo();
    }
}

void timeLeavingLedUpdate()
{
    static bool lastState = true;
    if (timeLeavingActive) {
        lastState = !lastState;
        digitalWrite(TIME_LEAVING_LED, lastState);
    }
}

void setNextRunTime()
{
    int sec = random(LEAVE_PERIOD_SEC_MIN, LEAVE_PERIOD_SEC_MAX);
    nextRunTime = (millis() / 1000) + sec;
    log("Next run time in " + String(sec) + " sec: " + String(nextRunTime));
}

bool crossingFree() { return digitalRead(CROSSING_PIN) == HIGH; }
