/*Please install the < Adafruit BMP280 Library > （https://github.com/adafruit/Adafruit_BMP280_Library）
 * from the library manager before use. 
 *This code will display the temperature, humidity and air pressure information on the screen*/

#include <M5Stack.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

// for WiFi Manager
#include <WiFi.h>
#include <DNSServer.h>     // https://github.com/zhouhan0126/DNSServer---esp32
#include <WebServer.h>     // https://github.com/zhouhan0126/WebServer-esp32
#include "WiFiManager.h"   // https://github.com/zhouhan0126/WIFIMANAGER-ESP32

SHT3X sht30;
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

// WiFi instanse
WiFiManager wifiManager;
WiFiClient client;
IPAddress ipadr;

// IFTTT key and event triggers are  
const char* server = "maker.ifttt.com";  // Server URL
String makerKey = ""; // MakerKey of Webhooks
String makerEvent = "";   // Event name of Applet

File myFile;
String cfgFile = "/M5IFTTT.cfg";
String Event[3];

void setup() {
  // M5Stack::begin(LCDEnable, SDEnable, SerialEnable, I2CEnable);
  M5.begin(true, true, true, true);
  M5.Power.begin();
  //Wire.begin();

  M5.Lcd.setBrightness(64);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));

  while (!bme.begin(0x76)){  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }

  // WiFiManager debug message enabled
  wifiManager.setDebugOutput(false);

  // WiFiManager auto connect setting check
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("AP Name: M5StackAP");
  M5.Lcd.println("IP adrs: 192.168.4.1");
  wifiManager.autoConnect("M5StackAP");

  //if you get here you have connected to the WiFi
  ipadr = WiFi.localIP();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(0, 0);

  M5.Lcd.println("Wifi Connected!");
  M5.Lcd.println("");
  M5.Lcd.println(" SSID: " + WiFi.SSID());
  M5.Lcd.println(" IP adrs: " + (String)ipadr[0] + "." + (String)ipadr[1] + "." + (String)ipadr[2] + "." + (String)ipadr[3]);

  delay(500);

  // read IFTTT config from SD
  loadConfig(cfgFile);

  // set makerEvent from SD data
  makerEvent = Event[0];

  Serial.println("makerKey = " + makerKey);
  Serial.println("makerEvent = " + makerEvent);

  delay(1000);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(130, 220);
  M5.Lcd.print("IFTTT");
}

void loop() {
  M5.update();  // Key scan

  pressure = bme.readPressure();
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("Temperatura: %2.2f*C  Humidity: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n", tmp, hum, pressure);

  if(M5.BtnB.wasPressed()){
    sendIFTTT(makerEvent, String(tmp), String(hum), String(pressure));
    M5.Lcd.setTextColor(YELLOW, BLACK);
    M5.Lcd.drawCentreString("DATA SENDING...", 160, 180, 2);
    delay(1000);
    M5.Lcd.setTextColor(BLACK, BLACK);
    M5.Lcd.drawCentreString("DATA SENDING...", 160, 180, 2);
  }

  delay(100);
}

// #########################################################################
// send trigger and data to IFTTT
// #########################################################################
void sendIFTTT(String event, String value1, String value2, String value3){

  Serial.println("\nStarting connection to server...");
  if(!client.connect(server, 80)){
    Serial.println("Connection failed!");
    return;
  }else{
    Serial.println("Connected to " + String(server));

    // Make a HTTP request:
    String url = "/trigger/" + event + "/with/key/" + makerKey;
    url += "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;

    client.println("POST " + url + " HTTP/1.1");
    client.print("Host: ");
    client.println(server);

    Serial.println(url);
    delay(100);

    client.println("Connection: close");
    client.println();

    Serial.print("Waiting for response ");
    int count = 0;
    while (!client.available()){
      delay(50); //
      Serial.print(".");
      count++;
      if (count > 20 * 20){ // about 20s
        Serial.println("(send) failed!");
        return;
      }
    }

    // read incoming bytes from the server
    while (client.available()){
      char c = client.read();
      Serial.write(c);
    }

    // if the server's disconnected, stop the client:
    if (!client.connected()){
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
    }
  }
  
  client.stop();
  Serial.println("closing connection");
  Serial.println("");
}

// ##########
// load IFTTT config from SD
// ##########
void loadConfig(String cfgFile){
  String readLine;
  String dmyLine[255];
  String dmyItem;
  String dmyData;
  int lineCount = 0;

  myFile = SD.open(cfgFile);   // open the file for reading
  if (myFile) {                            // read from the file until EOF
    while (myFile.available()) {
      readLine.concat(char(myFile.read()));
      if(readLine.endsWith("\n")){
        dmyLine[lineCount] = readLine.substring(0, readLine.indexOf("\n", 0));
        readLine = "";
        lineCount++;
      }
    }
    myFile.close();                      // close the file

    // Extract settings from read data and assign to variables
    for(int i = 0; i < lineCount; i++){
      if(dmyLine[i].length() > 0){
        dmyItem = dmyLine[i].substring(0, dmyLine[i].indexOf("=", 0) - 1);
        dmyItem.trim();
        dmyData = dmyLine[i].substring(dmyLine[i].indexOf("=", 0) + 1, dmyLine[i].length());
        dmyData.trim();
        if(dmyItem == "makerKey"){
          makerKey = dmyData;
        }
        if(dmyItem == "Event1"){
          Event[0] = dmyData;
        }
        if(dmyItem == "Event2"){
          Event[1] = dmyData;
        }
        if(dmyItem == "Event3"){
          Event[2] = dmyData;
        }
      }
    }
  } else {   // if the file didn't open, print an error
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.print("error opening " + cfgFile);
  }
}
