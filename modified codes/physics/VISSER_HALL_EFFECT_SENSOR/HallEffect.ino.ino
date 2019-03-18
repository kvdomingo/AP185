 /*
*********************************************************************************************************
*
* Author: Hazel Sales
* Hardware: VISSER handheld    
* Module: Hall effect Sensor
* Date: Feb 25 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/
 

#include <vLib.h>

vLib vlib;


int red = 17;
int green = 16; 
const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to
int n=10;                     //10 samples

int sensorValue1 = 0;         //preset value
int button = 3;
long int start;
float volts=0.0;
float tesla=0.0;
long ttime=0;
float diff=0.01;


void setup() {
  pinMode(button,INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(red, HIGH);
  vlib.lcdInitialize(9, 8);   // 9 - DATA; 8 - CLOCK
  writeFirstLine("     VISSER");
  writeSecondLine(" Magnetic Field");
  
  delay(2000);
  clearScreen();
  startButton(); 
}

void loop() {
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
  sensorValue1=0.0;
  int readTesla;
  const float b = -33.139;
  
  for (int i=0;i<n;i++) {
     readTesla = analogRead(analogInPin1);
     sensorValue1 = sensorValue1 + readTesla;
  }

  sensorValue1 = sensorValue1/n;
  tesla = (readTesla*0.062) + b;

  char tes[20];
  floatToString(tes,tesla, 3, 4);
  writeFirstLine("B Field(mT):   ");
  writeSecondLine(tes);
  
  delay(1000);
}
  
char * floatToString(char * outstr, double val, byte precision, byte widthp) {
  char temp[16]; //increase this if you need more digits than 15
  byte i;

  temp[0] = '\0';
  outstr[0] = '\0';

  if (val < 0.0) {
    strcpy(outstr, "-\0"); //print "-" sign
    val *= -1;
  }

  if ( precision == 0) {
    strcat(outstr, ltoa(round(val), temp, 10)); //prints the int part
  }
  else {
    unsigned long frac, mult = 1;
    byte padding = precision - 1;

    while (precision--)
      mult *= 10;

    val += 0.5 / (float)mult;    // compute rounding factor

    strcat(outstr, ltoa(floor(val), temp, 10)); //prints the integer part without rounding
    strcat(outstr, ".\0"); // print the decimal point

    frac = (val - floor(val)) * mult;

    unsigned long frac1 = frac;

    while (frac1 /= 10)
      padding--;

    while (padding--)
      strcat(outstr, "0\0");   // print padding zeros

    strcat(outstr, ltoa(frac, temp, 10)); // print fraction part
  }

  // generate width space padding
  if ((widthp != 0) && (widthp >= strlen(outstr))) {
    byte J = 0;
    J = widthp - strlen(outstr);

    for (i = 0; i < J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp, outstr);
    strcpy(outstr, temp);
  }

  return outstr;
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

