#include <Wire.h>
#include <VL53L0X.h> 
#include <Servo.h>  

VL53L0X sensor; 
Servo myservo;
int servoPin = 9; 
int pos = 0;      //서보모터 각을 저장할 변수
int stop_state=0; //이미 멈추었는지를 나타내줄 변수
void setup()
{
  Serial.begin(9600);
  Wire.begin();
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor.startContinuous();
}
void loop()
{
  Serial.print(sensor.readRangeContinuousMillimeters()/10);  //측정된 거리 시리얼 모니터에 출력
 Serial.print("cm");
  if (sensor.timeoutOccurred()) 
  { Serial.print(" TIMEOUT"); }
  Serial.println();
  if(sensor.readRangeContinuousMillimeters() > 700 && stop_state == 0)  //측정된 거리가 설정 값보다 크고 stop_state가 0일때 멈춤
    {
       for (pos = 0; pos <= 90; pos += 1) 
       { 
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(3);                       // waits 15ms for the servo to reach the position
       }
       
       for (pos = 90; pos >= 0; pos -= 1) 
       { 
       myservo.write(pos);              // tell servo to go to position in variable 'pos'
       delay(3);                       // waits 15ms for the servo to reach the position
       }
    stop_state = 1;                   //이미 멈추었으니 stop_state를 1로 바꿔줌 
    }
 if(sensor.readRangeContinuousMillimeters() < 700)   //측정된 거리가 설정 값보다 작다면 stop_state
 {
  stop_state = 0;
  }
}
