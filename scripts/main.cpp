
#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <DS3231.h>
#include<Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#define MAX_PORTIONS 5
#define MIN_PORTIONS 0

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

Servo servo_1; // servo controller (multiple can exist)

int servo_pin = 3; // PWM pin for servo control
int pos = 0;    // servo starting position
unsigned int millis_before = 0;     //We use these to create the loop refresh rate
unsigned int millis_now = 0;
byte portions = 3;
int upButton = 6;
int selectButton = 5;
int dwnButton = 4;
int buzzer = 7;
int secs = 5;
byte i = 0;
int interval_list[] = {1,3,8,12};

int portion_conversion = 180/portions;

int dwn_state = 0;
int sel_state = 0;
int up_state = 0;

bool mode = false;

int Hour, Minute, seconds;

int prt_Address = 0;   //Location we want the data to be put.
int intval_Address = 1;

Time t;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL);
void checkButton();


void setup()
{
  Serial.begin(9600);
  servo_1.attach(servo_pin); // start servo control
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  rtc.begin();
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
  pinMode(upButton, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(dwnButton, INPUT);

  portions = EEPROM.read(prt_Address);
  interval_list[i] = EEPROM.read(intval_Address);

  debug("Contents of EEPROM portion Adress 0: ");
  debugln(portions);
  debug("Contents of EEPROM interval Adress 1: ");
  debugln(interval_list[i]);
  delay(1000);
}

void loop()
{
  millis_now = millis(); 

  if(millis_now - millis_before > 100)
  {     //Refresh rate of the OLED
    millis_before = millis();  
    lcd.setCursor(0,0);
    lcd.print(portions); 
    lcd.print(" Portion(s)");
                                              
    lcd.setCursor(0,1);
    lcd.print("Every "); 
    lcd.print(interval_list[i]); 
    lcd.print(" hours "); 
    lcd.display(); 
  }

  up_state = digitalRead(upButton);
  sel_state = digitalRead(selectButton);
  dwn_state = digitalRead(dwnButton);

  debugln("******************");
  debugln("Portions");
  debug(prt_Address);
  debug("\t");
  debug(portions);
  debugln();

  debugln("******************");
  debugln("Interval Index");
  debug(intval_Address);
  debug("\t");
  debug(interval_list[i]);
  debugln();

  if ((sel_state == HIGH) && ( mode==false)){
    mode = true;
    delay(200);
  }
  else if(sel_state == HIGH){
    mode = false;
    delay(200);
  }

  if(mode){ // if middle button is pressed  and mode is true| analog val = 400
    if (up_state== HIGH)// if up is pressed increment through interval list
    { 
      i++;
      EEPROM.write(intval_Address, interval_list[i]);
      i = (i>3)? 0 : i;
      delay(200);
    }
    if (dwn_state == HIGH && i>0)//if down is pressed decrement through interval list
    {  
      i--;
      EEPROM.write(intval_Address, interval_list[i]);
      delay(200);
    }
  }
  else{// if middle is pressed and mode is true
    if (up_state== HIGH)
    {
      portions++;
      portions = (portions>MAX_PORTIONS)? MIN_PORTIONS : portions;
      EEPROM.write(prt_Address, portions);
      delay(200);
    }
    if (dwn_state== HIGH && portions>MIN_PORTIONS)
    {
      portions--;
      EEPROM.write(prt_Address, portions);
      delay(200);
    }
  }
    debugln("Down button Val: ");
    debugln(dwn_state);
    debugln(" ");
    debug("Mode :" );
    debugln( mode);
  

  t = rtc.getTime();                          //Get time from DS3232
  Hour = t.hour;                              //Get the hour in 0h-23h format
  Minute = t.min;  
  seconds = t.sec;

  debugln("******");
  debug("Hour: ");
  debugln(Hour);
  debug("Min: ");
  debugln(Minute);
  // debug("Interval Every - ");
  // debugln(interval);
  debug("interval: ");
  debugln(interval_list[i]);

  debug("portions: ");
  debugln(portions);

  if (Hour % interval_list[i] == 0 && Minute == 00 && seconds< secs)
  {

    tone(buzzer, 1000); // Send 1KHz sound signal...

    for (pos = 0; pos <= portions*portion_conversion; pos += 1) 
    { 
    // in steps of 1 degree
      servo_1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15ms for the servo to reach the position
    }
    for (pos = portions*portion_conversion; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo_1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20); 
    }
    
    int toDelay = secs - seconds;
    debugln("********");
    debug("TO DELAY: ");
    debugln(toDelay);
    debugln("********");

    delay(toDelay*1000);
    noTone(buzzer);
  }
}
