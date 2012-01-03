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
   D7 -> voltage divider with 1K resistor and 10k pot (from +5V to ground)
   A0 -> +5V via 10K pot
   
   Micro pinouts (other than above)
   P1->GRD (Vss)
   P2->+5V (Vdd)
   P3->GRD (Vo, contract)
   P5->GRD (Data r/w)
   P7-P11->Float
   P15-> +5V via diode (bias towards pin) (backlight +)
   P16->GRD (Backlight -)  
   
   Debounce code pulled from Arduino debounce demo.
   
   Circuit/code by Matthew Christensen
   mchristensen@moosepod.com
 */
 
 // include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define MODE_SWITCH_BUTTON_PIN  7 // Digital pin
#define WPM_POT_PIN             0 // Analog pin

#define MODE_KEYER 1
#define MODE_FUNC  2

int current_mode = 0; // Current 
int last_mode = 0;
int current_wpm = 0;

#define WPM_MAX_POT_VAL  916
#define MAX_WPM           40

// Following all used for toggle-switch debounce
int last_button_state = LOW;   // the previous reading from the input pin
int button_state = HIGH;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long last_debounce_time = 0;  
long debounce_delay = 50;    

void setup() {
  pinMode(MODE_SWITCH_BUTTON_PIN,INPUT);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  enter_mode(MODE_KEYER);
  
  Serial.begin(9600);
}

void update_lcd(int mode_changed) {
  if (mode_changed) {
    lcd.clear();
  }
  
  switch (current_mode) {
    case MODE_KEYER:
      lcd.setCursor(0,0);
      lcd.print("Keyer");
      lcd.setCursor(0,1);
      lcd.print("WPM: ");
      lcd.print(current_wpm);
      lcd.print(' ');
      break;
    case MODE_FUNC:
      lcd.clear();
      lcd.print("Function Gen.");
      break;
    default:
      lcd.print("Unknown mode.");
  }
}

void enter_mode(int mode) {
  current_mode = mode;
  update_lcd(true);
}

void next_mode() {
  switch (current_mode ) {
    case MODE_KEYER:
      enter_mode(MODE_FUNC);
      break;
    case MODE_FUNC:
      enter_mode(MODE_KEYER);
      break;
    default:
      enter_mode(MODE_KEYER);
      break;
  } 
}

void update_wpm () {
  int reading = analogRead(WPM_POT_PIN);

  int new_wpm = abs(WPM_MAX_POT_VAL - reading) / (WPM_MAX_POT_VAL / MAX_WPM);
  if (new_wpm < 1) {
    new_wpm = 1;
  }
   
  if (current_wpm != new_wpm) {
    current_wpm = new_wpm;
    update_lcd(false);
  } 

}

void check_for_mode_change() {
    int reading = digitalRead(MODE_SWITCH_BUTTON_PIN);
  
  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != last_button_state) {
    // reset the debouncing timer
    last_debounce_time = millis();
  } 
  
  if ((millis() - last_debounce_time) > debounce_delay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    button_state = reading;
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the last_button_state:
  last_button_state = reading;
  
  if (button_state == LOW) {
      if (current_mode != last_mode) {
          next_mode();
          last_mode = current_mode;
      }
  } else {
      last_mode = 0;
  }
}

void loop() {
  check_for_mode_change();
  update_wpm();
}
