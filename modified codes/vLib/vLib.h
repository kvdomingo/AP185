#ifndef vLib_h
#define vLib_h

#include "Arduino.h"
#include "dht.h"

// Digital/Analog Port Reference:
#define VISSER_D1  4
#define VISSER_D2  5
#define VISSER_D3  6
#define VISSER_D4  7
#define VISSER_A1  A0
#define VISSER_A2  A1
#define VISSER_A3  A2
#define VISSER_A4  A3

class vLib
{
  public:
    vLib();
    void lcdWriteNibble(unsigned char d);
    void lcdInitialize(int lcdData, int lcdClk);
    void lcdCmd(unsigned char cbyte);
    void lcdData(unsigned char dbyte);
    void lcdPrint(const char * message);
    void lcdPrint(float message_float);
    void lcdPrint(unsigned long message_long);
    void lcdPrint(int message_int);
    void lcdPrint(uint8_t message_uint);
    void lcdClear();
    void lcdGoToFirstLine();
    void lcdGoToSecondLine();
    void lcdSetCursor(int row, int column);

    void InitializeTemperature(int pin_temperature);
    float ReadTemperature();
    void InitializeDistanceUltrasonic(int pin_echo, int pin_trig);
    int ReadDistanceUltrasonic();
    void InitializeDistancePing(int pin_ping);
    int ReadDistancePing();
    void InitializeHumidityDHT(int pin_humidity);
    int ReadHumidityDHT();
    int ReadTemperatureDHT();

  private:
    int _RS;
    int _lcdData;
    int _lcdClk;
    int _i;
    char _string[20];
    int _temp;
    int _pin_temperature;
    float _value_temperature;
    float _value_temperature_celsius;
    int _pin_echo;
    int _pin_trigger;
    int _value_distance_ultrasonic;
    int _value_distance_ultrasonic_cm;
    int _pin_ping;
    int  _value_distance_ping;
    int _value_distance_ping_cm;
    int _pin_humidity;
    int _value_humidity_dht_percent;
    int _value_temperature_dht_celsius;
    

};

#endif


