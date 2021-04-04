//--Parameters settings--
uint8_t ADDH = 0x00;
uint8_t ADDL = 0x00;
uint8_t SPED;

#ifndef PAR_8N1
  #define PAR_8N1     0b00000000  //0x00 (Default)
#endif
#ifndef PAR_8O1
  #define PAR_8O1     0b01000000  //0x40
#endif
#ifndef PAR_8E1
  #define PAR_8E1     0b10000000  //0x80
#endif

#ifndef BAUD_1200
  #define BAUD_1200   0b00000000  //0x00
#endif
#ifndef BAUD_2400
  #define BAUD_2400   0b00001000  //0x08
#endif
#ifndef BAUD_4800
  #define BAUD_4800   0b00010000  //0x10
#endif
#ifndef BAUD_9600
  #define BAUD_9600   0b00011000  //0x18 (Default)
#endif
#ifndef BAUD_19200
  #define BAUD_19200  0b00100000  //0x20
#endif
#ifndef BAUD_38400
  #define BAUD_38400  0b00101000  //0x28
#endif
#ifndef BAUD_57600
  #define BAUD_57600  0b00110000  //0x30
#endif
#ifndef BAUD_115200
  #define BAUD_115200 0b00111000  //0x38
#endif

#ifndef BPS_300
  #define BPS_300     0b00000000  //0x00
#endif
#ifndef BPS_1200
  #define BPS_1200    0b00000001  //0x01
#endif
#ifndef BPS_2400
  #define BPS_2400    0b00000010  //0x02 (Default)
#endif
#ifndef BPS_4800
  #define BPS_4800    0b00000011  //0x03
#endif
#ifndef BPS_9600
  #define BPS_9600    0b00000100  //0x04
#endif
#ifndef BPS_19200
  #define BPS_19200   0b00000101  //0x05
#endif

uint8_t CHAN; //410Mhz = 0x00. Each added 1H means added 1Mhz (Default is 0x017 = 410 + 23 = 433Mhz)
uint8_t OPTION = 0x44; // Default

uint8_t LORA_VERSION[3] = {0xC3, 0xC3, 0xC3};
uint8_t LORA_READ_PARAM[3] = {0xC1, 0xC1, 0xC1};
uint8_t LORA_RESET[3] = {0xC4, 0xC4, 0xC4};
uint8_t HEAD_SAVE = 0xC0;
uint8_t HEAD_NOT_SAVE = 0xC2;
