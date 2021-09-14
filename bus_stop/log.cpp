#include "log.h"
#include <Arduino.h>

void log(const String &message)
{
    Serial.print(String(millis()) + ": ");
    Serial.print(message);
    Serial.print('\n');
}
