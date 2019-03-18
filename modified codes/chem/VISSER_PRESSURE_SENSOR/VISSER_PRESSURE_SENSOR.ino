/* SFE_BMP180 altitude example sketch

This sketch shows how to use the Bosch BMP180 pressure sensor
as an altimiter.
https://www.sparkfun.com/products/11824

Like most pressure sensors, the BMP180 measures absolute pressure.
Since absolute pressure varies with altitude, you can use the pressure
to determine your altitude.

Because pressure also varies with weather, you must first take a pressure
reading at a known baseline altitude. Then you can measure variations
from that pressure

Hardware connections:

- (GND) to GND
+ (VDD) to 3.3V

(WARNING: do not connect + to 5V or the sensor will be damaged!)

You will also need to connect the I2C pins (SCL and SDA) to your
Arduino. The pins are different on different Arduinos:

Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3

Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the BMP180 to systems with lower logic levels such as 1.8V

Have fun! -Your friends at SparkFun.

The SFE_BMP180 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/

Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.

V10 Mike Grusin, SparkFun Electronics 10/24/2013
*/

// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_BMP180.h>
#include <Wire.h>

#include <SPI.h>

#include <vLib.h>
vLib vlib;

SFE_BMP180 pressure;
int button = 3;
int pressSensor_existing = 0;
double baseline; // baseline pressure
const int red =  17;      // the number of the LED pin
const int green =  16;      // the number of the LED pin
// variables will change:

// You will need to create an SFE_BMP180 object, here called "pressure":
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

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
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



void printTemp(){
  int sensor1[50];
  int sensor2[50];


  double ave;
  double ave2;
  
int i=0;
  //take five (5) readings from A0 A1 and A2 and get average for data processing
  for (i = 0; i < 50 ; i++) {
    sensor1[i] = analogRead(0);
    sensor2[i] = analogRead(1);
  }
  ave = 0.0;
  ave2 = 0.0;

  for (i = 0; i < 50; i++) {
    ave = ((i * ave) + sensor1[i]) / (i + 1);
    ave2 = ((i * ave2) + sensor2[i]) / (i + 1);
  }

if (ave<200.1){
  //  int temp = ave * 0.09765625; // Convert to C
  double temp1 = (ave * 5.0 * 100.0) / 1024;
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("t1:");
  char cms[20];
  floatToString(cms, temp1, 1, 3);
  vlib.lcdPrint(cms);
  vlib.lcdPrint(" C");
  
}
else{
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("        ");
  
}
if (ave2<200.1){
  vlib.lcdCmd(0xC8);
  //  int temp = ave * 0.09765625; // Convert to C
  double temp1 = (ave2 * 5.0 * 100.0) / 1024;
  vlib.lcdPrint("t2:");
  char cms[20];
  floatToString(cms, temp1, 1, 3);
  vlib.lcdPrint(cms);
  vlib.lcdPrint("C");
  
}


else{
  vlib.lcdCmd(0xC8);
  vlib.lcdPrint("        ");
  
}
}


void setup()
{
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  digitalWrite(red,HIGH);
  writeFirstLine("     VISSER");
  writeSecondLine("    Pressure");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("SDA on A3");
  writeSecondLine("SCL on A4");
  delay(5000);
  clearScreen();
  startButton();

  // Initialize the sensor (it is important to get calibration values stored on the device).
  if (pressure.begin()){
    pressSensor_existing = 1;
  }
  else
  {

  }
  
  // Get the baseline pressure:
  baseline = getPressure();


}

void loop(){     
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    double a,P;
    if (pressSensor_existing==1){
    // Get a new pressure reading:
    P = getPressure();
    vlib.lcdCmd(0x80); 
    vlib.lcdPrint("Pressure:");
    char pressures[7];
    floatToString(pressures, P/10, 3, 3);
    vlib.lcdCmd(0xC0);
    vlib.lcdPrint(pressures);
    vlib.lcdPrint(" kPa       ");
    delay(5000);
    }
  }
