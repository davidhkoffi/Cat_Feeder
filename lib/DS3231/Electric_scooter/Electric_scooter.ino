/*  Electric scooter project. Created by ELECTRONOOBS
 *   Web: http://www.electronoobs.com
 *   Yt:  http://www.youtube.com/c/ELECTRONOOBS
 *
 *   PWM signal is digital pin D3
 *   The potentiometer read is analog pin A0
 *   Connect the 6.25V from ESC to Vin of Arduino
 *   Share GND with everything
 */

#include <Servo.h>;                         //We need this library for the PWm signal
Servo motor;                                //We create the servo pin, later attach to D3

int pot_in=A0;                              //Now let's create the variables that we need in this code
int Value=0;
int Speed=1505;
int realSpeed=1505;

void setup() {
  motor.attach(3);
  motor.writeMicroseconds(1505);           //The range of this ESC is 1000us to 1500 for CW and 1500 to 2000us for CCW
                                           //That's why I map the values from 1505 to 2000u in order to avoid backwards rotations
  delay(5000);  
}

void loop() {
  Value=analogRead(pot_in);                 //We read the analog value
  Speed= map(Value, 1020,600,1505,2000);    //map the 0 to 1024 ponts of analog read to our ranfe from 1505 to 2000us
  Speed = constrain(Speed,1505,2000);       //make sure the signal will never be over the range

  /*In order to make sure we will have a smooth acceleration and braking we add a small delay*/
  if(Speed > realSpeed)
  {
    realSpeed= realSpeed+1;
    delay(3);
  }

  if(Speed < realSpeed)
  {
    realSpeed= realSpeed-1;
    delay(3);
  }
  /*Now we write the PWM value using the servo microseconds functions*/
  realSpeed = constrain(realSpeed,1505,2000);
  motor.writeMicroseconds(realSpeed);
}
