#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DataStructure.h"

RF24 NRF(8, 9);  // CE, CSN
const byte address[6] = "DX001";

uint16_t dx_patient1 = 501;
uint16_t dx_patient2 = 502;

#define buzzer 4

void setup() {
  Serial.begin(9600);

  Serial.println("System Starting...");
  if (!NRF.begin()) {
    Serial.println("NRF24L01 is Broken or Hardware Not Installed");
  } else {
    Serial.println("NRF24L01 detected!");
  }
  NRF.setPALevel(RF24_PA_MAX);
  NRF.setDataRate(RF24_250KBPS);
  NRF.openReadingPipe(0, address);
  NRF.startListening();

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, 0);
}


void loop() {
  if (NRF.available()) {
    Serial.println("Data Received!");
    NRF24L01_DecodeData();
  }


  String toSend = "A," + String(dx_patient1) + "," + String(dx_patient2) + ",B";
  Serial.println(toSend);

  if ((dx_patient1 > 100 && dx_patient1 < 200) || (dx_patient2 > 100 && dx_patient2 < 200)) {
    digitalWrite(buzzer, 1);
    delay(500);
    digitalWrite(buzzer, 0);
    delay(500);
  } else if (dx_patient1 < 100 || dx_patient2 < 100) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(buzzer, 1);
      delay(100);
      digitalWrite(buzzer, 0);
      delay(100);
    }
  } else {
    digitalWrite(buzzer, 0);
    delay(1000);
  }
}

void NRF24L01_DecodeData() {
  NRF.read(&payload, sizeof(Payload_Data));  // Read the whole data and store it into the 'data' structure
  // Serial.print("Patient ID:");
  // Serial.println(payload.ID);
  // Serial.print("DXWeight:");
  // Serial.println(payload.DXweight);
  if (payload.ID == 1) {
    dx_patient1 = payload.DXweight;
  } else if (payload.ID == 2) {
    dx_patient2 = payload.DXweight;
  }
}