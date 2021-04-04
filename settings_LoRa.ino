// Original by Emmanuel Leite de Medeiros
// Modified by Vítor José Costa Rodrigues (30-Jun-2019)
// Last Update: 17-Jan-2020.

#include <SoftwareSerial.h>

#define M0 4
#define M1 5
#define AUX 6

#include "param.h"

SoftwareSerial Slora(10, 11); // TX (pin4), RX (pin3)

void setup() {
  pinMode(AUX, INPUT); // AUX High (1) means it's idle (may be false), AUX LOW (0) means it's busy (always)
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  Serial.begin(9600);
  Slora.begin(9600);
  begin_instructions();
}

void loop() {
  char buff[16];
  while (Serial.available() > 0) {
    size_t i = Serial.readBytesUntil('\n', buff, 16);
    buff[i] = '\0';
    Serial.println(buff);
    while (1) {
      if (buff[0] != '0' || buff[1] != 'x' || buff[2] != 'C' || buff[3] < 48 || buff[3] > 52) {
        Serial.println("Invalid Command!! Try a valid one.");
        break;
      }
      else {
        switch (buff[3] - 48) {
          case 0:
            Serial.println("SELECTED: Set and Save Parameters");
            param_settings_save();
            break;
          case 1:
            Serial.println("SELECTED: Read Parameters Currently Set");
            param_read_config();
            break;
          case 2:
            Serial.println("SELECTED: Set Parameters (Don't Save)");
            param_settings_not_save();
            break;
          case 3:
            Serial.println("SELECTED: Read Module Version");
            param_read_version();
            break;
          case 4:
            Serial.println("SELECTED: Reset Module");
            param_reset();
            break;
          default:
            continue;
        }
        break;
      }
    }
  }
}

void param_read_version() {
  char buf[16];
  char buf2[16];
  int i = 0;
  Slora.write(LORA_VERSION, 3);
  delay(10);
  while (Slora.available()) {
    buf[i] = Slora.read();
    i++;
  }
  sprintf(buf2, "C3|%02X|%02X|%02X", buf[1], buf[2], buf[3]);
  //Serial.println(buf2);
}

void param_read_config() {
  int i = 0;
  char buf[16];
  char buf2[16];
  Slora.write(LORA_READ_PARAM, 3);
  Slora.flush();
  Slora.write(LORA_READ_PARAM, 3);
  while (Slora.available()) {
    buf[i] = Slora.read();
    i++;
  }
  //Slora.flush();
  Serial.print("Read = ");
  sprintf(buf2, "%02X|%02X|%02X|%02X|%02X|%02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
  Serial.println(buf2);
  delay(1000);
}

void param_reset() {
  Serial.println("Resetting...");
  Slora.write(LORA_RESET, 3);
  while (!digitalRead(AUX))
  {
    //Serial.print("AUX LOW ");
  }
  Serial.println("Resetted.");
}

void param_settings_save() {
  setSPED();
  uint8_t COM[6] = {HEAD_SAVE, ADDH, ADDL, SPED, CHAN, OPTION};
  Slora.write(COM, 6);
  delay(1000);
  Serial.flush();
}

void param_settings_not_save() {
  setSPED();
  uint8_t COM[6] = {HEAD_NOT_SAVE, ADDH, ADDL, SPED, CHAN, OPTION};
  Slora.write(COM, 6);
  delay(50);
  delay(1000);
  Serial.flush();
}

void begin_instructions() {
  Serial.println("Welcome!\nThis is the LoRa Module Programmer\nModel: EByte UART LoRa Module 100mW (E32-TTL-100)");
  Serial.println("\nInsert your command using the chart below: \n(WARNING: It is case sensitive)");
  Serial.println("0xC0 - Set and Save Parameters;\n0xC1 - Read Parameters Currently Set;");
  Serial.println("0xC2 - Set Parameters (Don't Save);\n0xC3 - Read Module Version;\n0xC4 - Reset Module.\n");
}

void setADDR() {
  // Set ADDR
  Serial.println("ADDR: 0x0000 (0/Default), 0x00A0 (1) or 0x00B0 (2).");
  while (1) {
    switch (Serial.read() - 48) {
      case 0:
        Serial.println("Selected: Address 0x0000");
        ADDL = 0x00; //Default
        Serial.flush();
        break;
      case 1:
        Serial.println("Selected: Address 0x00A0");
        ADDL = 0xA0;
        Serial.flush();
        break;
      case 2:
        Serial.println("Selected: Address 0x00B0");
        ADDL = 0xB0;
        Serial.flush();
        break;
      default:
        continue;
    }
    break;
  }
}

void setSPED() {
  // Set Parity
  Serial.println("Parity: 8N1 (0/Default), 8O1 (1) or 8E1 (2).");
  while (1) {
    switch (Serial.read() - 48) {
      case 0:
        Serial.println("Selected: 8N1 Parity (0b00000000)");
        SPED = PAR_8N1; //Default
        Serial.flush();
        break;
      case 1:
        Serial.println("Selected: 8O1 Parity (0b01000000)");
        SPED = PAR_8O1;
        Serial.flush();
        break;
      case 2:
        Serial.println("Selected: 8E1 Parity (0b10000000)");
        SPED = PAR_8E1;
        Serial.flush();
        break;
      default:
        continue;
    }
    break;
  }

  // Set Baud Rate
  Serial.println("Baud Rate: 1200 (0), 2400 (1), 4800 (2), 9600 (3/Default), 19200 (4), 38400 (5), 57600 (6) or 115200 (7).");
  while (1) {
    switch (Serial.read()) {
      case '0':
        Serial.println("Selected: 1200 baud (0b00000000)");
        SPED += BAUD_1200;
        break;
      case '1':
        Serial.println("Selected: 2400 baud (0b00001000)");
        SPED += BAUD_2400;
        break;
      case '2':
        Serial.println("Selected: 4800 baud (0b00010000)");
        SPED += BAUD_4800;
        break;
      case '3':
        Serial.println("Selected: 9600 baud (0b00011000)");
        SPED += BAUD_9600; //Default
        break;
      case '4':
        Serial.println("Selected: 19200 baud (0b00100000)");
        SPED += BAUD_19200;
        break;
      case '5':
        Serial.println("Selected: 38400 baud (0b00101000)");
        SPED += BAUD_38400;
        break;
      case '6':
        Serial.println("Selected: 57600 baud (0b00110000)");
        SPED += BAUD_57600;
        break;
      case '7':
        Serial.println("Selected: 115200 baud (0b00111000)");
        SPED += BAUD_115200; // Unstable??
        break;
      default:
        continue;
    }
    break;
  }

  // Set Air Data Rate
  Serial.println("Air Data Rate: 300 (0), 1200 (1), 2400 (2/Default), 4800 (3), 9600 (4) or 19200 (5).");
  while (1) {
    switch (Serial.read()) {
      case '0':
        Serial.println("Selected: 300 bps (0b00000000)");
        SPED += BPS_300;
        break;
      case '1':
        Serial.println("Selected: 1200 bps (0b00000001)");
        SPED += BPS_1200;
        break;
      case '2':
        Serial.println("Selected: 2400 bps (0b00000010)");
        SPED += BPS_2400; //Default
        break;
      case '3':
        Serial.println("Selected: 4800 bps (0b00000011)");
        SPED += BPS_4800;
        break;
      case '4':
        Serial.println("Selected: 9600 bps (0b00000100)");
        SPED += BPS_9600;
        break;
      case '5':
        Serial.println("Selected: 19200 bps (0b00000101)");
        SPED += BPS_19200; // Unstable??
        break;
      default:
        continue;
    }
    break;
  }

  char buffr[64];
  memset(buffr, 0, sizeof(char) * 64);
  sprintf((void *) buffr, "CMD = C0|%02X|%02X|%02X|%02X|%02X", ADDH, ADDL, SPED, CHAN, OPTION);
  Serial.println("SPED Values Updated");
  Serial.print("SPED = ");
  Serial.println(SPED, BIN);
  Serial.println(buffr);
}
