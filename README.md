# settings_LoRa_E32 - Comprehensive interface to setup EBYTE's LoRa Modules (E32 Series)
 
## Pins 
Lora Pin | Arduino Pin |
-------- | ----------- |
1 - M0   | 04          |
2 - M1   | 05          |
3 - RX   | 11          |
4 - TX   | 10          |
5 - AUX  | 06          |




(Pins can be changed if needed. The Only pins not allowed are 0 and 1, which corresponds to Arduino's Main Hardware Serial.)


Software Serial Library is used to create Serial Object:
> SoftwareSerial Slora(10, 11); // TX (pin4), RX (pin3)

## Setting the Module
void setup() : Sets Lora Module on Setup Mode. Baud Rate is set on 9600 as per required in the datasheet.


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

## Parameter Setting Functions (ADDR and SPED*)
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
    
 void setCHAN(): *This will be implemented in the future.
  ***Channel is Fixed at 0x00 (410 MHz)*** 
 
 void setOPTION(): *This will be implemented in the future.
  ***Default Options (0x44)***
  
  
 ## Examples
 
 Examples for a simple Receiver and Transmitter can be found on /Examples/ folder.
 Please note a few important details:
  - Serial and SoftwareSerial BAUD rates may have different values if needed, but absolute functionality is not guaranteed (further tests are needed).
  - SoftwareSerial **MUST** have the same BAUD rate of its module. 
  - Receiver and Transmitter, however, do not need to share the same BAUD Rate themselves.
  - They still need to share the **SAME** Air Data Rate.
  - Pin Presets can be changed freely (avoiding pins 0 and 1, which are reserved for Arduino's Programming Serial), just remember that LoRa TX pin is connected to SoftwareSerial RX pin and vice-versa.
