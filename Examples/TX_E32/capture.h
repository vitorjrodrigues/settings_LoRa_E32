#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

#if !(__has_include("param.h"))
  #include "param.h"
#endif

void camCapture(ArduCAM cam) // Store captured frame on internal FIFO and revert capture flag to idle mode.
{
  if (start_capture == 1) // Flag to initiate capture (Value 1 for MODE 1)
  {
    cam.flush_fifo(); 
    cam.clear_fifo_flag();
    cam.start_capture(); 
    start_capture = 0;
  }
}

uint8_t read_fifo_burst(ArduCAM myCAM) // Burst FIFO data through Serial Port
{
  uint32_t length = 0;
  uint8_t buf[payload];
  int k = 0;
  length = myCAM.read_fifo_length();
  if (length >= MAX_FIFO_SIZE) //512 kb
  {
    return 0;
  }
  if (length == 0 ) //0 kb
  {
    return 0;
  }
  myCAM.CS_LOW();
  myCAM.set_fifo_burst(); //Set fifo burst mode
  temp = SPI.transfer(0x00);
  length --;
  while ( length-- )
  {
    temp_last = temp; //Stores the previous byte received from SPI
    temp =  SPI.transfer(0x00); //Reads the current byte from SPI
    if (is_header == true) 
    {
      buf[k] = temp;
      k++;
      if(k == payload){
        for(int i=0; i<k;i++)
        {
#ifndef DISABLE_LORA
          lora.write(buf[i]); 
#else
          Serial.write(buf[i]);
#endif
        }
        k = 0;
        auxWait();
      }
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF)) 
    { 
      is_header = true;
      buf[k] = temp_last;
      k++;
      buf[k] = temp;
      k++;
    }
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break the while loop
    {
      for(int i=0; i<k;i++){ 
#ifndef DISABLE_LORA
        lora.write(buf[i]); 
#else
        Serial.write(buf[i]);
#endif
      }
      k = 0;
      auxWait();
      break; 
    }
    delayMicroseconds(15);
  }
  myCAM.CS_HIGH();
  is_header = false; 
  return 1;
}
