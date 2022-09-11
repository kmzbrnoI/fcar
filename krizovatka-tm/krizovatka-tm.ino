#include "blocks.h"
#include "junction_btn_led.h"
#include "log.h"

Junction *junctions[JUNCTION_COUNT];

struct ServoBtnLedDef {
    String name;
    int pin;
    int anglePlus;
    int angleMinus;
    int pinLedPlus;
    int pinLedMinus;
    int pinBtn;
};

ServoBtnLedDef junction_defs[JUNCTION_COUNT] = {
    { "V1 Skuhrov", 10, 10, 90, 45, 43, 32 },
    { "V2 Brizky",  0, 0, 40, 0, 0, 0 },
    { "V3 Lesna",   0, 0, 40, 0, 0, 0 },
};

/* -------------------------------------------------------------------------- */

void setup()
{
    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i] = new Junction(junction_defs[i].name, junction_defs[i].pin,
                                    junction_defs[i].anglePlus, junction_defs[i].angleMinus,
                                    junction_defs[i].pinLedPlus, junction_defs[i].pinLedMinus,
                                    junction_defs[i].pinBtn);
        delay(100); // to avoid large current due to a lot of servos moving
    }

    junctions[0]->to_plus();
    junctions[1]->to_plus();
    junctions[2]->to_plus();

    Serial.begin(9600);
    log("Initialized Faller T-Cross manual");
}

void loop()
{
    for (int i = 0; i < JUNCTION_COUNT; i++) {
        junctions[i]->updateButton();
    }

    delay(1);
}
