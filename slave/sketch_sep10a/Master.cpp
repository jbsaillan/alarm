#include "Master.h"


Master::Master() {
  m_address = 0;
  m_alarmState = false;
  m_connected = true;
  m_lastMeeting = 0;
}

int Master::getAddress() {
  return m_address;
}

bool Master::getAlarmState() {
  return m_alarmState;  
}

//Update the master depending on the received packet
void Master::setAlarmState(int state) {
  if(state == 1 || state == 0) m_alarmState = state;  
}

//Check if the master has talked the last 3  seconds
void Master::updateConnection() {
  m_connected = millis() < m_lastMeeting + 1000;
  if(m_connected == 0) {
    Serial.println(m_connected);
  }
  
}

void Master::setLastMeeting(int t) {
  m_lastMeeting = millis();
}
