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
> COM = { HEAD, ADDH, ADDL, SPED, CHAN, OPTION }

where:
Byte Value | Content | Notes
------------ | --------------- | -------------
HEAD | Marks the type of instruction. |  0xC0 marks the command to set and save the parameters. 0xC2 marks the command to set but not save the parameters.       
ADDH | High Byte for Module Address. | 
ADDL | Low Byte for Module Address. | 0x0000 is reserved for Universal Transmission/Reception.
SPED | Transmission parameters. | Refer to pages 19-20 of the User's Manual to set this value.
CHAN | Modulation Channel. | Channels between 410 and 441 MHz are allowed.  The Channel is set through the formula: (410 + CHAN) * 1MHz.
OPTION | Miscellaneous. | Please refer to pages 20-21 of the User's Manual to set this value. 


void loop() : Prompts a menu for user selection. The user must input the valid option on Serial:
Option | Description | Function Executed
------ | ----------- | -----------------
0xC0 | Set and Save Parameters | param_settings_save()
0xC1 | Read Parameters Currently Set | param_read_config()
0xC2 | Set Parameters (Don't Save) | param_settings_not_save()
0xC3 | Read Module Version | param_read_version()
0xC4 | Reset Module | param_reset()


void setADDR() : User Inputs Address for the Lora Module. Values can be altered freely. On the Future the user will be able to input the intended adress directly on Serial Buffer.

Options Presetted:
Option 0 | Option 1 | Option 2
------ | ----------- | -----------------
   ADDR 0x0000 | ADDR 0x00A0 | ADDR 0x00B0
    

void setSPED() : User Inputs Parameters for Lora Communication
  - Set Parity: 
    Option 0 | Option 1 | Option 2
    ------- | ------- | -------
    8N1 (Default) | 8O1 | 8E1
  - Set Baud Rate: 
    Option 0 | Option 1 | Option 2 | Option 3       | Option 4 | Option 5 | Option 6 | Option 7
    -------- | -------- | -------- | -------------- | -------- | -------- | -------- | --------
    1200     | 2400     | 4800     | 9600 (Default) | 19200    | 38400    | 57600    | 115200
  - Set Air Data Rate: 
    Option 0 | Option 1 | Option 2       | Option 3 | Option 4 | Option 5
    -------- | -------- | -------------- | -------- | -------- | --------
    300      | 1200     | 2400 (Default) | 4800     | 9600     | 19200 
  - Channel: 
  ***Fixed at 0x00 (410 MHz)***
  - Options: 
  ***Default Options (0x44)***
