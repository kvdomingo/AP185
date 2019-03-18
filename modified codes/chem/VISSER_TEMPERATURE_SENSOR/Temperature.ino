 /*
*********************************************************************************************************
*
* Author: Dwight Angelo Bruzon
* Hardware: VISSER handheld    
* Module: Temperature Sensor
* Date: Feb 24 2018
* Company: VISSER, National Institute of Physics, University of the Philippines Diliman
*
*********************************************************************************************************
*/

// Initialize the library with the numbers of the interface pins
#include <vLib.h>

vLib vlib;

int red = 17;
int green = 16; 
int button = 3;

int sensorValue1 = 0;         //preset value
//int button = 3;
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
  writeSecondLine("  Temperature");
  
  delay(2000);
  clearScreen();
  startButton(); 
}

void loop() {
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  int sensorf, sensorf1, sensorf2, i;
  int sensor1[50];
  int sensor2[50];
  int sensor3[50];
  int sensor4[50];


  double ave, ave1, ave2, ave3;

  //lcd.setCursor(0,0);
  //time = millis();
  //time = time / 1000;
  //Serial.println(time);

  //take five (5) readings from A0 A1 and A2 and get average for data processing
  for (i = 0; i < 50 ; i++) {


    sensor1[i] = analogRead(A0);
    sensor2[i] = analogRead(A1);
    sensor3[i] = analogRead(A4);
    sensor4[i] = analogRead(A5);
  }
  ave = 0.0;
  ave1 = 0.0;
  ave2 = 0.0;
  ave3 = 0.0;

  for (i = 0; i < 50; i++) {
    ave = ((i * ave) + sensor1[i]) / (i + 1);
    ave1 = ((i * ave1) + sensor2[i]) / (i + 1);
    ave2 = ((i * ave2) + sensor3[i]) / (i + 1);
    ave3 = ((i * ave3) + sensor4[i]) / (i + 1);
  }

  //  int temp = ave * 0.09765625; // Convert to C
  double temp1 = (ave * 5.0 * 100.0) / 1024;
  //  lcd.print("temp: ");
  //  lcd.print(temp);
  Serial.print(temp1);
  Serial.print("\t");
  //  int temp1 = ave1;
  // float temp1 = ((((ave1 * 5 )/1024)-0.881)*100)-0.6;
  //  Serial.println(temp1);
  //  lcd.print("temp: ");
  //  lcd.print(temp1);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("t1:");
  char cms[20];
  floatToString(cms, temp1, 1, 3);
  vlib.lcdPrint(cms);
  //vlib.lcdPrint("C");

  vlib.lcdPrint(" t2:");

  double temp2 = (ave1 * 5.0 * 100.0) / 1024;
  floatToString(cms, temp2, 1, 3);
  vlib.lcdPrint(cms);
  //vlib.lcdPrint("C");
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("t3:");
  double temp3 = (ave2 * 5.0 * 100.0) / 1024;
  floatToString(cms, temp3, 1, 3);
  vlib.lcdPrint(cms);
  //vlib.lcdPrint("C");
  vlib.lcdPrint(" t4:");

  double temp4 = (ave3 * 5.0 * 100.0) / 1024;
  floatToString(cms, temp4, 1, 3);
  vlib.lcdPrint(cms);
  //vlib.lcdPrint("C");


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

