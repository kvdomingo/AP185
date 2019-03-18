/*
*********************************************************************************************************
*
* Author: Archie Veloria
* Hardware: VISSER handheld    
* Module: pH Sensor
* Date: Feb 23 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/
 
#include <vLib.h>

vLib vlib;

float ph_4_voltage = 0.00;
float ph_7_voltage = 0.00;
float ph_10_voltage = 0.00;
float slope = 0;
int readPH = 0;
int button = 3;

void setup() {
  pinMode(button,INPUT);
  pinMode(A0,INPUT);
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  vlib.lcdInitialize(9,8);
  writeFirstLine("     VISSER");
  writeSecondLine("       pH");
    
  delay(2000);
  clearScreen();

  // calibrate device
  writeFirstLine("Calibrate pH");
  writeSecondLine("Probe");
  delay(2000);
  clearScreen();
  proceedButton();
  writeFirstLine("Probe on ph 4");
  writeSecondLine("Calibrating...");
  delay(10000);
  
  int x = 0;
  while (x < 60) {
    readPH = analogRead(A0);
    ph_4_voltage = ph_4_voltage + float(readPH);
    x = x + 1;
    delay(1000);
  }
  ph_4_voltage = (ph_4_voltage/60.00)/1024.00*5.00;

  clearScreen();
  writeFirstLine("Done!");
  delay(2000);
  proceedButton();
  writeFirstLine("Probe on ph 7");
  writeSecondLine("Calibrating...");
  delay(10000);
  
  x = 0;
  while (x < 60) {
    readPH = analogRead(A0);
    ph_7_voltage = ph_7_voltage + float(readPH);
    x = x + 1;
    delay(1000);
  }
  ph_7_voltage = (ph_7_voltage/60.00)/1024.00*5.00;

  // clear display
  clearScreen();
  writeFirstLine("Done!");
  delay(2000);
  proceedButton();
  writeFirstLine("Probe on ph 10");
  writeSecondLine("Calibrating...");
  delay(10000);
  
  x = 0;
  while (x < 60) {
    readPH = analogRead(A0);
    ph_10_voltage = ph_10_voltage + float(readPH);
    x = x + 1;
    delay(1000);
  }
  ph_10_voltage = (ph_10_voltage/60.00)/1024.00*5.00;
  slope = ((7.00-4.00)/(ph_7_voltage - ph_4_voltage) + (10.00-7.00)/(ph_10_voltage - ph_7_voltage))/2.00;

  clearScreen();

  // ready to measure
  writeFirstLine("Device Ready!");
  writeSecondLine("                ");
  delay(2000);
  proceedButton();
}

void loop() {
  int i = 0;
  float valueDO = 0;
  float valuePH = 0;
  
  while (i < 10) {
    readPH = analogRead(A0);

    if (i == 0) {
      valuePH = slope*(readPH/1024.00*5.00);
    }
    else {
      valuePH = valuePH + (slope*(readPH/1024.00*5.00));
    }
    i++;
    delay(500);
  }
  
  valuePH = valuePH / 10.00;
  char cms[20];
  floatToString(cms, valuePH, 1, 3);
  writeFirstLine("        pH");
  writeFirstLine(cms);
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
