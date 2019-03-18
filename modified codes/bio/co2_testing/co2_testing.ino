#include "stdlib.h"
#include <vLib.h>
vLib vlib;
#define FIRSTLINE     0x80
#define SECONDLINE    0xc0
#define PPM_VALE_ADDRESS 0X89
#define CLEAR_PPM     "       "

#pragma region LCD_code
/*------------------------------------------------------------------------------------------lcd-----------------------------------------------------------------------------*/
#define DATA 9
#define CLK  8
#define DELAYPERBIT delayMicroseconds(40) //us

void clk_write( byte data ){
  for(int i=0; i<8;i++)
  {
    digitalWrite(DATA, 0x01&(data>>i)) ;
    //activate CLK
    DELAYPERBIT;
    digitalWrite(CLK,HIGH);
    DELAYPERBIT;
    digitalWrite(CLK,LOW);
  }

  DELAYPERBIT;
  //activate EN
  digitalWrite(DATA, HIGH);
  DELAYPERBIT;
  //disable EN
  digitalWrite(DATA, LOW);
  DELAYPERBIT;
}

void LCD_W4bits(byte data, byte isChar ){  
  int RS_EN;
  data =( 0x40&(data<<6) )|
        ( 0x20&(data<<4) )|
        ( 0x10&(data<<2) )|
        ( 0x08&(data) );   //shift
  if(isChar>0) RS_EN=0x06;    //RS = 1, EN = 1
  else RS_EN=0x02;  //RS = 0, EN = 1
  data &= 0xfc;
  data |= RS_EN;
  clk_write(data);
  clk_write(0);
}

void LCD_W8bits(byte data, byte isChar ){ 
  LCD_W4bits(0x0f&(data>>4),isChar);
  LCD_W4bits(0x0f&data,isChar);
}

void LCD_StartUp(){  
  //4-BIT interface initialization sequence  
  delay(45);  
  LCD_W4bits(0x03,0);  
  delay(15);  
  LCD_W4bits(0x03,0);  
  delay(1);    
  LCD_W8bits(0x32,0);  
  LCD_W8bits(0x28,0);  
  LCD_W8bits(0x00,0);  
  LCD_W8bits(0x0c,0);  
  LCD_W8bits(0x00,0);  
  LCD_W8bits(0x01,0);  
  LCD_W8bits(0x06,0);
}  

void LCD_StringWrite(char* dataString, char dataCommand){
  char* pointer = dataString;
  LCD_W8bits(dataCommand,0);
  while(*pointer!='\0')
  LCD_W8bits(*pointer++,1);
}

void LCD_CommandWrite(char dataCommand){
  LCD_W8bits(dataCommand,0);
}

void LCDsetup(){
  pinMode( DATA ,OUTPUT );
  pinMode( CLK ,OUTPUT );
  digitalWrite(CLK,LOW);
  digitalWrite(DATA,LOW);
  LCD_StartUp();
}

/*----------------------------------------------------------------------------------------end of lcd----------------------------------------------------------------*/
#pragma endregion endofLibrary


int button = 3;
const int red =  17;      // the number of the LED pin
const int green =  16;      // the number of the LED pin



float calculated_ppm = 0;     /* the CO2 PPM */
float ms_accumulator = 0;     /* the PWM high signal delay (ms) counter */
char charPPM[7];              /*buffer for convertion of int to char*   */

#define INPUT_PIN A0

#define RESET_VALUES ms_accumulator=0;

#define DETECT_RISING_EDGE while(digitalRead(INPUT_PIN)!=HIGH);

#define DETECT_FALLING_EDGE while(digitalRead(INPUT_PIN)!=LOW);

#define CALCULATE_PPM calculated_ppm = (ms_accumulator-2.0)*2; if(calculated_ppm<0) calculated_ppm = 0;  /* from datasheet, DELAY in ms = (PPM/2) +2 */

#define DISPLAY_PPM_VALUE itoa(calculated_ppm,charPPM,10); LCD_StringWrite( CLEAR_PPM , PPM_VALE_ADDRESS ); LCD_StringWrite( charPPM , PPM_VALE_ADDRESS ); 

void setup() {
  pinMode(button,INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  digitalWrite(red,HIGH);
  writeFirstLine("     VISSER");
  writeSecondLine("      CO2");
  delay(3000);
  clearScreen();
  proceedButton();
  writeFirstLine("Put CO2 sensor");
  writeSecondLine("on A1.");
  delay(5000);
  clearScreen();
  proceedButton();
  writeFirstLine("Pull calibration");
  writeSecondLine("pin if near 400.");
  delay(10000);
  startButton();

  pinMode(INPUT_PIN,INPUT);
  digitalWrite(INPUT_PIN,INPUT_PULLUP);
  LCDsetup();
  LCD_StringWrite("CO2 PPM: " , FIRSTLINE );
}

void loop() {
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  DETECT_FALLING_EDGE;
  DETECT_RISING_EDGE;
  do
  {
    delayMicroseconds(500);   
    ms_accumulator+=0.5;              /*accumulate delay in milliseconds, this valued is used for the calculation of PPM*/ 
  }while(digitalRead(INPUT_PIN)!=LOW);  /*detect falling edge*/
  CALCULATE_PPM;
  DISPLAY_PPM_VALUE;
  RESET_VALUES;
  
  
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
