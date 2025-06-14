#include <Arduino.h>
#include <Servo.h>


Servo servo_1; // servo controller (multiple can exist)
int portions = 3;
int portion_conversion = 180/portions;
int buzzer = 7;
bool start = true;

int servo_pin = 3; // PWM pin for servo control
int pos = 0;    // servo starting position

void setup() {
    servo_1.attach(servo_pin); // start servo control
    pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);
    noTone(buzzer);
}

// the loop function runs over and over again forever
void loop() {

    servo_1.write(85);
    delay(5000);
    servo_1.write(90);
    delay(1000);
    while (1);   
    
}