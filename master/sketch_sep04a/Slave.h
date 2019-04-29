#ifndef SLAVE_H_
#define SLAVE_H_

#include "Arduino.h"
#include "RadioPacket.h"
#include <SPI.h>
#include <NRFLite.h>


class Slave {
  public:

  Slave(uint8_t address);
  
  bool getSensorState();

  uint8_t getAddress();

  bool isConnected();

  void setSensorState(int state);

  //Set the last time we received something from the slave
  void setLastMeeting(int t);

  int getLastMeeting();

  //Check if the slave has talked for the last 3  seconds
  void updateConnection();

  private:

  //Address of the NRf attached to the slave
  uint8_t m_address;

  //0: shutter closed ; 1:shutter open
  bool m_sensorState;

  //Is the slave powered and communicating with the master?
  bool m_connected;
  //Last time we heard about it
  unsigned long int m_lastMeeting;
  
};

#endif
