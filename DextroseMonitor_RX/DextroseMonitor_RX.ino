#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DataStructure.h"
#include <SoftwareSerial.h>

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 717 bytes
  { 255,78,0,92,0,198,2,17,0,0,0,8,2,106,200,200,84,2,1,0,
  22,0,130,4,38,50,157,5,14,94,66,0,82,129,34,13,35,7,6,5,
  80,7,31,70,32,76,32,85,32,73,32,68,32,32,65,32,76,32,69,32,
  82,32,84,32,32,86,32,49,32,46,32,48,0,129,9,42,29,4,8,18,
  28,4,8,80,65,84,73,69,78,84,32,78,65,77,69,0,129,232,47,72,
  10,8,33,19,4,8,82,79,79,77,32,78,79,46,0,66,12,105,7,76,
  8,54,88,23,128,2,26,69,7,3,13,13,89,47,6,6,0,1,129,241,
  83,48,10,8,50,33,4,8,68,69,88,84,82,79,83,69,32,76,69,86,
  69,76,0,130,3,38,50,157,102,14,94,66,0,110,129,233,47,70,10,105,
  18,28,4,8,80,65,84,73,69,78,84,32,78,65,77,69,0,129,241,83,
  48,10,105,33,19,4,8,82,79,79,77,32,78,79,46,0,66,4,133,47,
  55,105,54,88,23,128,2,26,69,42,117,14,14,186,47,6,6,0,1,129,
  228,124,81,10,105,50,33,4,8,68,69,88,84,82,79,83,69,32,76,69,
  86,69,76,0,129,6,33,71,29,90,16,6,5,8,91,49,93,0,129,42,
  43,14,12,187,16,6,5,8,91,50,93,0,67,23,112,21,24,49,49,27,
  5,1,8,17,11,67,27,121,14,12,146,49,27,5,1,8,21,11,67,5,
  52,21,24,8,22,88,7,4,2,26,21,67,5,57,47,17,8,37,88,7,
  4,2,26,11,67,5,57,47,17,105,22,88,7,4,2,26,21,67,5,93,
  47,17,105,37,88,7,4,2,26,11,131,89,10,15,19,167,2,29,8,1,
  2,2,31,83,69,84,84,73,78,71,83,0,6,18,0,130,4,38,50,157,
  6,19,95,54,0,82,130,55,38,50,157,103,19,93,54,0,110,129,234,47,
  70,10,9,23,28,4,8,80,65,84,73,69,78,84,32,78,65,77,69,0,
  129,242,83,48,10,9,38,19,4,8,82,79,79,77,32,78,79,46,0,7,
  5,93,47,19,9,42,88,8,36,2,26,2,16,129,29,47,70,10,106,23,
  28,4,8,80,65,84,73,69,78,84,32,78,65,77,69,0,7,57,57,47,
  19,106,27,88,8,36,2,26,2,21,129,38,83,48,10,106,38,19,4,8,
  82,79,79,77,32,78,79,46,0,7,57,93,47,19,106,42,88,8,36,2,
  26,2,16,129,43,43,14,12,91,21,6,5,8,91,49,93,0,129,95,43,
  14,12,188,21,6,5,8,91,50,93,0,129,14,9,71,29,65,5,80,12,
  0,83,32,69,32,84,32,84,32,73,32,78,32,71,32,83,32,0,129,243,
  88,48,10,9,54,42,4,8,84,72,82,69,83,72,79,76,68,32,76,69,
  86,69,76,32,91,37,93,0,129,228,126,88,10,106,54,42,4,8,84,72,
  82,69,83,72,79,76,68,32,76,69,86,69,76,32,91,37,93,0,7,6,
  136,47,19,106,58,88,8,52,2,26,2,131,75,167,21,33,168,2,27,7,
  1,2,2,31,72,79,77,69,0,9,7,57,69,47,19,9,27,88,8,36,
  2,26,2,21,7,6,143,47,19,9,58,88,8,52,2,26,2 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  char editPatientRoom1[16]; // string UTF8 end zero
  char editPatientName2[21]; // string UTF8 end zero
  char editPatientRoom2[16]; // string UTF8 end zero
  int16_t editThresholdLevel2; // -32768 .. +32767
  char editPatientName1[21]; // string UTF8 end zero
  int16_t editThresholdLevel1; // -32768 .. +32767

    // output variables
  int8_t patientDexLevel1; // from 0 to 100
  int16_t patientAlarm1; // =0 no sound, else ID of sound, =1001 for example, look sound list in app
  int8_t patientDexLevel2; // from 0 to 100
  int16_t patientAlarm2; // =0 no sound, else ID of sound, =1001 for example, look sound list in app
  char patientDexValue1[11]; // string UTF8 end zero
  char patientDexValue2[11]; // string UTF8 end zero
  char patientName1[21]; // string UTF8 end zero
  char patientRoom1[11]; // string UTF8 end zero
  char patientName2[21]; // string UTF8 end zero
  char patientRoom2[11]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


RF24 NRF(A0, A1);  // CE, CSN

const byte address[6] = "DX001";
char patientName1 = "RYAN GARCIA";

void setup() {
  RemoteXY_Init ();
  Serial.begin(9600);

  if (!NRF.begin()) {
    Serial.println("NRF24L01 Hardware Didn't Respond");
  }

  Serial.println("NRF24L01 Reciever Mode");
  NRF.setPALevel(RF24_PA_MAX);
  NRF.setDataRate(RF24_250KBPS);
  NRF.openReadingPipe(0, address);
  NRF.startListening();

  

  strcpy(RemoteXY.editPatientName1, "RYAN GARCIA");
  strcpy(RemoteXY.editPatientRoom1, "PR-101");
  strcpy(RemoteXY.editPatientName2, "DEVIN HANEY");
  strcpy(RemoteXY.editPatientRoom2, "PR-102");
  RemoteXY.editThresholdLevel1 = 5;
  RemoteXY.editThresholdLevel2 = 5;
}

void loop() {
  RemoteXY_Handler ();
  if (NRF.available()) {
    NRF.read(&payload, sizeof(Payload_Data));  // Read the whole data and store it into the 'data' structure
    int payloadID = payload.ID;
    double payloadDXweight = payload.DXweight;
    
    if (payloadID == 1) {
      int rounded1 = (int)(payloadDXweight+0.5f);
      RemoteXY.patientDexLevel1 = (int) ((rounded1/10.6)+0.5f);
      sprintf(RemoteXY.patientDexValue1, "%d%%",RemoteXY.patientDexLevel1);
      Serial.println("A," + String(payload.ID) + "," + String(payload.DXweight) + ", B," + String(RemoteXY.patientDexLevel1));

      if (RemoteXY.patientDexLevel1 < RemoteXY.editThresholdLevel1) {
        RemoteXY.patientAlarm1 = 1;
      } else {
        RemoteXY.patientAlarm1 = 0;
      }
    }

    if (payloadID == 2) {
      int rounded2 = (int)(((payloadDXweight*2)+143)+0.5f);
      RemoteXY.patientDexLevel2 = (int) ((rounded2/10.6)+0.5f);
      sprintf(RemoteXY.patientDexValue2, "%d%%",RemoteXY.patientDexLevel2);
      Serial.println("A," + String(payload.ID) + "," + String(((payloadDXweight*2)+143)) + ", B," + String(RemoteXY.patientDexLevel2));

      if (RemoteXY.patientDexLevel2 < RemoteXY.editThresholdLevel2) {
        RemoteXY.patientAlarm2 = 2;
      } else {
        RemoteXY.patientAlarm2 = 0;
      }
    }

    strcpy(RemoteXY.patientName1, RemoteXY.editPatientName1);
    strcpy(RemoteXY.patientRoom1, RemoteXY.editPatientRoom1);
    strcpy(RemoteXY.patientName2, RemoteXY.editPatientName2);
    strcpy(RemoteXY.patientRoom2, RemoteXY.editPatientRoom2);
  
  }

  
}
