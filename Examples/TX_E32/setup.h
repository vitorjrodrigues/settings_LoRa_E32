//#include "memorysaver.h"

#if !(__has_include("param.h"))
  #include "param.h"
#endif

ArduCAM myCAM( OV2640, CS );
uint8_t read_fifo_burst(ArduCAM myCAM);

void camStart(ArduCAM cam) {
  Wire.begin();  //Start i2c
  pinMode(CS, OUTPUT);  //Toggle CS as output
  digitalWrite(CS, HIGH); //And keep it high (Digital 0)
  SPI.begin(); //Start SPI
  cam.write_reg(0x07, 0x80); //Clear Arducam CLPD
  delay(100);
  cam.write_reg(0x07, 0x00); 
  delay(100);
}

void camCheck(ArduCAM cam) //Check if the ArduCAM SPI bus is OK
{
  while (1) //Check SPI connection
  {
    cam.write_reg(ARDUCHIP_TEST1, 0x55);
    if ((cam.read_reg(ARDUCHIP_TEST1)) != 0x55) 
    { 
      Serial.println("SPI interface Error!"); 
      delay(1000); 
      continue; 
    }
    else { break; }
  }
  uint8_t vid, pid;
  while (1) //Test OV2640 sensor module
  {
    cam.wrSensorReg8_8(0xff, 0x01);
    cam.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    cam.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))) 
    { 
      Serial.println("Can't find OV2640 module!"); 
      delay(1000); 
      continue; 
    }
    else 
    { 
      break; 
    }
  }
}


void camInit(ArduCAM cam) //Change to JPEG capture mode and initialize the OV2640 module
{
  cam.set_format(FORMAT); 
  cam.InitCAM();
  cam.OV2640_set_JPEG_size(SIZE); 
  delay(100);
  cam.OV2640_set_Special_effects(FX); 
  delay(400);
  cam.clear_fifo_flag(); 
  cam.write_reg(ARDUCHIP_FRAMES, 0x00);
}

void blink3times() //Blink LED 3 times to warn that system is ready to capture pictures
{
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  delay(75);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(75);
  digitalWrite(LED_BUILTIN,LOW);
  delay(75);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(75);
  digitalWrite(LED_BUILTIN,LOW);
  delay(75);
  digitalWrite(LED_BUILTIN,HIGH);
}

void serialBegin(){
  delay(100);
  Serial.begin(BAUD); //Start Serial
  while (!Serial); // wait for serial port to connect. Needed for native USB port only
}

void auxWait(){
  while(!(digitalRead(AUX))){};
  delay(2);
}

void warmUp(){
  delay(4000);
  pinMode(PIR, INPUT);
  blink3times();
}

void setCamera(){
  camStart(myCAM);
  camCheck(myCAM);
  camInit(myCAM);
}

void SetupMode() {
  lora.end();
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  lora.begin(9600);
  delay(100);
}

void NormalMode() {
  lora.end();
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  lora.begin(BAUD);
  delay(100);
}

void SendCOM() {
  SetupMode();
  lora.write(COM, 6);
  delay(100);
  NormalMode();
}

void ResetLORA() {
  SetupMode();
  lora.write(RST,3);
  while(digitalRead(AUX) == 1);
  NormalMode();
  delay(250);
}

void setLoRa(){
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  lora.begin(BAUD);
  //ResetLORA();
}

void initialMessages()
{
#ifdef CONSOLE_DEBUG
  Serial.println("\nSerial Debug ENABLED (Check 'param.h' for #define settings)");
#endif
#ifdef DISABLE_CAPTURE && CONSOLE_DEBUG
  Serial.println("Camera Disabled");
#endif
#ifdef DISABLE_ACK && CONSOLE_DEBUG
  Serial.println("Ack Disabled");
#endif
#ifdef DISABLE_PIR && CONSOLE_DEBUG
  Serial.println("PIR Disabled");
#endif
#ifdef DISABLE_LORA && CONSOLE_DEBUG
  Serial.println("LoRa Disabled");
#endif
#ifdef LORA_TEST && CONSOLE_DEBUG
  Serial.println("LoRa Testing ENABLED. Sending a 48 byte long fixed message.");
#endif
#ifdef SINGLE_CAPTURE && CONSOLE_DEBUG
  Serial.println("Single Capture Mode Active (Reset for a new Transmission)");
#endif
}
