/**
 * 
 * 
 * Code written by Jea-Baptiste Saillan
 * Fell free to use it and change it
 * 
 * SLAVE MODULE
 * 
 * 
 */


#include "Slave.h"
#include "Master.h"
#include "Radio_Packet.h"
#include <SPI.h>
#include <NRFLite.h>

//CE and CSn pins of the Arduino
#define CE 7
#define CSN 8

//Address of the slave
//Be carefull, two slaves must not have the same address !
//It must not be 0
#define ADDR 2


NRFLite _radio;

Slave mySlave;

Radio_Packet packetIn;
Radio_Packet packetOut;

void setup() {
 
  mySlave.setAddress(ADDR);
  _radio.init(mySlave.getAddress(), CE, CSN);

  mySlave.init();

  Serial.begin(9600);
}

void loop() {
     

   mySlave.updateSensor();

   mySlave.writePacket(packetOut); 

   mySlave.sendPacket(_radio, packetOut);

   delay(10);

   mySlave.getPacket(_radio, packetIn);

   delay(10);

   mySlave.updateMaster(packetIn);

   mySlave.updateHardware();

}
