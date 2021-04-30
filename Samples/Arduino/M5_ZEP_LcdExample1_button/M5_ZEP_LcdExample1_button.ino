/*
    Name:       M5_button_example
    Created:    2021/03/09
    Author:     @tomorrow56
*/

#include <M5Stack.h>
void setup() {
  // M5Stack::begin(LCDEnable, SDEnable, SerialEnable, I2CEnable);
  M5.begin(true, false, true, false);
  M5.Power.begin();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(85, 10);
  M5.Lcd.println("LCD Example");

  M5.Lcd.setTextColor(CYAN);
  M5.Lcd.drawCentreString("drawCentreString", 160, 100, 2);

  M5.Lcd.setTextColor(MAGENTA);
  M5.Lcd.drawRightString("drawRightString", 319, 150, 2);
}

void loop() {
  M5.update();

  // M5.BtnA/B/C.pressedFor(uint32_t ms, uint32_t continuous_time);
  // M5.BtnA/B/C.wasReleasefor(uint32_t ms);
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0, 0);
    for(int i = 1; i <= 7; i++){
      M5.Lcd.setTextSize(i);
      M5.Lcd.println("Size:" + (String)i);
    }
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(50, 10);
    M5.Lcd.println("Progress Bar example");
    for(int i = 0; i <= 10; i++){
      M5.Lcd.progressBar(20, 180, 280, 30, i * 10);
      delay(500);
    }
  } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(65, 10);
    M5.Lcd.println("QR Code example");
    M5.Lcd.qrcode("https://www.zep.co.jp/index.html", 85, 45, 150, 6);
  } else if (M5.BtnA.wasReleasefor(700)) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(85, 10);
    M5.Lcd.println("LCD Example");

    M5.Lcd.setTextColor(CYAN);
    M5.Lcd.drawCentreString("drawCentreString", 160, 100, 2);

    M5.Lcd.setTextColor(MAGENTA);
    M5.Lcd.drawRightString("drawRightString", 319, 150, 2);
  }
}
