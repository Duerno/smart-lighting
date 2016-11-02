// Definitions
#define AC_INPUT 2
#define TRIAC_PIN 3
#define AC_HALF_CYCLE_TIME 8333 // [us] Time spent by an AC wave to make a half cycle
#define MAX_DIMMING_LEVEL 128 // Max dimming level

// Global variables
int dimminglevel = 125; // Dimming level (0-128) 0 = ON, 128 = OFF

// Prototypes
void interruptRoutine();
void triggerTheTriac();

// Configure the triac and serial ports
void setup() {
  pinMode(TRIAC_PIN, OUTPUT);
  attachInterrupt(0, interruptRoutine, RISING);
  Serial.begin(9600);
}

// Update the load lighting via serial port
void loop() {
  if (Serial.available() > 0) {
    char rx = Serial.read();
    if(rx >= 'a' && rx <= 'z') {
      rx = 25 - (rx - 'a'); // rx: 0-25 corresponding to dimminglevel: 5-120
      dimminglevel = rx * 120.0 / 25 + 5;
    }
  }
}

// This routine is called whenever the AC signal passes through zero.
// It consists in waiting an attenuation time and then triggering the triac, in other words, turning the load on.
void interruptRoutine() {
  // Dimming time per dimming level unit: AC_HALF_CYCLE_TIME / MAX_DIMMING_LEVEL [us]
  float dimmingtime = (1.0 * dimminglevel * AC_HALF_CYCLE_TIME) / MAX_DIMMING_LEVEL;
  delayMicroseconds(dimmingtime); // Off cycle
  triggerTheTriac();
}

// Turn the load on
void triggerTheTriac() {
  digitalWrite(TRIAC_PIN, HIGH);
  delayMicroseconds(AC_HALF_CYCLE_TIME/1000.0);
  digitalWrite(TRIAC_PIN, LOW);
}
