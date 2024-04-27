#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DataStructure.h"
#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3

// Set up a new SoftwareSerial object
SoftwareSerial BTSerial = SoftwareSerial(rxPin, txPin);

RF24 NRF(A0, A1);  // CE, CSN

const byte address[6] = "DX001";

void setup() {
  Serial.begin(9600);
  if (!NRF.begin()) {
    Serial.println("NRF24L01 Hardware Didn't Respond");
  }

  Serial.println("NRF24L01 Reciever Mode");
  NRF.setPALevel(RF24_PA_MAX);
  NRF.setDataRate(RF24_250KBPS);
  NRF.openReadingPipe(0, address);
  NRF.startListening();

  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // Set the baud rate for the SoftwareSerial object
  BTSerial.begin(9600);
}

void loop() {
  if (NRF.available()) {
    NRF.read(&payload, sizeof(Payload_Data));  // Read the whole data and store it into the 'data' structure
    Serial.println("A," + String(payload.ID) + "," + String(payload.DXweight) + ",B");
    BTSerial.println("A," + String(payload.ID) + "," + String(payload.DXweight) + ",B");
  }
}
