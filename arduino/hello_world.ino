int LED = 10;
int rx = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    rx = Serial.read();
    if(rx >= 105)
      digitalWrite(LED, HIGH);
    else
      digitalWrite(LED, LOW);
  }
}
