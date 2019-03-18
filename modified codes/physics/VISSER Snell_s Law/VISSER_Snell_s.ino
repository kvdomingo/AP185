 /*
*********************************************************************************************************
*
* Author: Krista Melgarejo
* Hardware: VISSER handheld    
* Module: Snell's Law Kit
* Date: Feb 27 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

#include <vLib.h>
vLib vlib;

int red = 17;
int green = 16;
int button = 3;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
 
 pinMode(red, OUTPUT);
 pinMode(green, OUTPUT);
 Serial.begin(9600);

 digitalWrite(red, HIGH);
 vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
 vlib.lcdCmd(0x85);
 vlib.lcdPrint("VISSER");
 vlib.lcdCmd(0XC2);
 vlib.lcdPrint("Snell's Law");
 
 delay(2000);

 proceedButton();
 
}

// the loop routine runs over and over again forever:
void loop() {

  digitalWrite(green, HIGH);
  digitalWrite(red, LOW);
  
  // read the input on analog pin 0:
  int sensorValue = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  float deg = (voltage - 4.1986) / 0.0116;

     char v2s[8];
     floatToString(v2s, deg, 2, 4);
     vlib.lcdCmd(0x80);
     vlib.lcdPrint("Angle:");
     vlib.lcdPrint(deg); 
     vlib.lcdPrint(" Deg"); //change me to cm/s
     vlib.lcdCmd(0XC0);
     vlib.lcdPrint("                 ");
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

void proceedButton() {
  clearScreen();
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Press the button");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to proceed.");
  delay(500);
  while (digitalRead(button) == HIGH);
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
