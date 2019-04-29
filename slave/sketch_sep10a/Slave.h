#ifndef SLAVE_H_
#define SLAVE_H_

#include "Arduino.h"
#include "Master.h"
#include "Radio_Packet.h"
#include <SPI.h>
#include <NRFLite.h>



class Slave {
  public:

  Slave();

  void init();

  //Return the address of the NRF attached to the slave
  uint8_t getAddress();

  //Set the address of the NRF attached to the slave
  void setAddress(uint8_t address);

  //Read the incoming packet
  void getPacket (NRFLite &radio, Radio_Packet &packetReceived);

  //Write a packet according to the system state
  void writePacket(Radio_Packet &packetToSend);

  //Send the desired packet to all the slaves 
  void sendPacket(NRFLite &radio, Radio_Packet &packetToSend);

  //Update the sensor state
  void updateSensor();

  //Update he sitation of the master : is any sensor alarm triggered ?
  void updateMaster(Radio_Packet &packet);

  //Update all the LEDs regarding the state of the system and the slaves
  void updateHardware();

  //Activate the buzzer
  void makeSound();

  //Switch off the buzzer
  void stopSound();
  

  private:

  //Adress of the NRF attached
  uint8_t m_address;

  // 0: not triggered ; 1: triggered
  bool m_sensorState;

  //The master communicating with our slave
  Master * m_master;

  //Led representating the system
  uint8_t m_ledPin;
  bool m_stateLed;
  //Timer so the LED can blink
  int m_timer;
  int m_previousTime;

  //Sensor that an trigger the alarm
  uint8_t m_sensorPin;

  //Buzzer
  uint8_t m_buzzerPin;

  
};



#endif
