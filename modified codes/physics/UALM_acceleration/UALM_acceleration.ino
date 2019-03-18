#include <SPI.h>


/* Ping))) Sensor
  
   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
    * +V connection of the PING))) attached to +5V
    * GND connection of the PING))) attached to ground
    * SIG connection of the PING))) attached to digital pin 7

   http://www.arduino.cc/en/Tutorial/Ping
   
   created 3 Nov 2008
   by David A. Mellis
   modified 30 Aug 2011
   by Tom Igoe
 
   This example code is in the public domain.

 */
#include <vLib.h>
vLib vlib;
#include<LiquidCrystal.h>
#include <SD.h>
#include <string.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 2, 4, 6);
// this constant won't change.  It's the pin number
// of the sensor's output:
int button = 3;
const int red =  17;      // the number of the LED pin
const int green =  16;      // the number of the LED pin
const int pingPin = 5;

const int chipSelect = 10;




void setup() {
  // initialize serial communication:
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  writeFirstLine("     VISSER");
  writeSecondLine("     UALM-A");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("Insert SD card");
  delay(5000);
  clearScreen();
  proceedButton();
  writeFirstLine("Motion sensor");
  writeSecondLine("on A1.");
  delay(5000);
  clearScreen();
  startButton();

  
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  
  pinMode(10, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  
  Serial.println("card initialized.");
  long t0=-1;
  long t=0;
  float v0=0;
  float v=0;
  float a=0;
  float cm0=0;
}

void loop()
{
  long t0=-1;
  long t=0;
  float v0=0;
  float v=0;
  float a=0;
  float cm0=0;
  
  while(1){
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches;
  float cm;
  

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(400);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  

  // convert the time into a distance
  // inches = microsecondsToInches(duration);
  cm0=cm;
  cm = microsecondsToCentimeters(duration);
  t0=t;
  t = millis();
  v0=v;
  v=(cm-cm0)*(10)/(t-t0);
  a=(v-v0)*1000/(t-t0);
  
  lcd.setCursor(0,0);
  lcd.print("Distance:           ");
  lcd.setCursor(0,1);
  lcd.print(cm);
  lcd.print(" cm        ");
  //Serial.println("Distance:");
  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.println(" cm");
  //Serial.println();
  //Serial.println();
  
 //Serial.println("Velocity:");
 //Serial.println(v);
 //Serial.println("Acceleration:");
 Serial.print(a); 
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // make a string for assembling the data to log:
  String dataString = "";
  // if the file is available, write to it:
  char vs[20];
  floatToString(vs, v, 3, 3);
  char as[20];
  floatToString(as, a, 3, 3);
  char cms[20];
  floatToString(cms, cm, 3, 3);

  dataString+= t;
  dataString+=",";
  
  dataString+=cms;
  dataString+=",";
  
  dataString+=vs;
  dataString+=",";
  
  dataString+=as;
  dataString+=",";
  
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
  delay(10);
  }
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

float microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (1.0*microseconds / 29.0) * 0.5;
}

char * floatToString(char * outstr, double val, byte precision, byte widthp){
  char temp[16]; //increase this if you need more digits than 15
  byte i;

  temp[0]='\0';
  outstr[0]='\0';

  if(val < 0.0){
    strcpy(outstr,"-\0");  //print "-" sign
    val *= -1;
  }

  if( precision == 0) {
    strcat(outstr, ltoa(round(val),temp,10));  //prints the int part
  }
  else {
    unsigned long frac, mult = 1;
    byte padding = precision-1;
    
    while (precision--)
      mult *= 10;

    val += 0.5/(float)mult;      // compute rounding factor
    
    strcat(outstr, ltoa(floor(val),temp,10));  //prints the integer part without rounding
    strcat(outstr, ".\0"); // print the decimal point

    frac = (val - floor(val)) * mult;

    unsigned long frac1 = frac;

    while(frac1 /= 10) 
      padding--;

    while(padding--) 
      strcat(outstr,"0\0");    // print padding zeros

    strcat(outstr,ltoa(frac,temp,10));  // print fraction part
  }

  // generate width space padding 
  if ((widthp != 0)&&(widthp >= strlen(outstr))){
    byte J=0;
    J = widthp - strlen(outstr);

    for (i=0; i< J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp,outstr);
    strcpy(outstr,temp);
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
