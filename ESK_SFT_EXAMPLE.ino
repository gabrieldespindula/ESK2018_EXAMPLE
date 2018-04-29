//Emirates Skills 2018, example program
#include <LiquidCrystal.h>
#include "BasicStepperDriver.h"
//Emirates Skills 2018, example program
#include <LiquidCrystal.h>
#include "BasicStepperDriver.h"
#include "Ultrasonic.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 8

// All the wires needed for full functionality
#define DIR 12
#define STEP 13

//buzzer definition
#define BUZZ  A1

// define some values used by the panel and buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//define ultrasonic
#define TRIGPIN 2
#define ECHOPIN 3 

// select the motor pins
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//ultrsonic start
Ultrasonic ultrasonic(TRIGPIN,ECHOPIN);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}


void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
 stepper.begin(RPM, MICROSTEPS);
 pinMode(BUZZ, OUTPUT);
 Serial.begin(9600);
}

int motor_steps=100;
int motor_dir=0;
int step_move(){
  if(motor_dir) motor_steps++;
  else motor_steps--;
  if(motor_steps<0 || motor_steps>200) motor_dir=!motor_dir;
  return (motor_dir*2)-1;
}

int distance;
void loop()
{
 //Move the motor
 stepper.move(step_move());
 
 //Read ultrassonic sensor and display in the LCD
 distance = ultrasonic.Ranging(CM);
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(distance);
 lcd.print("cm     ");

 //Ring buzzer if distance is too low
 if(distance<10 && distance>0)  digitalWrite(BUZZ, HIGH);
 else  digitalWrite(BUZZ, LOW);

 //Read buttons and show in the LCD 
 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }
}

