/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int servoPin = 9;  //서보 모터 데이터를 전송할 핀번호
int pos = 180;    // variable to store the servo position

int sw2 = 12;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
   Serial.begin(9600); 

    pinMode(sw2,INPUT_PULLUP);
}


void loop() {
  for (pos =200; pos >= 199; pos -= 1)
  { // goes from 0 degrees to 180 degrees
     //in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    
  }

 if(digitalRead(sw2)==LOW)
 {
  Serial.println("sw"); 
   for (pos =200; pos >= 110; pos -= 1) { // goes from 0 degrees to 180 degrees
     //in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                  // waits 15ms for the servo to reach the position
   }
   
  for (pos = 110; pos <= 200; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  }
}


   
  
