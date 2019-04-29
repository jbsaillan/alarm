#ifndef MASTER_H_
#define MASTER_H_

#include "Arduino.h"


class Master {
  
  public:

  Master();
  
  int getAddress();

  bool getAlarmState();

  void setAlarmState(int state);

  //Check if the master has talked the last 3  seconds
  void updateConnection();

  void setLastMeeting(int t);

  
  private :

  int m_address;
  //The general state of the alarm
  bool m_alarmState;

  //Is the master powered and communicating with the slave?
  bool m_connected;
  //Last time we heard about it
  unsigned long int m_lastMeeting;
};


#endif
