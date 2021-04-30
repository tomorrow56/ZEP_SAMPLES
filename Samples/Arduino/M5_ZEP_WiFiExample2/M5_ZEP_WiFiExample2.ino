#include <M5Stack.h>

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include "WiFiManager.h"   // https://github.com/zhouhan0126/WIFIMANAGER-ESP32

WiFiManager wifiManager;
WiFiClient client;
IPAddress ipadr;

//wifitest.adafruit.com/testwifi/index.html
const char* host = "wifitest.adafruit.com";
const String page = "/testwifi/index.html";
const int port = 80;

void setup(){
  // init lcd, serial, but don't init sd card
  M5.begin(true, false, true);
  M5.Power.begin();

  M5.Lcd.setBrightness(128);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5Stack Wi-Fi Test2");

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.println();
  //connect to WiFi
  M5.Lcd.println("AP Name: M5StackAP");
  M5.Lcd.println("IP adrs: 192.168.4.1");
  wifiManager.autoConnect("M5StackAP");

  M5.Lcd.println("CONNECTED");
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.println();
  M5.Lcd.println("SSID:" + (String)WiFi.SSID());
  ipadr = WiFi.localIP();
  M5.Lcd.println("IP adrs: " + (String)ipadr[0] + "." + (String)ipadr[1] + "." + (String)ipadr[2] + "." + (String)ipadr[3]);
  M5.Lcd.println();

  // Use WiFiClient class to create TCP connections
  if (!client.connect(host, port)) {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println("connection failed");
    return;
  }

  M5.Lcd.setTextColor(CYAN);
  M5.Lcd.println("HTTP://" + (String)host + (String)page);
  M5.Lcd.println();

  String request = "GET ";
  request += page;
  request += " HTTP/1.1\r\nHost: ";
  request += host;
  request += "\r\n\r\n";
  client.print(request);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()) {
    String line = client.readStringUntil('\r');
    M5.Lcd.print(line);
  }

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){ 

}
