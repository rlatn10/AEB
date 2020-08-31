#include <Wire.h>
#include <VL53L0X.h> 
#include <Servo.h>  

VL53L0X sensor; 
Servo myservo;
int servoPin = 9;  //서보 모터 데이터를 전송할 핀번호
int pos = 200;      //서보 모터 각을 저장할 변수
int stop_state=0; //현재 멈추었는지를 나타내줄 변수. 0이면 아직 제동하지 않은 것이고 1이면 제동한 것이다. 중복하여 제동하는 상황을 막아주기 위하여 만듬
int sw1 = 12;     //택트 스위치로부터 데이터가 전송될 핀번호
int led1 = 10;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
 myservo.attach(9);  
 pinMode(8, OUTPUT);             // 디지털 8번 핀을 출력모드로 설정한다. (진동모터)
  pinMode(led1, OUTPUT);  
  pinMode(sw1, INPUT_PULLUP);    //sw를 인풋_풀업으로 하여 스위치가 열려있다면 HIGH 스위치가 닫혀있다면 LOW값을 반환}

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
for (pos =200; pos >= 199; pos -= 1) 
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
            
  
Serial.print(sensor.readRangeContinuousMillimeters());     //측정된 거리값 시리얼 모니터에 출력
if (sensor.timeoutOccurred()) 
  { Serial.print(" TIMEOUT"); }
  Serial.println();
  
if(sensor.readRangeContinuousMillimeters() > 950 && stop_state == 0)  //측정된 거리가 설정 값보다 크고 (위험요소가 측정 되었고) stop_state가 0 일때 (아직 제동하지 않은 상태) 제동

    
   {Serial.println("laser");                 //시리얼 모니터에 'laser' 출력
    digitalWrite(8, HIGH);                   // 8번 핀에 HIGH신호 (5V)를 출력한다 = 진동모터를 구동
    
    for (pos =200; pos >= 115; pos -= 1)       //0도에서 90도까지 서보모터를 돌려 브레이크 패드를 접촉시켜 휠체어를 제동
       {                                  
        myservo.write(pos);               
        delay(10);                            //브레이크를 잡아주는 시간은 신속해야 하기 때문에 속도를 빠르게 설정
       }
        delay(2000);                         //모터를 당긴 상태로 2초간 멈추어 안전성을 늘림
        digitalWrite(8, LOW);                // 8번 핀에 LOW신호 (0V)를 출력한다 = 진동모터를 멈춤 
                  
       for (pos = 115; pos <= 200; pos += 1) 
       {                                      //다시 서보모터를 원상태로 만들어 제동을 품
       myservo.write(pos);              
       delay(10);                             //브레이크를 풀어주는 시간 역시 신속해야 해서 속도를 빠르게 설정
       }
    stop_state = 1;     
   }//이미 멈추었으니 stop_state를 1로 변경    }
 if(sensor.readRangeContinuousMillimeters() < 950)   //측정된 거리가 설정 값보다 작다면 (위험요소가 측정 되지 않는다면) stop_state에 0을 넣음
 {
  stop_state = 0;
  }
 if(digitalRead(sw1) == LOW)                 //스위치가 눌리면
 {

  Serial.println("sw");                     //시리얼 모니터에 'sw' 출력
  
  for (pos =200; pos >= 115; pos -= 1)        //0도에서 90도까지 서보모터를 돌려 브레이크 패드를 접촉시켜 휠체어를 제동
       { 
        myservo.write(pos);              
        delay(10);                           //브레이크를 잡아주는 시간은 신속해야 하기 때문에 속도를 빠르게 설정
       }
        delay(1000);                         //모터를 당긴 상태로 1초간 멈추어 안전성을 늘림
       for (pos = 115; pos <= 200; pos += 1) 
       { 
       myservo.write(pos);              
       delay(10);                              //브레이크를 풀어주는 시간 역시 신속해야 해서 속도를 빠르게 설정
       }
    stop_state = 1;                          //이미 멈추었으니 stop_state를 1로 바꿔줌 
 }
   else
    digitalWrite(led1, LOW);                 //스위치가 닫히면 Off

}
