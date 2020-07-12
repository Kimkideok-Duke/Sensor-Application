#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

#define LED_RED 14
#define LED_GREEN 12
#define LED_BLUE 13
#define LED_CLEAR 2
#define MC 0         //기능전환버튼

#include <Wire.h>
#include "OLED.h"//LCD

OLED display(4,5);
char strdata[100];

int change;
bool mode_change = true;

void setup()
{
  pinMode(LED_CLEAR, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(MC, INPUT_PULLUP);//기능전환 버튼
  
  digitalWrite(LED_CLEAR, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  display.begin();

  Serial.begin(115200);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");
}

void loop()
{
  change = digitalRead(MC);
  
  if(change == 0)//버튼 입력 --> 기능전환
  {
    mode_change = !mode_change;
    Serial.println("MODE CHANGE");
    display.print("MODE CHANGE");
    delay(1000);
  }
  
    if(mode_change)//조도센서
    {
      apds.enableProximity(false);
      apds.enableGesture(false);//제스처 센서 off
      apds.enableColor(true);//RGB센서 on
      RGB_sensor();
    }
    else//제스처센서
    {
      apds.enableColor(false);//RGB센서 on
      apds.enableProximity(true);
      apds.enableGesture(true);//제스처 센서 on
      Gesture_sensor();
    }
  
}

void Gesture_sensor()//제스처 센서
{
  uint8_t gesture = apds.readGesture();//제스처 값 수집
    if(gesture == APDS9960_DOWN) //down일때
    {
      display.clear();
      display.print("DOWN", 1, 0);//oled출력
      Serial.println("v");
    }
    if(gesture == APDS9960_UP) //up일때
    {
      display.clear();
      display.print("UP", 1, 0);//oled출력
      Serial.println("^");
    }
    if(gesture == APDS9960_LEFT) //left일때
    {
      display.clear();
      display.print("LEFT", 1, 0);//oled출력
      Serial.println("<");
    }
    if(gesture == APDS9960_RIGHT) //right일때
    {
      display.clear();
      display.print("RIGHT", 1, 0);//oled출력
      Serial.println(">");
    }
  
}
void RGB_sensor()//조도센서 함수
{
  display.clear();
  
  digitalWrite(LED_CLEAR, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  //create some variables to store the color data in
  uint16_t r, g, b, c, mx;  //mx는 최댓값
  
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }
  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);  //값 수집
  
  if(r > g)                               //r,g,b,c값 크기비교 후 가장 큰 값을 mx에 저장
  {                                       //r값이 가장 클 경우는 빨간색 led출력
    if(r > b)                             //g값이 가장 클 경우는 초록색 led출력
    {                                     //b값이 가장 클 경우는 파란색 led출력
      if(r > c)                           //c값이 가장 클 경우는 gpio2 led출력
      {                                   //조도센서 역할을 위해 만약 mx값이 10보다 작을 시(주변이 매우 어두울시) 
        Serial.print(" red: ");           //led 모두 off
        Serial.println(r);
        mx = r;
        if(mx > 10)
        {
          digitalWrite(LED_RED, LOW);
        }else;
      }
      else
      {
        Serial.print(" clear: ");
        Serial.println(c);
        mx = c;
        if(mx > 10)
        {
          digitalWrite(LED_CLEAR, LOW);
        }else;
      }
    }
    else 
    {
      if(b > c)
      {
        Serial.print(" blue: ");
        Serial.println(b);
        mx = b;
        if(mx > 10)
        {
          digitalWrite(LED_BLUE, LOW);
        }else;
      }
      else
      {
        Serial.print(" clear: ");
        Serial.println(c);
        mx = c;
        if(mx > 10)
        {
          digitalWrite(LED_CLEAR, LOW);
        }else;
      }
    }
  }
  else
  {
    if(g > b)
    {
      if(g > c)
      {
        Serial.print(" green: ");
        Serial.println(g);
        mx = g;
        if(mx > 10)
        {
          digitalWrite(LED_GREEN, LOW);
        }else;
      }
      else
      {
        Serial.print(" clear: ");
        Serial.println(c);
        mx = c;
        if(mx > 10)
        {
          digitalWrite(LED_CLEAR, LOW);
        }else;
      }
    }
    else
    { 
      if(b > c)
      {
        Serial.print(" blue: ");
        Serial.print(b);
        mx = b;
        if(mx > 10)
        {
          digitalWrite(LED_BLUE, LOW);
        }else;
      }
      else
      {
        Serial.print(" clear: ");
        Serial.println(c);
        mx = c;
        if(mx > 10)
        {
          digitalWrite(LED_CLEAR, LOW);
        }else;
      }
    }
  }
   
  Serial.println();
  
  delay(10);
}
