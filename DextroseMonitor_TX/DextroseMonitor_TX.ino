#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DataStructure.h"

RF24 NRF(A0, A1);  // CE, CSN

const byte address[6] = "DX001";

#include <HX711_ADC.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 2;  //mcu > HX711 dout pin
const int HX711_sck = 3;   //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(9600);
  if (!NRF.begin()) {
    Serial.println("NRF24L01 Hardware Didn't Respond");
  }
  Serial.println("NRF24L01 Transmitter Mode");
  NRF.setPALevel(RF24_PA_MAX);
  NRF.setDataRate(RF24_250KBPS);
  NRF.openWritingPipe(address);
  NRF.stopListening();

  LoadCell_Setup();
}

void loop() {
  LoadCell_Loop();
  NRF_Send();
  delay(50);
}
