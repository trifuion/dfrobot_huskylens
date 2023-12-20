#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
#define BUZZER_PIN 12 

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);
String xString = "";
int xInteger = 0;
String yString = "";
int yInteger = 0;
int multiply = 0;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    pinMode(BUZZER_PIN, OUTPUT);

    display.begin();
    display.setContrast(60);  // Initial contrast set to 60
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.print("Hello, Peticel!");
    display.display();
    Wire.begin();
}

void loop() {
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    //else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        //Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter);
        xString = result.xCenter;
        xInteger = (xString.toInt());
        //Serial.println(xInteger);
        yString = result.yCenter;
        yInteger = (yString.toInt());
        //Serial.println(yInteger);
      if (result.ID==1 && 90 < xInteger && xInteger < 180 && 80 < yInteger && yInteger < 160) {
        display.clearDisplay();
        
        multiply = xInteger*yInteger*5;
        //Serial.println(xInteger);
        //Serial.println(yInteger);
        //Serial.println(multiply);
        Serial.println("in centru");
        
        tone(BUZZER_PIN, multiply);  // 1000 Hz frequency (adjust as needed)
        display.setCursor(xInteger, yInteger);
        display.print("MASINA");
        display.setCursor(xInteger, yInteger+10);
        display.print("X: ");
        display.print(result.xCenter);
        display.setCursor(xInteger, yInteger+20);
        display.print("Y: ");
        display.print(result.yCenter);
        display.display();
        
        }
        //Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}