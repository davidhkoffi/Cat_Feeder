#include <Arduino.h>
//#include <DS3231.h>
#include <Bounce2.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
int buzzer = 7;
int upButton = 4;
int up_state = 0;

Bounce debouncer = Bounce(); // Create a Bounce object

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);    // turn the LED off by making the voltage LOW
  pinMode(upButton, INPUT);
}

// the loop function runs over and over again forever
void loop(){

  up_state = digitalRead(upButton);

  debugln(up_state);
  // tone(buzzer, 1000); // Send 1KHz sound signal...
  // delay(1000);
  // noTone(buzzer);
  // delay(1000);


  // digitalWrite(LED_BUILTIN, 1);   // turn the LED on (HIGH is the voltage level)
  // delay(100);                       // wait for a second
  // digitalWrite(LED_BUILTIN, 0);    // turn the LED off by making the voltage LOW
  // delay(1000);                       // wait for a second
  
  // if (Serial.available() > 0) { // Check if data is available
  //   int incomingByte = Serial.read(); // Read incoming data
    
  //   if (incomingByte == 78) { // Check if 'N' (ASCII 78) is pressed
  //     digitalWrite(LED_BUILTIN, 1);   // turn the LED on (HIGH is the voltage level)
  //     delay(1000);                       // wait for a second
  //     digitalWrite(LED_BUILTIN, 0);    // turn the LED off by making the voltage LOW
  //     delay(1000);    
  //   }

  //   debugln(incomingByte); // Print received data
  // }

  // debugln("Hello World");

}
