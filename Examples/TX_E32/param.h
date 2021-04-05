#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "memorysaver.h"

#if !(defined OV2640_MINI_2MP_PLUS)
#error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define CONSOLE_DEBUG     //Disable this when in normal use, as it may lead to performance issues
//#define DISABLE_CAPTURE
//#define DISABLE_PIR
//#define DISABLE_LORA
//#define DISABLE_ACK
//#define SINGLE_CAPTURE
//#define LORA_TEST       //When this is active along with DISABLE_CAPTURE, TX will transmit nothing

byte ADDL = 0xA0;

//               {HEAD, ADDH, ADDL, SPED, CHAN, OPTN};
uint8_t COM[6] = {0xC0, 0x00, 0x00, 0x35, 0x17, 0x44};
char    RST[3] = {0xC4, 0xC4, 0xC4};

//CHOOSE ONLY ONE OF EACH
#define SIZE OV2640_160x120
//#define SIZE OV2640_176x144
//#define SIZE OV2640_320x240
//#define SIZE OV2640_352x288
//#define SIZE OV2640_640x480
//#define SIZE OV2640_800x600
//#define SIZE OV2640_1024x768
//#define SIZE OV2640_1280x1024
//#define SIZE OV2640_1600x1200

#define FX Normal
//#define FX Negative
//#define FX BW

#define FORMAT JPEG
//#define FORMAT BMP

#ifndef BMPIMAGEOFFSET
  #define BMPIMAGEOFFSET 66
const char bmp_header[BMPIMAGEOFFSET] PROGMEM =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};
#endif

#ifndef PIR
  #define PIR   12   //PIR  pin2
#endif

#ifndef M0
  #define M0    4   //LORA pin1
#endif
#ifndef M1
  #define M1    5   //LORA pin2
#endif
#ifndef AUX
  #define AUX   6   //LORA pin5
#endif
#ifndef LTX
  #define LTX   10   // LoRa Module TX
#endif
#ifndef LRX
  #define LRX   11   // LoRa Module RX
#endif

SoftwareSerial lora(LTX, LRX); // TX (pin4), RX (pin3)

#ifndef BAUD
  #define BAUD  9600//57600
#endif
#ifndef payload
  #define payload 256
#endif

bool is_header = false;
int mode = 0;
uint8_t temp = 0, temp_last = 0;
uint8_t start_capture = 0;
const int CS = 7;
int i = 0;
uint32_t y, m = 0;
