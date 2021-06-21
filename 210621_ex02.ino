
#include<U8g2lib.h>
#include "DHT.h"
#include<SoftPWM.h>

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,/*reset=*/U8X8_PIN_NONE);

DHT dht(DHTPIN,DHTTYPE);

int Soilhumi=0;
SOFTPWM_DEFINE_CHANNEL(A3);

uint32_t DataCaptureDelay=3000;//3초마다
uint32_t DataCapture_ST=0;

float Temp;
float Humi;
float fan;
String onoff;

void setup() {
  // put your setup code here, to run once:
  pinMode(SOILHUMI,INPUT);
  u8g2.begin();
  SoftPWM.begin(490);
  onoff = "";

  DataCapture_ST=millis();
}

void OLEDdraw(){
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1,15,"Exercise 02");

  u8g2.drawStr(15,36,"Soilhumi.");
  u8g2.setCursor(85,36);
  u8g2.print(Soilhumi);
  u8g2.drawStr(114,36,"\xb0");
  u8g2.drawStr(119,36,"%");

  u8g2.drawStr(15,47,"DC FAN.");
  u8g2.setCursor(85,47);
  u8g2.print(fan);
  u8g2.drawStr(116,47,"%");
  u8g2.setCursor(85,58);
  u8g2.print(onoff);

  u8g2.sendBuffer();
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()-DataCapture_ST)>DataCaptureDelay){
    Soilhumi=map(analogRead(SOILHUMI),0,1023,100,0);

    if(isnan(Soilhumi)){
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    if(Soilhumi>=30){
      SoftPWM.set(100);
      fan=100;
      onoff = "ON";
    }
    else if(Soilhumi<=25){
      SoftPWM.set(0);
      fan=0;
      onoff = "OFF";
    }
    else 
    {
      SoftPWM.set(65); 
      fan=65;
      onoff = "ON";
    }
    
    OLEDdraw();
    DataCapture_ST=millis();
  }
}
