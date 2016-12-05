//Sensor de luz

int ledred = 7; //Led no pino 7
int ledgreen = 8;
int ledblue = 9;
int ldrPin = 0; //LDR no pino analígico 0
int ldrValor = 0; //Valor lido do LDR

void setup() {
 pinMode(ledred,OUTPUT);
 pinMode(ledgreen,OUTPUT);
 pinMode(ledblue,OUTPUT);
 Serial.begin(9600); //Inicia a comunicação serial
}

void loop() {
 ///ler o valor do LDR
 ldrValor = analogRead(ldrPin); //O valor lido será entre 0 e 1023

 if(ldrValor<300){
   digitalWrite(ledred,HIGH);
   digitalWrite(ledgreen,HIGH);
   digitalWrite(ledblue,HIGH);
 }

 if(ldrValor>=300 && ldrValor<600){
   digitalWrite(ledred,HIGH);
   digitalWrite(ledgreen,LOW);
   digitalWrite(ledblue,LOW);
 }

 if (ldrValor >= 600 && ldrValor < 900){
   digitalWrite(ledgreen,HIGH);
   digitalWrite(ledred,LOW);
   digitalWrite(ledblue,LOW);
 }
  if (ldrValor >= 900){
   digitalWrite(ledgreen,LOW);
   digitalWrite(ledblue,HIGH);
   digitalWrite(ledred,LOW);
 }
 //imprime o valor lido do LDR no monitor serial
 Serial.println(ldrValor);
 delay(300);
}
