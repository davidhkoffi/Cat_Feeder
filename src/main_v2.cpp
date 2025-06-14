#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <DS3231.h>
#include<Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Bounce2.h>
#define MAX_PORTIONS 3
#define MIN_PORTIONS 1

#define DEBUG 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
// change note
//Second Change

//Class Defenition
Servo servo_1; // servo controller (multiple can exist)
Bounce debouncer = Bounce(); // Create a Bounce object

//Variable Defenition
int servo_pin = 3; // PWM pin for servo control
int pos = 0;    // servo starting position
int count;

// bool buttonPressed = false;
// unsigned long buttonPressStartTime = 0;
// const unsigned long holdTime = 3000; // 3 seconds
// bool isButtonHeld = false;           // Flag to indicate if button is held
// unsigned long buttonPressTime = 0;   // Time when the button was pressed

byte portions = 3;
int upButton = 12;
int selectButton = 10;
int dwnButton = 4;
int buzzer = 7;
int secs = 5;
byte i = 0;
int interval = 1;

int portion_conversion = 180/portions;

int dwn_state = 0;
int sel_state = 0;
int up_state = 0;

bool mode = false;

int Hour, Minute, seconds;

int prt_Address = 0;   //Location we want the data to be put.
int intval_Address = 1;

//Establish classes for timer module & display
Time t;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL);
void checkButton();
bool timeToFeed();
void feedCat();
// bool checkButtonHold(int pin, unsigned long duration);

void setup()
{
  Serial.begin(9600);
  servo_1.attach(servo_pin); // start servo control
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  debouncer.attach(selectButton);
  debouncer.interval(100);  // Debounce interval in milliseconds

  rtc.begin();
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
  pinMode(upButton, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(dwnButton, INPUT);

  portions = EEPROM.read(prt_Address);
  interval = EEPROM.read(intval_Address);

  debug("Last Portion ");
  debugln(portions);
  debug("Last Interval: ");
  debugln(interval);
  delay(1000);
}

void loop()
{
  //Output Portions & Interval
  lcd.setCursor(0,0);
  lcd.print(portions); 
  lcd.print(" Portion(s)");                           
  lcd.setCursor(0,1);
  lcd.print("Every "); 
  lcd.print(interval); 
  lcd.print(" hours "); 
  lcd.display(); 

  //Check button state
  checkButton();

  if(timeToFeed()){
    feedCat();
  }
    
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
}

void checkButton(){
  debouncer.update();
  up_state = digitalRead(upButton);
  sel_state = debouncer.read();
  dwn_state = digitalRead(dwnButton);

  if(sel_state == HIGH){
    //debugln("Select button pressed");
    if(!mode){
      mode = true;
      //debug("Mode is false");debugln("*************");
      //delay(100);
    }
    else{
      mode = false;
      //debug("Mode is true");debugln("*************");
      //delay(100);
    }
    count++;
    debug("Count: ");debugln(count);
    if(count > 60){
      feedCat();
      debugln("*************");
      debugln("Manual feed");
      debugln("*************");
    }
  }
  if(mode){
    if (up_state== HIGH)// if up is pressed increment through interval list
    { 
      interval  = interval +1;
      EEPROM.write(intval_Address, interval);
      if(interval>12){
        interval = 3;
      }
      debugln();
      debugln("Up button pressed");
      debug("Interval is now: ");debugln(interval);
      count = 0;
      delay(200);
    }
    else if(dwn_state == HIGH)//if down is pressed decrement through interval list
    {  
      interval  = interval -1;
      EEPROM.write(intval_Address, interval);
      if(interval<1){
        interval = 12;
      }
      debugln();
      debugln("Down button pressed");
      debug("Interval is now: ");debugln(interval);
      count = 0;
      delay(200);
    }
    
  }
  else{// if middle is pressed and mode is true
    if (up_state== HIGH)
    {
      portions++;
      portions = (portions>MAX_PORTIONS)? MIN_PORTIONS : portions;
      EEPROM.write(prt_Address, portions);
      count = 0;
      delay(200);
    }
    if (dwn_state== HIGH)
    {
      portions--;
      portions = (portions<MIN_PORTIONS)? MAX_PORTIONS : portions;
      EEPROM.write(prt_Address, portions);
      count = 0;
      delay(200);
    }
  }
}

bool timeToFeed(){
  t = rtc.getTime();                          //Get time from DS3232
  Hour = t.hour;                              //Get the hour in 0h-23h format
  Minute = t.min;  
  seconds = t.sec;
  //If the remainder of the hour divided by the interval is = 0, feed cat
  if (Hour % interval == 0 && Minute == 0 && seconds< secs){
    return true;
  }
  else{
    return false;
  }
}

void feedCat(){
  lcd.setCursor(0,0);
  
  lcd.print("Feeding Miso ");  
  lcd.display(); 
  tone(buzzer, 1000); // Send 1KHz sound signal...

  servo_1.write(85);
  delay(1000*portions*0.5);
  servo_1.write(90);
  servo_1.write(110);
  delay(1000*portions*0.5);
  servo_1.write(90);

  noTone(buzzer);
  count = 0;
}





