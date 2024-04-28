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
uint8_t RemoteXY_CONF[] =   // 373 bytes
  { 255,74,0,28,0,110,1,17,0,0,0,31,2,106,200,200,84,1,1,21,
  0,129,34,13,35,7,6,7,46,4,8,70,32,76,32,85,32,73,32,68,
  32,32,65,32,76,32,69,32,82,32,84,32,32,86,32,49,32,46,32,48,
  0,130,3,34,100,74,5,14,94,66,0,17,129,9,42,29,4,8,18,28,
  4,8,80,65,84,73,69,78,84,32,78,65,77,69,0,7,79,37,21,8,
  8,22,88,8,36,2,26,2,21,129,232,47,72,10,8,33,19,4,8,82,
  79,79,77,32,78,79,46,0,7,36,57,19,19,8,37,88,8,36,2,26,
  2,16,66,12,105,7,76,8,54,88,23,128,2,26,69,7,3,13,13,89,
  47,6,6,0,1,129,241,83,48,10,8,50,33,4,8,68,69,88,84,82,
  79,83,69,32,76,69,86,69,76,0,130,3,38,50,157,102,14,94,66,0,
  152,129,233,47,70,10,105,18,28,4,8,80,65,84,73,69,78,84,32,78,
  65,77,69,0,7,36,57,19,19,105,22,88,8,36,2,26,2,21,129,241,
  83,48,10,105,33,19,4,8,82,79,79,77,32,78,79,46,0,7,36,93,
  19,19,105,37,88,8,36,2,26,2,16,66,4,133,47,55,105,54,88,23,
  128,2,26,69,42,117,14,14,186,47,6,6,0,1,129,228,124,81,10,105,
  50,33,4,8,68,69,88,84,82,79,83,69,32,76,69,86,69,76,0,129,
  6,33,71,29,90,16,6,5,8,91,49,93,0,129,42,43,14,12,187,16,
  6,5,8,91,50,93,0,67,23,112,21,24,49,49,27,5,1,8,17,11,
  67,27,121,14,12,146,49,27,5,1,8,21,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  char editPatientName1[21]; // string UTF8 end zero
  char editPatientRoom1[16]; // string UTF8 end zero
  char editPatientName2[21]; // string UTF8 end zero
  char editPatientRoom2[16]; // string UTF8 end zero

    // output variables
  int8_t patientDexLevel1; // from 0 to 100
  int16_t patientAlarm1; // =0 no sound, else ID of sound, =1001 for example, look sound list in app
  int8_t patientDexLevel2; // from 0 to 100
  int16_t patientAlarm2; // =0 no sound, else ID of sound, =1001 for example, look sound list in app
  char patientDexValue1[11]; // string UTF8 end zero
  char patientDexValue2[11]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


RF24 NRF(A0, A1);  // CE, CSN

const byte address[6] = "DX001";

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

  // Define pin modes for TX and RX
  // pinMode(rxPin, INPUT);
  // pinMode(txPin, OUTPUT);

  // Set the baud rate for the SoftwareSerial object
  // BTSerial.begin(9600);
  //RemoteXY.patientDexLevel1 = 50;
  //RemoteXY.patientDexLevel2 = 100;
}

void loop() {
  RemoteXY_Handler ();
  if (NRF.available()) {
    NRF.read(&payload, sizeof(Payload_Data));  // Read the whole data and store it into the 'data' structure
    Serial.println("A," + String(payload.ID) + "," + String(payload.DXweight) + ",B");
    // BTSerial.println("A," + String(payload.ID) + "," + String(payload.DXweight) + ",B");


    Serial.println(payload.DXweight);
    int rounded = (int)(payload.DXweight+0.5f);
    Serial.println(rounded);

    RemoteXY.patientDexLevel1 = (int) ((rounded/10)+0.5f);

    sprintf(RemoteXY.patientDexValue1, "%d%%",RemoteXY.patientDexLevel1);

    if (RemoteXY.patientDexLevel1 < 5) {
      RemoteXY.patientAlarm1 = 1;
    } else {
      RemoteXY.patientAlarm1 = 0;
    }
  }

  // char str[] = "Value";
  // int val = 1234;
  // sprintf (RemoteXY.text_3, "%s is %d", str, val); // result: "Value is 1234"

  // int val = 1234;
  // itoa (val, RemoteXY.text_1, 10);

  //  double val = 1234.321;
  // dtostrf(val, 0, 2, RemoteXY.text_1); // result: "1234.32"
// float f = 2.345f;
// float f2 = 2.845f;

// int rounded = (int)(f+0.5f); // rounded  == 2
// rounded = (int)(f2+0.5f); // rounded  == 3

}
