/*
    Name:       M5_display_example
    Created:    2021/03/09
    Author:     @tomorrow56
*/

#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup() {
  
  // M5Stack::begin(LCDEnable, SDEnable, SerialEnable, I2CEnable);
  M5.begin(true, false, false, false);
  M5.Power.begin();
  
  // Lcd display
  M5.Lcd.fillScreen(WHITE);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  // draw graphic
  delay(1000);
  M5.Lcd.drawRect(100, 100, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.fillRect(100, 100, 50, 50, BLUE);
  delay(1000);
  M5.Lcd.drawCircle(100, 100, 50, RED);
  delay(1000);
  M5.Lcd.fillCircle(100, 100, 50, RED);
  delay(1000);
  M5.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(1000);
  M5.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(1000);
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  M5.Lcd.fillTriangle(random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(0xfffe));

  M5.update();
}
