//DistHelm C/C++ Program
#include <Adafruit_NeoPixel.h>    //header file for the Neo pixel ring
#include <LiquidCrystal.h>        //header file for the LCD display

LiquidCrystal lcd (13,12,11,10,9,8);  //initialize the library with the numbers of the interface pins
int ledPin=3;  //pin where the neo strip is connected to the Arduino
int ledNo=12;  //number of LEDs in the NeoPixel ring

Adafruit_NeoPixel strip= Adafruit_NeoPixel(ledNo,ledPin,NEO_RGB+NEO_KHZ800);

int buzzerPin=2; //pin where the buzzer is connected to the Arduino
int echoPin=6; //pin where the echo is connected to the Arduino
int trigPin=5; //pin where the trigger is connected to the Arduino
int minDistance=100; //it is in cm
int maxDistance=300; //it is in cm

void setup() 
{
  Serial.begin(9600);  

  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin(16,2);  
  lcd.setCursor(4,0);   
  lcd.print("COVID-19");  //this would be printed when device is on 
  lcd.setCursor(0,1);
  lcd.print("    DistHelm");
  delay(1000);
  lcd.clear();  
  
  strip.begin();
  for(int i=0;i<ledNo;i++)
  {
   strip.setPixelColor(i,strip.Color(0,0,0)); //default color of LEDs is black
  }
  strip.show();
}

void loop() 
{
  int distance=calcDistance();
  Serial.print("Distance is: ");
  Serial.print(distance);
  Serial.println("cm");
  int ledsToGlow = map(distance, minDistance, maxDistance, ledNo, 1);
  Serial.print(ledsToGlow);
  Serial.println(" LED(s) are glowing");
  
  if(ledsToGlow==9 || ledsToGlow == 10 || ledsToGlow == 11 || ledsToGlow == 12)
  {
    digitalWrite(buzzerPin, HIGH); //buzzer turns on
    Serial.println("Keep Distance");
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("ALERT");
      lcd.setCursor(0,1);
      lcd.print("Unsafe Distance");    
  }
  else
  {
    digitalWrite(buzzerPin, LOW);    //buzzer turns off
    Serial.println("Safe Distance");
      lcd.clear();
      lcd.setCursor(3,0);      //theses will be printed when red leds are off
      lcd.print("You are at a");
      lcd.setCursor(1,1);
      lcd.print("safe distance");
  }
  
  for(int i=0;i<ledsToGlow;i++)
  {
    if(i<4)
    {
      strip.setPixelColor(i,strip.Color(200,0,0));//green,red,blue
    }
    else if(i>=4 && i<8)
    {
      strip.setPixelColor(i,strip.Color(233,255,0));//green,red,blue
    }
    else if(i>=8 && i<12)
    {
      strip.setPixelColor(i,strip.Color(0,255,0));//green,red,blue
    }
  }
  
  for(int i=ledsToGlow;i<ledNo;i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,0));
  }
  strip.show();
  delay(50);
}

int calcDistance()
{
  float distance,duration;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin,HIGH);
  distance=duration/29/2;
  if(distance>=maxDistance)
  {
    distance=maxDistance;
  }
  if(distance<=minDistance)
  {
    distance=minDistance;
  }
  return distance;
}