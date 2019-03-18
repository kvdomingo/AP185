#include <vLib.h>
vLib vlib;
int counter = 0;
int button = 3;
const int red =  17;      // the number of the red LED pin
const int green =  16;      // the number of the green LED pin


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

void setup(){
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);  //Start the serial connection with the copmuter
                       //to view the result open the serial monitor 
                       //last button beneath the file bar (looks like a box with an antenae)
  digitalWrite(red,HIGH);
  vlib.lcdInitialize(9,8); // 9 - DATA; 8 - CLOCK 
  writeFirstLine("     VISSER");
  writeSecondLine("  Conductivity");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("Put conductivity");
  writeSecondLine("probe on A1.");
  delay(5000);
  clearScreen();
  startButton();
}

void loop(){
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    
  double x[50];
  int i;
  for (i=0; i<50; i++){
    x[i] = 1024-analogRead(A0);
    x[i] = (x[i]*5)/1024;
    delay(100);
  }
  float xave=0.0;
  for (i=0; i<50; i++){
    xave=(x[i] + (xave*i))/(i+1);
  }
  float conductivity = 39.715*(exp(1.859*xave));
  vlib.lcdCmd(0x80); // Prints in the first line
  vlib.lcdPrint("Conductivity:     ");  
  vlib.lcdCmd(0xC0); // Prints in the second line  
  vlib.lcdPrint(conductivity); // Sensor 1
  vlib.lcdPrint("uS/cm     ");
  delay(5000);

  }

