/*
*********************************************************************************************************

  Author: Anna Pamela O. De Jesus
  Hardware: VISSER handheld
  Module: Charge and Discharge of Capacitors
  Date: Feb 28 2018
  Company: VISSER, National Institute of Physics, University of the Philippines Diliman

*********************************************************************************************************
*/

//VISSER program for charging and discharging of capacitor
// data can be stored in sd card
#include <SPI.h>
#include <vLib.h>
vLib vlib;
#include <SD.h>
#include <stdio.h>
//#include<LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int red = 17;
const int green = 16;
int button = 3;
const int chipSelect = 10; 

int suffix=1;
char fname[15]="CAP",filename[15]="CAP001.csv";
float volts=0.0;             //sensor reading
unsigned long ttime=0;                //elapsed time
String dataString = "";

int sensorValue = 0;    // value read from the pot
long int start;

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
  vlib.lcdInitialize(9, 8);
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
   
  //pinMode(10, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9,8); // 9 - DATA; 8 - CLOCK 
  writeFirstLine("     VISSER");
  writeSecondLine("   Capacitors");
  delay(3000);
  clearScreen();
  proceedButton();
  clearScreen();
  initSDCard();  
  initvLibDisplay();
  writeFirstLine("Put voltmeter");
  writeSecondLine("on A1 and A2.");
  delay(5000);
  clearScreen();
  proceedButton();
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Connect pin");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to charge.");
  delay(5000);
  clearScreen();
  startButton();
  
  
  //lcd.begin(16,2);
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Charging and");
  //lcd.setCursor(0,1);
  //lcd.print("Discharging of C");
  //delay(5000);
  
}
void initSDCard()
{
  Serial.print("");
  Serial.println("Charging and Discharging Capacitor");
  Serial.println("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
   if (!SD.begin(chipSelect)) {
     Serial.println("Card failed, or not present");
     // don't do anything more:
   }
   else {
   Serial.println("Card initialized.");
   }
   delay(2000);
}
void initvLibDisplay()
{
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Charging and");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("Discharging of C"); 
  clearScreen();
}

void labelData()
{
  while (SD.exists(filename)) {   
      Serial.print(filename);
      Serial.println(" exists");
      snprintf(filename, 12, "%s%03d.csv", fname,++suffix);
  }
  Serial.print("Saving data to ");
  Serial.println(filename);
  File dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("TIME");
    dataFile.print(",");
    dataFile.println("VOLTAGE");    
    dataFile.close();
  }
    //if the file isn't open, pop up an error:
  else {
    Serial.print("Error opening ");
    Serial.println(filename);
  } 
}

void logData(){
  File dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.print(dataString);
    dataFile.println(volts, 5);
    dataFile.close();
  }  
}   

void lcdDisplayData()
{
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("T(ms): ");
  vlib.lcdCmd(0x87);
  //lcd.setCursor(7,0);
  char ttimes[20];
  floatToString(ttimes, ttime, 7, 1);
  vlib.lcdPrint(ttimes);
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("Vc(volts): ");
  //lcd.setCursor(10,1);
  vlib.lcdCmd(0xCA);
  char voltss[5];
  floatToString(voltss, volts, 2, 2);
  vlib.lcdPrint(voltss);
}



void loop() {
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
   //lcd.clear();
  //lcd.setCursor(0,1);

//  digitalWrite(red, LOW);
//  digitalWrite(green, HIGH);
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Data is being");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("collected...  ");
  lcdDisplayData();
  delay(2000);
  //clearScreen();
  //lcd.clear();
  start=millis();
  while (1) {
  sensorValue = analogRead(analogInPin);
  volts=(sensorValue*5.0)/(1023.0);
  ttime= (millis()-start);
  Serial.print(ttime);
  Serial.print("\t");  
  Serial.println(volts,5); 
  dataString=String(ttime)+",";
  lcdDisplayData();
  logData();
  delay(20);
  }
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


