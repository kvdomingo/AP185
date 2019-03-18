 /*
*********************************************************************************************************
*
* Author: Krista Melgarejo
* Hardware: VISSER handheld    
* Module: Temperature-Relative Humidity Box
* Date: Feb 26 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

//Libraries
#include <DHT.h>
#include <vLib.h>
vLib vlib;

//Constants
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int red = 17;
int green = 16;
int button = 3;

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(9600);

  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x85); 
  vlib.lcdPrint("VISSER");
  vlib.lcdCmd(0xC1);
  vlib.lcdPrint("Temp & Rel Hum");
  delay(2000);
  dht.begin();

proceedButton();
}

void loop()
{
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();

    vlib.lcdCmd(0x80);
    vlib.lcdPrint("Temp:");
    vlib.lcdPrint(temp);
    vlib.lcdPrint("C                   ");

    vlib.lcdCmd(0xC0);
    vlib.lcdPrint("Rel Hum:");
    vlib.lcdPrint(hum);
    vlib.lcdPrint("%           ");

    delay(2000); //Delay 2 sec.

}

void proceedButton(){
  clearScreen();
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Press the button");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to proceed.");
  delay(500);
  while(digitalRead(button)==HIGH);
  clearScreen();
}

void clearScreen() {
  // clear display
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("                ");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("                ");
}

void writeFirstLine(char* line) {
  vlib.lcdCmd(0x80);
  vlib.lcdPrint(line);
}

void writeSecondLine(char* line) {
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint(line);
}
