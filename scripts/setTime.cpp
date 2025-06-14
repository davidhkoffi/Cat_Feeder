#include <Wire.h>
#include <DS3231.h>

DS3231  rtc(SDA, SCL);

// Uncomment the following line and set the desired date and time for initial setup.
// This should be commented out after the first successful upload to avoid resetting the time again.
// DateTime now(2024, 9, 22, 15, 30, 0); // YYYY, MM, DD, HH, MM, SS

void setup() {
  Serial.begin(9600);

    rtc.begin();
    rtc.setDOW(TUESDAY);     // Set Day-of-Week to SUNDAY
    rtc.setTime(18, 31, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(1, 1, 2025);   // Set the date to January 1st, 2014
}

void loop() {
// Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
  
  // Wait one second before repeating
  delay (1000);

}