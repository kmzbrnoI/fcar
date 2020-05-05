#define PROBE_COUNT 2

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;    // the number of the pushbutton pin

typedef struct {
  int pin;
  String name;
  int counter;

  int reading;
  int lastState;
  int state;
  unsigned long lastDebounceTime;
} Probe;

Probe probes[PROBE_COUNT];


// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers


void setup() {

  // TODO: default values could be set by init function
  probes[0].pin = 5;
  probes[0].name = "Brown";
  probes[0].counter = 0;
  probes[0].reading = 0;
  probes[0].lastState = LOW;
  probes[0].lastDebounceTime = 0;

  probes[1].pin = 6;
  probes[1].name = "Yellow";
  probes[1].counter = 0;
  probes[1].reading = 0;
  probes[1].lastState = LOW;
  probes[1].lastDebounceTime = 0;

  for (int i=0; i < PROBE_COUNT; i++) {
    pinMode(probes[i].pin, INPUT_PULLUP);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {

  for (int i=0; i < PROBE_COUNT; i++) {

    // read the state of the switch into a local variable:
    probes[i].reading = digitalRead(probes[i].pin);
  
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
  
    // If the switch changed, due to noise or pressing:
    if (probes[i].reading != probes[i].lastState) {
      // reset the debouncing timer
      probes[i].lastDebounceTime = millis();
    }
  
    if ((millis() - probes[i].lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
  
      // if the button state has changed:
      if (probes[i].reading != probes[i].state) {
        probes[i].state = probes[i].reading;
  
        // only toggle the LED if the new button state is HIGH
        if (probes[i].state == HIGH) {
          Serial.print("Hall probe ");
          Serial.print(probes[i].name);
          Serial.print(" [");
          Serial.print(probes[i].pin);
          Serial.print(" ]");
          Serial.print(probes[i].counter);
          Serial.print("\n");
          probes[i].counter++;
        }
      }
    }

    probes[i].lastState = probes[i].reading;
  }
}
