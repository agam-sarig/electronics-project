#include <ESP32Servo.h>

#define SERVO_PIN 13

Servo myservo;

void setup(){
  myservo.attach(SERVO_PIN); 
}

void loop(){
  if(myservo.attached()){
    myservo.write(0);
    delay(2000);
    myservo.write(90);
    delay(2000);
    myservo.write(180);
    delay(2000);
    myservo.write(90);
    delay(2000);
    myservo.write(0);
    delay(2000);
  }
}
