// Definitions
#define AC_INPUT 2
#define TRIAC_PIN 3
#define LDR_PIN 0
#define AC_HALF_CYCLE_TIME 8333 // [us] Time spent by an AC wave to make a half cycle
#define MAX_DIMMING_LEVEL 1000 // Max dimming level
#define MIN_DIMMING_LEVEL 60 // Min dimming level
#define MAX_ADC 1023 // Max ADC value

// Global variables
float lightlevel = MIN_DIMMING_LEVEL; // MIN_DIMMING_LEVEL = OFF, MAX_DIMMING_LEVEL = ON
float cur_lightlevel = lightlevel;
float opt_lightlevel = lightlevel;

// Prototypes
void interruptRoutine();
void triggerTheTriac();
void increaseLight();
void decreaseLight();
float ADCToLight();
void updateCurLightLevel();
float serialToLight();
void updateOptLightLevel();

// Configure the triac and serial ports
void setup() {
  pinMode(TRIAC_PIN, OUTPUT);
  attachInterrupt(0, interruptRoutine, RISING);
  Serial.begin(9600);
}

// Update the load lighting via serial port
void loop() {
  updateCurLightLevel();
  updateOptLightLevel();

  if(cur_lightlevel - opt_lightlevel < -10) {
    increaseLight();
  } else if(cur_lightlevel - opt_lightlevel > 10) {
    decreaseLight();
  } else {
    // the light level is just ok.
  }

  delayMicroseconds(1000);
}

// This routine is called whenever the AC signal passes through zero.
// It consists in waiting an attenuation time and then triggering the triac, in other words, turning the load on.
void interruptRoutine() {
  // Dimming time per dimming level unit: AC_HALF_CYCLE_TIME / MAX_DIMMING_LEVEL [us]
  float dimmingtime = ((MAX_DIMMING_LEVEL + 60 - lightlevel) * AC_HALF_CYCLE_TIME) / (MAX_DIMMING_LEVEL + 60);
  delayMicroseconds(dimmingtime); // Off cycle
  triggerTheTriac();
}
// Turn the load on
void triggerTheTriac() {
  digitalWrite(TRIAC_PIN, HIGH);
  delayMicroseconds(AC_HALF_CYCLE_TIME / 1000.0);
  digitalWrite(TRIAC_PIN, LOW);
}

// Increase a bit the load lighting
void increaseLight() {
  if(lightlevel < MAX_DIMMING_LEVEL)
    lightlevel++;
}
// Decrease a bit the load lighting
void decreaseLight() {
  if(lightlevel > MIN_DIMMING_LEVEL)
    lightlevel--;
}

// Convert value read from ADC to a light level
float ADCToLight(int adc) {
  return (1.0 * (MAX_ADC - adc) * (MAX_DIMMING_LEVEL - MIN_DIMMING_LEVEL)) / MAX_ADC + MIN_DIMMING_LEVEL;
}
// Update LDR level
void updateCurLightLevel() {
  int adc = analogRead(LDR_PIN); // value in range [0,1023]
  cur_lightlevel = ADCToLight(adc);
}

// Convert value read from ADC to a light level
float serialToLight(char rx) {
  if(rx >= 'a' && rx <= 'z') {
    float ans = rx - 'a';
    char max_rx = 'z' - 'a';
    return (ans * MAX_DIMMING_LEVEL) / max_rx;
  } else {
    return opt_lightlevel;
  }
}
// Update Optimal Light Level chosen
void updateOptLightLevel() {
  while(Serial.available() > 0) {
    char rx = Serial.read();
    opt_lightlevel = serialToLight(rx);
  }
}
