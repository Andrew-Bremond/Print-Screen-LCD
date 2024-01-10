#include "Timer.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,10,5,4,3,2);
enum STATE {INIT, rel, erase} gState = INIT;
int x_joystick;
int y_joystick;
int button = LOW;
int x_cursor = 0;
int y_cursor = 0;
String c = "X";
int cnt = 0;

void ButtonPress(void) {
  //transitions

  switch (gState) {
    case INIT:
      if (cnt > 5) {
        gState = erase;
      }
      else if (!button) {
        gState = rel;
      }
      else {
        gState = INIT;
      }
    break;
    case rel:
      if (button) {
        gState = INIT;
      }
      else {
        gState = rel;
      }
    break;
    case erase:
      gState = INIT;
    break;
  }

  //actions
  switch (gState) {
    case INIT:
    break;
    case rel:
      lcd.print(c);
      cnt++;
    break;
    case erase:
      lcd.clear();
      cnt = 0;
      x_cursor = 0;
      y_cursor = 0;
    break;
  }

}

void MoveCursor(void) {
  if (x_joystick < 100 && x_cursor > 0) {
    x_cursor--;
  }
  if (x_joystick > 900 && x_cursor < 15) {
    x_cursor++;
  }
  if (y_joystick < 100 && y_cursor > 0) {
    y_cursor--;
  }
  if (y_joystick > 900 && y_cursor < 1) {
    y_cursor++;
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(A0, INPUT); //x_joystick input
  pinMode(A1, INPUT); //y_joystick input
  pinMode(13, INPUT_PULLUP); //joystick button
  TimerSet(100); //this value the period in ms
  TimerOn();
  Serial.begin(9600);
}

void loop()
{
  x_joystick = analogRead(A0);
  y_joystick = analogRead(A1);
  button = digitalRead(13);
  lcd.cursor();
  // Serial.println("What character do you want to print? Enter:");
  // String c = getString();
  lcd.setCursor(x_cursor, y_cursor);
  ButtonPress();
  MoveCursor();
  while(!TimerFlag){}
  TimerFlag = 0;
}

String getString(){
  while(true){
    while(!Serial.available()){}
    String input = Serial.readStringUntil('\n');
    return input;
  }
}
