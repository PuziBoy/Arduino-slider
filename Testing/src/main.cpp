///////////////////////////////CONNECT SECTION/////////////////////////////
#include <Wire.h> // apparently the most important thing idk arduino too yeah
#include <Arduino.h>// Include the Arduino Stepper Library
#include <Stepper.h> // stepper motor
#include <Encoder.h> // encoder
#include <LedControl.h> //maxon
#include <LiquidCrystal_I2C.h> //lcd
#include <Keypad.h>
/////////////////////MAXON SECTION//////////////////////
int DIN = 10;
int CS = 9;
int LCLK = 8;
byte center[8] = {0xE7, 0x99, 0xA5, 0x5A, 0x5A, 0xA5, 0x99, 0xE7};
byte rightArrow[8] = {0x08, 0x0C, 0xFE, 0xFF, 0xFF, 0xFE, 0x0C, 0x08};
byte leftArrow[8] = {0x10, 0x30, 0x7F, 0xFF, 0xFF, 0x7F, 0x30, 0x10};
byte smile[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
LedControl lc = LedControl(DIN, LCLK, CS, 1);
void printByte(byte character[])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}
//////////////////LCD I2C SECTION////////////////////
LiquidCrystal_I2C lcd(0x27, 16, 2);
void movementCenter()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(1, 1);
  lcd.print("the center");
}
void movementLeft()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(1, 1);   
  lcd.print("the left");
  lcd.clear();
}
void movementRight()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(1, 1);
  lcd.print("the right");
  lcd.clear();
}
void movementCenterDone()
{
  lcd.setCursor(1, 0);
  lcd.print("SUCCESS");
}
///////////////////MAXON + LSD////////////////////////
void clearM_L()
{
  lc.clearDisplay(0); // clear maxon display
  lcd.clear(); // clear lcd display
}
////////////////STEPPER SECTION////////////////////////
const int stepsPerRevolution = 200; // number of steps per output rotation
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
#define S1 11
#define S2 12
int prevS1;
int curS1, curS2;
int val = 0;
////////////////KEYPAD SECTION/////////////////////////
const byte ROWS = 4;   // Row quantity
const byte COLS = 4;   // Strokes quantity
char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {23, 25, 27, 20}; // pins for rows
byte colPins[COLS] = {31, 33, 35, 37}; // pins for cols
//////////////////////////////////////GENERAL SECTION//////////////////////////////////////////////////
/////////////////VOID SETUP////////////////////////
void setup()
{
  lcd.init(); //init lcd 1602
  lcd.backlight(); //turn backlight on
  lcd.display(); // display in power mode
  lc.shutdown(0, false);  //the maxon is in power-saving mode on startup
  lc.setIntensity(0, 15); //set the brightness of maxon to maximum value
  lc.clearDisplay(0); // clear maxon display
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode (S1, INPUT);
  pinMode (S2, INPUT);
  prevS1 = digitalRead(S1);
}
//////////////////////////VOID LOOP/////////////////////////////
void loop()
{
  // encoder code
  curS1 = digitalRead(S1);
  if (curS2!=curS1) {
    curS2 = digitalRead(S2);
    if(curS2 == curS1){
      val++;
    }
    else{
      val--;
    }
    Serial.print("Value = ");
    Serial.println(val);
  }
  prevS1=curS1;
  bool flag;
  if(curS1!=prevS1){
    curS2 = digitalRead(S2);
    if(flag){
      if(curS2==curS1){
        val++;
      }
      else{
        val--;
      }
      Serial.print("Value = ");
      Serial.println(val);
      flag=false;
    }
    else{
      flag=true;
    }
  }
  prevS1=curS1;
  //main actions
  /*movementCenter();
  printByte(center);
  delay(1500);
  clearM_L();*/
  movementRight();
  printByte(rightArrow);
  myStepper.step(stepsPerRevolution);
  delay(1500);
  clearM_L();
  movementLeft();
  printByte(leftArrow);
  myStepper.step(-stepsPerRevolution);
  delay(1500);
  clearM_L();
  /*movementCenterDone();
  printByte(center);
  delay(1500);
  printByte(smile);
  clearM_L();*/
}
///////////////////////////////////////END GENERAL SECTION//////////////////////////////////////////////////////
