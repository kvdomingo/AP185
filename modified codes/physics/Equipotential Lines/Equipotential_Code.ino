/*
*********************************************************************************************************

  Author: Anna Pamela O. De Jesus
  Hardware: VISSER handheld
  Module: Equipotential Lines
  Date: Feb 28 2018
  Company: VISSER, National Institute of Physics, University of the Philippines Diliman

*********************************************************************************************************
*/

#include <vLib.h>
vLib vlib; 
int vin = 0; //initial value
int vref = 0; //initial value
const int ledpin = 6;
const int button = 3;
const int power = 4;
const int red = 17;
const int green = 16;
int buttonState = 0;


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

void startButton(){
  clearScreen();
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Press the button");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to start.");
  delay(500);
  while(digitalRead(button)==HIGH);
  clearScreen();
}
void endButton(){
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
  clearScreen();
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("   Experiment");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("     done.");
  delay(5000);
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

void setup() {
  Serial.begin(9600); //serial monitor for testing
  pinMode(ledpin, OUTPUT);
  pinMode(button, INPUT);
  pinMode(power, OUTPUT);

  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9,8); // 9 - DATA; 8 - CLOCK 
  writeFirstLine("     VISSER");
  writeSecondLine("  Equipotential Lines");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("Put Hall effect");
  writeSecondLine("probe on A2.");
  delay(5000);
  clearScreen();
  startButton();
}

// the loop function runs over and over again forever
void loop() {
  //digitalWrite(power, HIGH);
  //buttonState = digitalRead(button);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);

  vin = analogRead(A4);
  //Serial.print("\n");
  vlib.lcdPrint("\n");
  if (buttonState == HIGH) {
    vref = vin;
  }

  if ((vin - vref) > 0.001) {
    digitalWrite(ledpin, LOW);
  } else if ((vin - vref) < 0.001 & (vin - vref) > -0.001) {
    digitalWrite(ledpin, HIGH);
  }
  
  float out_vref = (5.0 * vref) / 1024.0;
  vlib.lcdCmd(0x80); // Prints in the first line
  vlib.lcdPrint("Magnetic field: ");
  vlib.lcdCmd(0xC0); // Prints in the second line  
  vlib.lcdPrint(out_vref);
  vlib.lcdPrint("            ");
  //vlib.lcdCmd(0x80); // Prints in the first line
  //vlib.lcdPrint("Movable Probe:  ");
  //vlib.lcdCmd(0xC0); // Prints in the second line  
  //vlib.lcdPrint(out_vin);
  delay(2000);
}
