#include "Arduino.h"
#include "vLib.h"
#include "dht.h"

dht DHT;

vLib::vLib()
{
}

void vLib::lcdWriteNibble(unsigned char d)
{
  digitalWrite(_lcdClk,LOW);
  digitalWrite(_lcdData,LOW);

  for(_i=0;_i<8;_i++)
  {
    delayMicroseconds(10);
    digitalWrite(_lcdClk,HIGH);
    delayMicroseconds(10);
    digitalWrite(_lcdClk,LOW);
    delayMicroseconds(10);
  }

  digitalWrite(_lcdData,HIGH);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,HIGH);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,LOW);
  delayMicroseconds(10);

  if(_RS==0)
  {
    digitalWrite(_lcdData,LOW);
  }
  else
  {
    digitalWrite(_lcdData,HIGH);
  }
	
  delayMicroseconds(10);
  digitalWrite(_lcdClk,HIGH);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,LOW);
  delayMicroseconds(10);	

  for(_i=8;_i>0;_i=_i>>1)
  {
  if(d&_i)
  {
    digitalWrite(_lcdData,HIGH);
  }
  else
  {
    digitalWrite(_lcdData,LOW);
  }
		
  delayMicroseconds(10);
  digitalWrite(_lcdClk,HIGH);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,LOW);
  }

  digitalWrite(_lcdData,LOW);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,HIGH);
  delayMicroseconds(10);
  digitalWrite(_lcdClk,LOW);
  delayMicroseconds(10);

  digitalWrite(_lcdData,HIGH);
  delayMicroseconds(100);
  digitalWrite(_lcdData,LOW);
}

void vLib::lcdInitialize(int lcdData, int lcdClk)
{
  pinMode(lcdData, OUTPUT);
  _lcdData = lcdData;
  pinMode(lcdClk, OUTPUT);
  _lcdClk = lcdClk;

  _RS = 0;
  delay(15);
  lcdWriteNibble(0x3);
  delay(5);
  lcdWriteNibble(0x3);
  delay(5);
  lcdWriteNibble(0x3);
  delay(5);
  lcdWriteNibble(0x2);
  
  lcdWriteNibble(0x2);
  delay(5);
  lcdWriteNibble(0x8);
  delay(5);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0x8);
  delay(5);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0x1);
  delay(5);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0x6);
  delay(5);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0xF);  
  delay(5);  
}

void vLib::lcdCmd(unsigned char cbyte)
{
  _RS = 0;
  lcdWriteNibble(cbyte>>4);
  delayMicroseconds(100);
  lcdWriteNibble(cbyte&0xF);
}

void vLib::lcdData(unsigned char dbyte)
{
  _RS = 1;
  lcdWriteNibble(dbyte>>4);
  delayMicroseconds(100);
  lcdWriteNibble(dbyte&0xF);
}

void vLib::lcdPrint(const char * message)
{
  lcdCmd(0x8);
  while(*message)
  {
    lcdData(*message++);
  }
  lcdCmd(0xE);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0xC);
  delay(5);
}

void vLib::lcdPrint(float message_float)
{
  dtostrf(message_float,0,2,_string);
  lcdPrint(_string);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0xC);
  delay(5);
}

void vLib::lcdPrint(unsigned long message_long)
{
  char _string[20];
  ltoa(message_long, _string, 10);
  lcdPrint(_string);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0xC);
  delay(5);
}

void vLib::lcdPrint(int message_int)
{
  itoa(message_int,_string,10);
  lcdPrint(_string);
  lcdWriteNibble(0x0);
  delay(5);
  lcdWriteNibble(0xC);
  delay(5);
}

//void vLib::lcdPrint(uint8_t message_uint)
//{
  //itoa(message_uint,_string,10);
  //dtostrf(message_uint,0,2,_string);
  //lcdPrint(_string);
//}

void vLib::lcdClear()
{
  lcdCmd(0x01);
}

void vLib::lcdGoToFirstLine()
{
  lcdCmd(0xC0);
}

void vLib::lcdGoToSecondLine()
{
  lcdCmd(0x80);
}

void vLib::lcdSetCursor(int row, int column)
{
  _temp = 127 + column;
  if (row==2)
  {
    _temp = _temp + 64;
  }
  lcdCmd(_temp);
}

// pin_temperature will be output pin where lm35 will be connected
// make private variable _pin_temperature so it can be used in the other functions

void vLib::InitializeTemperature(int pin_temperature)
{
  pinMode(pin_temperature, INPUT); 
  _pin_temperature = pin_temperature;  
}

// this function returns the temperature in celsius using a LM35 
float vLib::ReadTemperature()
{
  _value_temperature = analogRead(_pin_temperature);
  _value_temperature_celsius = _value_temperature * 0.09765625;
  return _value_temperature_celsius;
}

void vLib::InitializeDistanceUltrasonic(int pin_echo, int pin_trigger)
{
  pinMode(pin_echo, INPUT);
  _pin_echo = pin_echo;
  pinMode(pin_trigger, OUTPUT);
  _pin_trigger = pin_trigger;
}

int vLib::ReadDistanceUltrasonic()
{
  digitalWrite(_pin_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pin_trigger, LOW);
  _value_distance_ultrasonic = pulseIn(_pin_echo, HIGH);
  _value_distance_ultrasonic_cm = _value_distance_ultrasonic/58;  
  return _value_distance_ultrasonic_cm;
}

void vLib::InitializeDistancePing(int pin_ping)
{
  _pin_ping = pin_ping;
}

int vLib::ReadDistancePing()
{
  pinMode(_pin_ping, OUTPUT);

  digitalWrite(_pin_ping, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin_ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(_pin_ping, LOW);
  
  pinMode(_pin_ping, INPUT);
  _value_distance_ping = pulseIn(_pin_ping,HIGH);
  _value_distance_ping_cm = _value_distance_ping/58;  
  return _value_distance_ping_cm;
}

// dht.h needs to be included
// class needs to be declared (e.g. dht DHT)
void vLib::InitializeHumidityDHT(int pin_humidity)
{
  _pin_humidity = pin_humidity;
}

int vLib::ReadHumidityDHT()
{
  DHT.read11(_pin_humidity);
  _value_humidity_dht_percent = DHT.humidity;
  return _value_humidity_dht_percent;
}

int vLib::ReadTemperatureDHT()
{
  DHT.read11(_pin_humidity);
  _value_temperature_dht_celsius = DHT.temperature;
  return _value_temperature_dht_celsius;
}
