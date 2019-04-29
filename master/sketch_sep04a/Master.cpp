#include "Master.h"

//Constructor
Master::Master(): slave1(new Slave(1)), slave2(new Slave(2)), slave3(new Slave(3))
{
  m_ledPins[0] = 5;
  m_ledPins[1] = 4;
  m_ledPins[2] = 3;
  m_ledPins[3] = 2;
  m_button1 = A2;
  m_button2 = A1;
  
  m_systemState = false;
  m_alarmState = false;
  m_testState = false;
  m_warn = false;
  m_shouldBlink = false;
  m_stateLed = 0;

  m_stateButton2 = false;

  m_timer = 0;
  m_previousTime = millis();
}

//Init all the necessary pins 
void Master::init() {
  for(int i=0; i<4; i++) {
    pinMode(m_ledPins[i], OUTPUT);
    digitalWrite(m_ledPins[i], HIGH);
  }
  pinMode(m_button1, INPUT);
  pinMode(m_button2, INPUT);
}

uint8_t Master::getAddress() {
  return m_address;
}

void Master::setAddress(uint8_t address) {
  m_address = address;
}

//Read the incoming packet
bool Master::getPacket (NRFLite &radio, Radio_Packet &packetReceived) {
  if(radio.hasData()) {
    radio.readData(&packetReceived);
    Serial.write("got one");
  } else {
    packetReceived.emitter = -1;
  }
}

//Write the next packet to send
void Master::writePacket(Radio_Packet &packetToSend) {
  packetToSend.emitter = getAddress();
  packetToSend.value = m_warn || m_testState;;
}

//Send the desired packet to all the slaves
//Delays are mandatory 
void Master::sendPacket(NRFLite &radio, Radio_Packet &packetToSend) {
  radio.send(slave1->getAddress(), &packetToSend, sizeof(packetToSend));
  delay(10);
  radio.send(slave2->getAddress(), &packetToSend, sizeof(packetToSend));
  delay(10);
  radio.send(slave3->getAddress(), &packetToSend, sizeof(packetToSend));
  delay(10);
}

//Update the sensor states of the slaves according to the received packets
void Master::updateSlaves (Radio_Packet &packetReceived) {
  //Check the sender
  if(packetReceived.value >= 0 && packetReceived.value <= 3) {
    //Update the correct slave
    switch(packetReceived.emitter) {
      case 1:
        slave1->setSensorState(packetReceived.value);
        slave1->setLastMeeting(millis());
        break;
      case 2:
        slave2->setSensorState(packetReceived.value);
        slave2->setLastMeeting(millis());
        break;
      case 3:
        slave3->setSensorState(packetReceived.value);
        slave3->setLastMeeting(millis());
        break;
    }
  }
}



//Update the state of the system : is any of the sensors triggered ?
void Master::updateSystem () {
  //Check wether they're still alive or not
  slave1->updateConnection();
  slave2->updateConnection();
  slave3->updateConnection();

  //Check if their sensors are triggered
  m_alarmState = (slave1->getSensorState() && slave1->isConnected());
  m_alarmState |= (slave2->getSensorState() && slave2->isConnected());
  m_alarmState |= (slave3->getSensorState() && slave3->isConnected());
}



//Update the alarm state
void Master::updateAlarm() {

  //Have you pushed the main button ?
  if(digitalRead(m_button2)) {
    m_stateButton2 = true;
  }
  if(!digitalRead(m_button2) && m_stateButton2) {
    //Then switch on or off the alarm depending on the previous state
    m_systemState = !m_systemState;
    m_stateButton2 = false;
    m_warn = false;
  }

  //Do you want to test the alarm ?
  m_testState = digitalRead(m_button1);

  //Final output : do we nedd to activate the buzzers ?
  m_warn |= (m_alarmState && m_systemState);
  
}

//Update the alarm depending on the bluetooth
void Master::updateWithBluetooth(char d) {
   if(d== 1) {
      m_systemState = true;
   } else if (d == 2) {
      m_systemState = false;
      m_warn = false;
   }
}



//Update all the LEDs regarding the state of the system and the slaves
void Master::updateHardware() {
  m_timer += millis() - m_previousTime;
  m_previousTime = millis();

  //If the timer is out, we should make the correct leds blink
  if(m_timer > 500) {
    m_timer = 0;
    m_stateLed = !m_stateLed;
    m_shouldBlink = true;
  }

  if(m_systemState) {
    if(m_warn) {
      if(m_shouldBlink) {
        digitalWrite(m_ledPins[0], m_stateLed);
      }
    } else {
      digitalWrite(m_ledPins[0], LOW);
    }
  }else {
    digitalWrite(m_ledPins[0], HIGH);
  }

  if(slave1->isConnected()) {
    if(slave1->getSensorState()){
      if(m_shouldBlink) {
        //If the slave is triggered, the LED should blink
        digitalWrite(m_ledPins[1], m_stateLed);
      }
    } else {
      //If the slave is not triggered the LED is on
      digitalWrite(m_ledPins[1], LOW);
    }
  } else {
    //If the slave is not connected, the LED is switched off
    digitalWrite(m_ledPins[1], HIGH);
  }

  if(slave2->isConnected()) {
    if(slave2->getSensorState()){
      if(m_shouldBlink) {
        digitalWrite(m_ledPins[2], m_stateLed);
      }
    } else {
      digitalWrite(m_ledPins[2], LOW);
    }
  } else {
    digitalWrite(m_ledPins[2], HIGH);
    Serial.write("off");
  }

  if(slave3->isConnected()) {
    if(slave3->getSensorState()){
      if(m_shouldBlink) {
        digitalWrite(m_ledPins[3], m_stateLed);
      }
    } else {
      digitalWrite(m_ledPins[3], LOW);
    }
  } else {
    digitalWrite(m_ledPins[3], HIGH);
  }

  m_shouldBlink = false;    
}
