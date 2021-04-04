# settings_LoRa
 

*Pins*
M0   4 (Lora Pin 1)
M1   5 (Lora Pin 2)
AUX  6 (Lora Pin 5)
TX  10 (Lora Pin 4)
RX  11 (Lora Pin 3)


(Pins can be changed if needed. The Only pins not allowed are 0 and 1, which corresponds to Arduino's Main Hardware Serial.)


Software Serial Library is used to create Serial Object:
> SoftwareSerial Slora(10, 11); // TX (pin4), RX (pin3)


void setup() : Sets Lora Module on Setup Mode. Baud Rate is set on 9600 as required in the datasheet.


The Setup packet is comprised of 6 bytes:
 COM = { HEAD, ADDH, ADDL, SPED, CHAN, OPTION }
  where:
    -- HEAD: Marks the type of instruction. 
       > 0xC0 must set and save the parameters. 
       > 0xC2 must set but not save the parameters.
    -- ADDH and ADDL: The High and Low Bytes, respectively, for the Module Address. 
       > 0x0000 is reserved for Universal Transmission/Reception.
    -- SPED: Transmission parameters. Please refer to pages 19-20 of the User's Manual.
    -- CHAN: Modulation Channel. Channels between 410 and 441 MHz are allowed. 
       > The Channel is set through the formula: (410 + CHAN) * 1MHz.
    -- OPTION: Miscellaneous. Please refer to pages 20-21 of the User's Manual.


void loop() : Prompts a menu for user selection. The user must input the valid option on Serial:
** 0xC0 - Set and Save Parameters **        (param_settings_save())
** 0xC1 - Read Parameters Currently Set **  (param_read_config())
** 0xC2 - Set Parameters (Don't Save) **    (param_settings_not_save())
** 0xC3 - Read Module Version **            (param_read_version())
** 0xC4 - Reset Module **                   (param_reset())


void setADDR() : User Inputs Address for the Lora Module
  Options:
    -- 0x0000
    -- 0x00A0
    -- 0x00B0


void setSPED() : User Inputs Parameters for Lora Communication
  - Set Parity: 8N1 (0/Default), 8O1 (1) or 8E1 (2).
  - Set Baud Rate: 1200 (0), 2400 (1), 4800 (2), 9600 (3/Default), 19200 (4), 38400 (5), 57600 (6) or 115200 (7).
  - Set Air Data Rate: 300 (0), 1200 (1), 2400 (2/Default), 4800 (3), 9600 (4) or 19200 (5).
  - Channel: Fixed at 0x00 (410 MHz)
  - Options: Default Options (0x44)
