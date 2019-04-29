#include "Slave.h"

Slave::Slave(uint8_t address) {
  m_address = address;
  m_sensorState = 0;
  m_connected = true;
  m_lastMeeting = 0;
}

bool Slave::getSensorState() {
  return m_sensorState;
}

uint8_t Slave::getAddress() {
  return m_address;
}

bool Slave::isConnected() {
  return m_connected;
}

void Slave::setSensorState(int state) {
  m_sensorState = state;
}

void Slave::setLastMeeting (int t) {
  m_lastMeeting = millis();    
}

int Slave::getLastMeeting() {
  return m_lastMeeting;
}

//Check if the slave has talked the last 3  seconds
void Slave::updateConnection() {
  m_connected = millis() < m_lastMeeting + 1000;
}
