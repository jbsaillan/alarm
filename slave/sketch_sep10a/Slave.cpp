#include "Slave.h"


Slave::Slave(): m_master(new Master)
{
  m_ledPin = A0;
  m_sensorPin = A1;
  m_buzzerPin = 5;

  m_sensorState = false;
  m_stateLed = 0;

  m_timer = 0;
  m_previousTime = millis();
}

//Init all the encessary pins
void Slave::init() {
  pinMode(m_ledPin, OUTPUT);
  pinMode(m_sensorPin, INPUT);
  pinMode(m_buzzerPin, OUTPUT);

  digitalWrite(m_ledPin, HIGH);
  digitalWrite(m_buzzerPin, LOW);
}


uint8_t Slave::getAddress() {
  return m_address;
}

void Slave::setAddress(uint8_t address) {
  m_address = address;
}

//Read the incoming packet
void Slave::getPacket (NRFLite &radio, Radio_Packet &packetReceived) {
  if(radio.hasData()) {
    radio.readData(&packetReceived);
    m_master->setLastMeeting(millis());
  } else {
    packetReceived.emitter = -1; 
  }
}

void Slave::writePacket(Radio_Packet &packetToSend) {
  packetToSend.emitter = getAddress();
  packetToSend.value = m_sensorState;
}

//Send the desired packet to all the slaves 
void Slave::sendPacket(NRFLite &radio, Radio_Packet &packetToSend) {
  radio.send(m_master->getAddress(), &packetToSend, sizeof(packetToSend));
}

//Update the sensor state
void Slave::updateSensor() {
  m_sensorState = !digitalRead(m_sensorPin);
  Serial.write(m_sensorState);
}

void Slave::updateMaster(Radio_Packet &packet) {
  if(packet.emitter != -1) {
      m_master->setAlarmState(packet.value);
  }
  m_master->updateConnection();
}


//Update all the LEDs regarding the state of the system and the slaves
void Slave::updateHardware() {
  m_timer += millis() - m_previousTime;
  m_previousTime = millis();

  //When the sensor is triggered we blink
  if(m_sensorState) {    
    if(m_timer > 500) {
      m_timer = 0;
      m_stateLed = !m_stateLed;
      digitalWrite(m_ledPin, m_stateLed);
    }
  } else {
    digitalWrite(m_ledPin, LOW);
  }

  if(m_master->getAlarmState()) {
    makeSound();
  } else {
    stopSound();
  }

}

void Slave::makeSound() {
  //digitalWrite(m_buzzerPin, HIGH);
  tone(m_buzzerPin, 1000);
}

void Slave::stopSound() {
  //digitalWrite(m_buzzerPin, LOW);
  noTone(m_buzzerPin);
}
