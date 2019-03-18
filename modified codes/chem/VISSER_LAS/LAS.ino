
 /*
*********************************************************************************************************
*
* Author: Christian Valgomera
* Hardware: VISSER handheld    
* Module: Light Absorption and Scattering (LAS) Setup
* Date: Feb 28 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

#include <vLib.h>
vLib vlib;

int sensor = 0;
float ave = 0;

int red = 17;
int green = 16;
int button = 3;

void setup (){
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(9600);

  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9,8);
  vlib.lcdCmd(0x84); // Prints in the first line
  vlib.lcdPrint("VISSER");
  vlib.lcdCmd(0xC5); // Prints in the second line; C0 to CF
  vlib.lcdPrint("LAS");
  
  delay(2000);
  clearScreen();
  startButton();

}

void loop () {
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  
  for (int i = 0; i<50; i++){
    int readings = analogRead(A0);
    ave += readings;
  }
  ave /= 50.0;
  sensor = map(int(ave), 0, 1023, 0, 255);
  sensor = constrain(int(ave), 255, 0);
  Serial.println(sensor); 
  
  vlib.lcdCmd(0x80); // Prints in the first line
  vlib.lcdPrint("Rel Intensity");
  vlib.lcdCmd(0xC0); // Prints in the first line
  vlib.lcdPrint(sensor);
  vlib.lcdPrint("            ");

  delay(1000);
  ave = 0;
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
  vlib.lcdPrint("  Experiment");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("     done.");
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

