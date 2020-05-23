#include <Wire.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "smart_window.h"

#define DHTPIN A2
#define DHTTYPE DHT11
int oc_state = 1;
Stepper myStepper(stepsPerRevolution, 11, 9, 10, 8); //
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
//#########창문 개폐 함수###############
void open_window()                                   // 창문을 여는 코드
{  
  if (oc_state == 1)                                 //창문이 열리고 닫힌 상태를 나타내려고 만든 변수 oc_state 1이면 닫혀있는거고 0이면 열려있다
  {                                                  //변수 oc_state 가 0이면 이미 열려있으니 열 필요 없고 1일때만 염
    for (int i = 0; i < 49; i++) { // 64 * 32 = 2048 한바퀴  // 정방향으로 49번 회전하여(49*5.625)=275도 창문을 닫음
      myStepper.step(stepsPerRevolution);
    }
    oc_state = 0;                                    //창문을 열었으니 oc_state에 열려있음을 나타내는 값인 0을 줌
  }                                                     
}
void close_window()                                  // 창문을 닫는 코드
{
  if (oc_state == 0)                                //변수 oc_state 가 1이면 이미 닫혀있으니 닫을 필요 없고 0일때만 닫음
  {
    for (int i = 0; i < 49; i++) {                   // 역방향으로 49번 회전하여(49*5.625)=275도 창문을 닫음
      myStepper.step(-stepsPerRevolution);
    }
    oc_state = 1;                                    //창문을 닫았으니 oc_state에 닫혀있음을 나타내는 값인 1을 줌
  }                                          
}
//#####################################

void setup()
{
  myStepper.setSpeed(500);
  lcd.init();
  Serial.begin(9600);
  pinMode(ledPower , OUTPUT);
  dht.begin();

}

void loop()
{
  //######레인센서#######
  int rainvalue = analogRead(rainsensorPin);        //rainvalue에 레인센서를 통해 측정한 값을 저장함

  Serial.print("강수량 : ");                         //시리얼 모니터에 강수량을 나타냄
  Serial.println(rainvalue);
  //#####################

  //########온습도 센서###########
  double h = dht.readHumidity();
  double t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");
  Serial.println(oc_state);
  //#############################

  //######### LCD모니터 ###########
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(2, 0);
  lcd.print((int)t);
  lcd.setCursor(6, 0);
  lcd.print("H=" );
  lcd.print((int)h);
  lcd.setCursor(0, 1);
  //###############################

  //###########창문 개폐 조건###############         //온도가 일정온도 이하이고 비가 오며 습도가 일정 이상일때 문을 닫음
  //
  if (rainvalue < 500)                             //레인센서에 비가 측정되어 창문을 닫는 상황
  {     
    lcd.clear();                                  //lcd에 이전에 쓰여있던 내용을 지운다
    lcd.setCursor(0, 0);
    lcd.print("it's rainning!");                  //비가오는 상황임을 알린다
    lcd.setCursor(11, 1);
    lcd.print("Close");                           //비가와서 창문을 닫는 상황임을 알린다
    delay(500);    
    close_window();                                //창문을 닫는다 (만약 oc_state가 1이라면 이미 닫혀있으니 모터를 돌려서 닫을 필요가 없다)                           
  }
  else if ( t < 22.0 || h > 75  )                 //온도가 일정온도 이하이거나 습도가 높아서 문을 닫는 상황
  {         
    lcd.setCursor(11, 1);
    lcd.print("Close");
    delay(500);    
    close_window();
   lcd.clear();    
  }
  else                                          //위의 모든 상황을 만족하지 않을때만 창문을 열고 있음
  {
    lcd.setCursor(12, 1);
    lcd.print("Open");
    delay(500);
    open_window();
    lcd.clear(); 
   }
    delay(100);
    //####################################
}
