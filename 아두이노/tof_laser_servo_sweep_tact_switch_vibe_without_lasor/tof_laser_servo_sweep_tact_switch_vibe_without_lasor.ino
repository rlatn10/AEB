
#include <Servo.h>  


Servo myservo;
int servoPin = 9;  //서보 모터 데이터를 전송할 핀번호
int pos = 0;      //서보 모터 각을 저장할 변수
int stop_state=0; //현재 멈추었는지를 나타내줄 변수. 0이면 아직 제동하지 않은 것이고 1이면 제동한 것
int sw1 = 12;     //택트 스위치로부터 데이터가 전송될 핀번호
int led1 = 10;

void setup()
{
  Serial.begin(9600);
  
 myservo.attach(9);  

  pinMode(led1, OUTPUT);  
  pinMode(sw1, INPUT_PULLUP);    //sw를 인풋_풀업으로 하여 스위치가 열려있다면 HIGH 스위치가 닫혀있다면 LOW값을 반환}

 
}

void loop()
{
  
 if(digitalRead(sw1) == LOW)                   //스위치가 눌리면
 {
  for (pos = 0; pos <= 90; pos += 1)           //0도에서 90도까지 서보모터를 돌려 브레이크 패드를 접촉시켜 휠체어를 제동
       { 
        myservo.write(pos);              
        delay(3);                              //브레이크를 잡아주는 시간은 신속해야 하기 때문에 속도를 빠르게 설정
       }
        delay(1000);                           //모터를 당긴 상태로 1초간 멈추어 안전성을 늘림
       for (pos = 90; pos >= 0; pos -= 1) 
       { 
       myservo.write(pos);              
       delay(3);                               //브레이크를 풀어주는 시간 역시 신속해야 해서 속도를 빠르게 설정
       }
    stop_state = 1;                            //이미 멈추었으니 stop_state를 1로 바꿔줌 
 }
   else
    digitalWrite(led1, LOW);                   //스위치가 닫히면 Off
}
