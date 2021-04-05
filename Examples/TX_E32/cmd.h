#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

#if !(__has_include("param.h"))
  #include "param.h"
#endif

void camCMD(ArduCAM cam)
{
  switch (temp)
  {
    case 0x10:
      mode = 1; 
      temp = 0xff; 
      start_capture = 1;
      break;
    case 0x70:
      mode = 1; 
      temp = 0xff; 
      start_capture = 1;
      break;
  } 
}
