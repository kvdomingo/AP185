/*
Tested with HY-SRF05, HC-SR04
Assuming a room temp of 20 degrees centigrade
The circuit:
	* VVC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 12
        * ECHO connection of the sensor attached to digital pin 13
*/
#include <vLib.h>
vLib vlib;
#include<SPI.h>
#include<LiquidCrystal.h>
#include <SD.h>
#include <string.h>

// Initialize the libra ry with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);
// this constant won't change.  It's the pin number
// of the sensor's output:
int button = 3;
const int red =  17;      // the number of the LED pin
const int green =  16;      // the number of the LED pin
const int chipSelect = 10;
 

const int TRIG_PIN = 5;
const int ECHO_PIN = 7;

unsigned long time;
 
void setup() {
  // initialize serial communication:
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  writeFirstLine("     VISSER");
  writeSecondLine("      Light");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("PING on A2");
  writeSecondLine("ECHO on A4");
  delay(5000);
  clearScreen();
  proceedButton();
  writeFirstLine("Light sensor");
  writeSecondLine("on A1.");
  delay(5000);
  clearScreen();
  proceedButton();
  writeFirstLine("Light source");
  writeSecondLine("on A2.");
  delay(5000);
  clearScreen();
  startButton();

  
  pinMode(10, OUTPUT);
 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}
 
void loop()
{
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  vlib.lcdCmd(0x80);
  
  double ave = 0.0;
//  double ave1 =0.0;
//  double ave2 = 0.0;
  int i = 0;
  int sensorf, sensorf1, sensorf2;
  int sensor[10];
//  int sensor1[5];
//  int sensor2[5];

//take five (5) readings from A0 A1 and A2 and get average for data processing  
for (i=0; i<10 ; i++){

  sensor[i] = analogRead(0); 
//  sensor1[i] = analogRead(1); 
//  sensor2[i] = analogRead(2);  
}
 sensorf = sensor[0] + sensor[1] + sensor[2] +sensor[3] + sensor[4];
 ave = sensorf/10;
// sensorf1 = sensor1[0] + sensor1[1] + sensor1[2] +sensor1[3] + sensor1[4];
// ave1 = sensorf1/5;
// sensorf2 = sensor2[0] + sensor2[1] + sensor2[2] +sensor2[3] + sensor2[4];
// ave2 = sensorf2/5;
// ldr(ave,ave1,ave2);
   ldr(ave);
  }

//convert ADC value to light intensity
void ldr(int sensor) { 
  double volt = sensor * (5.0/1024.0);
//  double volt1 = sensor1 * (5.0/1024.0);
//  double volt2 = sensor2 * (5.0/1024.0);
  
  double res = (10.0 * 5.0) / volt - 10.0;
//  double res1 = (10.0 * 5.0) / volt1 - 10.0;
//  double res2 = (10.0 * 5.0) / volt2 - 10.0; 
  
// Calculating the intensity of light in lux and display in LCd and serial
  float lx = 255.84 * pow(res, -10/9);
//  double lx1 = 255.84 * pow(res1, -10/9);
//  double lx2 = 255.84 * pow(res2, -10/9); 
  vlib.lcdPrint("Ev=");
  Serial.println("I=");
  
  char lxs[5];
  
  
  floatToString(lxs, lx, 2, 2);
  vlib.lcdPrint(lxs);
  Serial.println(lx);
  vlib.lcdPrint(" lux         ");
  vlib.lcdPrint(" ");
   vlib.lcdCmd(0x80);
//  lcd.print("B=");
//  lcd.print(lx1); 
//  lcd.print(" ");
//  lcd.setCursor(0,1);
//  lcd.print("C=");
//  lcd.print(lx2);
//  lcd.print(" ");
  //delay(1000);
  time = millis();
  time=time/1000;
   double duration;
   float distanceCm, distanceIn;
 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;
 
  if (distanceCm <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distanceIn);
    
    Serial.print("in, ");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
    
    vlib.lcdCmd(0xC0);
    vlib.lcdPrint("d:");
    char cms[5];
    floatToString(cms, distanceCm, 1, 3);
    vlib.lcdPrint(cms);
    vlib.lcdPrint(" cm  ");
  }
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

