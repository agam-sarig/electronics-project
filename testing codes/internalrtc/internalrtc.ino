#include <ESP32Time.h>
ESP32Time rtc(3600);  

void setup() {
  Serial.begin(115200);
  rtc.setTime(30, 24, 9, 19, 5, 2024);  
}

void loop() {
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  
  delay(1000);
}
