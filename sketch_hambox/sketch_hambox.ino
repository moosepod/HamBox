/*
  HamBox
  
  Arduino-driven ham utility box.
  
  Two primary modes:
  - CW Keyer
  - Function generator
 
  - Uses a Microtivity 16x2 LCD display
 
   Pinouts:
   D2 -> LCD-D7
   D3 -> LCD-D6
   D4 -> LCD-D5
   D5 -> LCD-D4
   D11-> LCD-Enable
   D12-> LCD-RS
   
   Micro pinouts (other than above)
   P1->GRD (Vss)
   P2->+5V (Vdd)
   P3->GRD (Vo, contract)
   P5->GRD (Data r/w)
   P7-P11->Float
   P15-> +5V via diode (bias towards pin) (backlight +)
   P16->GRD (Backlight -)  
   
   Circuit/code by Matthew Christensen
   mchristensen@moosepod.com
 */
 
 // include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define MODE_KEYER 1
#define MODE_FUNC  2

int g_mode = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  enter_mode(MODE_KEYER);
}

void enter_mode(int mode) {
  g_mode = mode;
  switch (g_mode) {
    case MODE_KEYER:
      lcd.print("Keyer");
      break;
    case MODE_FUNC:
      lcd.print("Function Generator");
      break;
    default:
      lcd.print("Unknown mode.");
  }
}

void loop() {
}
