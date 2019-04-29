#ifndef MASTER_H_
#define MASTER_H_

#include "Arduino.h"
#include "RadioPacket.h"
#include "Slave.h"
#include <SPI.h>
#include <NRFLite.h>


class Master {
  
  public:

  Master();

  void init();

  uint8_t getAddress();

  void setAddress(uint8_t address);

  //Read the incoming packet
  bool getPacket(NRFLite &radio, Radio_Packet &packetReceived);

  //Write the next packet to send
  void writePacket(Radio_Packet &packetToSend);

  //Send the desired packet to all the slaves 
  void sendPacket(NRFLite &radio, Radio_Packet &packetToSend);

  //Update the sensor states of the slaves according to the received packets
  void updateSlaves(Radio_Packet &packetReceived);

  //Update the state of the system : is any of the sensors triggered ?
  void updateSystem();

  //read any data from bluetoth connection to update the system
  void updateWithBluetooth(char d);

  //Update the alarm state
  void updateAlarm();

  //Update all the LEDs regarding the state of the system and the slaves
  void updateHardware() ;
  

  private:

  //Adress of the NRF attached to the master
  uint8_t m_address;

  // 0: switched off ; 1: switched on
  bool m_systemState;
  // 0: not triggered ; 1: triggered
  bool m_alarmState;
  //Is the test mode activated?
  bool m_testState;
  // 1 :we hzve to ring the alarm (save the high state)
  bool m_warn;

  //All the slaves communicating wth the master
  Slave * slave1;
  Slave * slave2;
  Slave * slave3;

  //List of the diferent pins:
  //0:red 1:firstSlave 2:secondSlave...
  uint8_t m_ledPins[4];
  bool m_stateLed;
  bool m_shouldBlink;
  //Button to switch on/off the alarm
  uint8_t m_button1;
  //Button to test the alarm
  uint8_t m_button2;
  bool m_stateButton2;

  //Timer so the LED can blink
  int m_timer;
  int m_previousTime;
  
};

#endif
