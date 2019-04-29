/**
 * 
 * 
 * Code written by Jean-Baptiste Saillan
 * Fell free to use it and to change it
 * 
 * 
 * MASTER MODULE
 * 
 */
 
#include <SPI.h>
#include <NRFLite.h>
#include <SoftwareSerial.h>

#include "RadioPacket.h"
#include "Master.h"
#include "Slave.h"

//Pins CE and CSN on the Arduino (could be any GPIO)
#define CE 7
#define CSN 8


NRFLite _radio;

Master myMaster;

Radio_Packet packetIn;
Radio_Packet packetOut;

//Bluetooth serial
SoftwareSerial hc06(0,1);
char data = 0;

void setup() {

  // Master's address is always 0 
  myMaster.setAddress(0);
  _radio.init(myMaster.getAddress(), CE, CSN);
  myMaster.init();

  //Incorrect value
  packetIn.value = -1;

  Serial.begin(9600);
  hc06.begin(9600);
}

void loop() {
  
   myMaster.getPacket(_radio, packetIn);

   myMaster.updateSlaves(packetIn);

   myMaster.updateSystem();

   myMaster.updateAlarm();

   myMaster.writePacket(packetOut);  
   delay(10);
   
   myMaster.sendPacket(_radio, packetOut);
   delay(10);
   
   if (hc06.available()){
    data = hc06.read();
    myMaster.updateWithBluetooth(data);
    delay(10);
  }

  myMaster.updateHardware();
}
