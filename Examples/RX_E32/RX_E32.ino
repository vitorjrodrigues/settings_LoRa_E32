// Code for the Receiver

#include <SoftwareSerial.h>

#define BAUD 9600
#define M0 4       // Lora Pin 1
#define M1 5       // Lora Pin 2
#define LRX 11     // Lora Pin 3
#define LTX 10     // Lora Pin 4
#define AUX 6      // Lora Pin 5

SoftwareSerial lora(LTX, LRX);

uint8_t rd = 0;

void setup() {
  Serial.begin(BAUD);
  lora.begin(BAUD);
  
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);
  
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  Serial.println("Start");
}

void loop() 
{
  if (lora.available())
  {
    rd = lora.read();
    Serial.print("Li: "); Serial.println(rd, HEX);
  }
}
