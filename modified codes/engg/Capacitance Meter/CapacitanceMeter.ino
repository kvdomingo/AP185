 /*
*********************************************************************************************************
*
* Author: Christian Valgomera
* Hardware: VISSER handheld    
* Module: Capacitance Meter
* Date: Feb 26 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

#include <vLib.h>
vLib vlib;

#define analogPin      0          // analog pin for measuring capacitor voltage
#define chargePin      6         // pin to charge the capacitor - connected to one end of the charging resistor
#define dischargePin   7         // pin to discharge the capacitor
#define resistorValue  9910.0F   // change this to whatever resistor value you are using
                                  // F formatter tells compliler it's a floating point value
int red = 17;
int green = 16;
int button = 3;

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                // floating point variable to preserve precision, make calculations
float nanoFarads;


void setup(){
  pinMode(chargePin, OUTPUT);     // set chargePin to output
  digitalWrite(chargePin, LOW);  
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);             // initialize serial transmission for debugging

  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x85); 
  vlib.lcdPrint("VISSER");
  vlib.lcdCmd(0xC2);
  vlib.lcdPrint("Capacitance");
  delay(2000);

proceedButton();
}

void loop(){
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
  digitalWrite(chargePin, HIGH);  // set chargePin HIGH and capacitor charging
  startTime = millis();
//Serial.println("AA");

  int  raw = analogRead(A0);
  float capacitance = 0.0113*exp(0.0085*raw);
  float capacitanceMeter = capacitance*1000;

  vlib.lcdCmd(0x80); 
  vlib.lcdPrint("Capacitance:");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint(capacitanceMeter);
  vlib.lcdPrint(" nF");
  Serial.print(capacitanceMeter);
  Serial.println(" nF");
  delay(1000);
  
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
