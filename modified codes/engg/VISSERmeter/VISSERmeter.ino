 /*
*********************************************************************************************************
*
* Author: Archie Veloria
* Hardware: VISSER handheld    
* Module: VISSERmeter
* Date: Feb 26 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

#include <vLib.h>
#include <math.h>
vLib vlib;

float v1=0;
float v2=0;
float v3=0;
float v4=0;
float vout, R2, buff;
float R1 = 10000.00;  // resistor value of R1 in the voltage divider circuit for the voltmeter 
float Rs = 1;  // burden resistor for the ammeter = 1 ohm
float vin = 5;  // 5 volts from Arduino
int roffset = 0;
int button = 3;

void setup(){
  pinMode(button,INPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  
  Serial.begin(9600);
  vlib.lcdInitialize(9,8);

  writeFirstLine("     VISSER");
  writeSecondLine("   Multimeter");    
  delay(2000);
  clearScreen();

  writeFirstLine("Use A1&A2 for");
  writeSecondLine("VISSER Voltmeter");
  delay(2000);
  clearScreen();
  proceedButton();

  writeFirstLine("Use A1 for");
  writeSecondLine("VISSER Ohmmeter");
  delay(2000);
  clearScreen();
  proceedButton();

  writeFirstLine("Use A3&A4 for");
  writeSecondLine("VISSER Ammeter");
  delay(2000);
  clearScreen();
  proceedButton();

  writeFirstLine("VISSERmeter is");
  writeSecondLine("Ready!");
  delay(2000);
  clearScreen();
}

void loop(){
  //Connect voltmeter probes to A0 & A1 port
  //Connect Ohmmeter probe to A0 port
  //Connect ammeter probes to A4 & A5 port
  
  //Gets voltage data for 5 seconds, then the average of the data
  int count = 0;
  while (count < 10) {
    v1 = v1 + (analogRead(A0)*vin)/float(1024);  // v1 = higher potential side
    v2 = v2 + (analogRead(A1)*vin)/float(1024);  // v2 = lower potential side
    v3 = v3 + (analogRead(A4)*vin)/float(1024);
    v4 = v4 + (analogRead(A5)*vin)/float(1024);
    count++;
    delay(500);
  }
  
  v1 = v1/float(10);
  vout = v1;           // for the ohmmeter; vout = voltage across the measured resistor 
  v2 = v2/float(10);
  v3 = v3/float(10);
  v4 = v4/float(10);

  //Ohmmeter
  //Voltage division: Vout = Vin(R2/(R1+R2))
  buff = (vin/vout) - 1.00;
  R2 = R1/buff;
  
  seriallcd();  // prints the units at the first line using vlib
  
  //Ohms
  vlib.lcdCmd(0xC0); // Prints in the second line
  //vlib.lcdPrint(abs(int(R2)));
  //vlib.lcdPrint("  ");
  // resistance value simplification
  char r2s[5];
  if (R2 >= 1000000) {
    floatToString(r2s, float(((abs(R2)))/float(1000000)), 2, 2);
    vlib.lcdPrint(r2s);
    vlib.lcdPrint(" M");
  } else if (R2 >= 99999) {
    floatToString(r2s, float(((abs(R2)))/float(1000)), 2, 2);
    vlib.lcdPrint(" k ");
  } else if (R2 >= 1000) {
    floatToString(r2s, float(((abs(R2)))/float(1000)), 2, 2);
    vlib.lcdPrint(r2s);
    vlib.lcdPrint ("k ");
  } else {
    floatToString(r2s, float(abs(R2)), 2, 2);
    vlib.lcdPrint(r2s);
  }
  
  //Voltmeter
  //Potential difference between two points v1 & v2
  vlib.lcdCmd(0xC6); // Prints in the second line
  vlib.lcdPrint((v1-v2));  // voltage across
  
  //Amperes
  //Gets the potential differemce across the burden resistor Rs 
  //then divide it by the voltage value
  //to obtain the current through the circuit
  vlib.lcdCmd(0xCC); // Prints in the second line
  vlib.lcdPrint((-1)*(v3-v4)*1000/Rs); //milliamperes

  v1=v2=v3=v4=0;
  delay(1000);
}

void seriallcd(){
  vlib.lcdCmd(0x80); // Prints in the first line  
  vlib.lcdPrint("OHM:");
  vlib.lcdCmd(0x86); // Prints in the second line
  vlib.lcdPrint("VOLT:");
  vlib.lcdCmd(0x8C);
  vlib.lcdPrint("mA:");
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
