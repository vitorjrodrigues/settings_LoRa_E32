// Code for the Transmitter

#include <SoftwareSerial.h>

#define BAUD  9600
#define M0    4   //LoRa pin1
#define M1    5   //LoRa pin2
#define LRX   11  //LoRa pin3
#define LTX   10  //LoRa pin4
#define AUX   6   //LoRa pin5

SoftwareSerial lora(LTX, LRX);

uint8_t wr = 0;

void setup()
{
  Serial.begin(BAUD);
  lora.begin(BAUD);
  
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);
  
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  Serial.println("Sender Ready");
}

void loop()
{
  if (wr > 255)
  {
    wr = 0;
  }
  wr++;
  lora.write(wr);
  Serial.print("Sent: "); Serial.println(wr, HEX);
  delay(1000);
}
