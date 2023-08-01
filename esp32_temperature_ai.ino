#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include<WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN 13     
#define TFT_CS 15
#define TFT_DC 2

const char* ssid = "";
const char* pass = "";

unsigned const long interval = 5000;
unsigned long zero = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  tft.begin();
  dht.begin();
  WiFi.begin(ssid, pass);

  // title
  write_lcd(62,80,"BLUE","JAGO",5);
  write_lcd(39,130,"BLUE","INSTITUTE",3);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
  }
  
  // subtitle
  write_lcd(33,168,"WHITE","BARD (AI) READY",2);
  delay(2500);

  // body
  write_lcd(48,198,"RED","Let's Start!",2);
  delay(2000);

  clear_lcd();
}

void loop() { 
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  write_lcd(27,70,"GREEN","DATA SENSOR",3);

  write_lcd(20,130,"WHITE","Suhu :",2);
  write_lcd(100,130,"WHITE",String(temperature),2);

  write_lcd(20,160,"WHITE","Kelembaban :",2);
  write_lcd(170,160,"WHITE",String(humidity),2);

  write_lcd(20,300,"RED","by Jago Institute",2);
  delay(2500);

  if(millis()-zero > interval){
    String resp_id = post_bard_google(temperature,humidity);
    String resp = get_bard_google(resp_id);
    clear_lcd();
    write_lcd(36,2,"RED","BARD (AI) SAYS",2);
    write_lcd(20,33,"WHITE",String(resp),2);
    delay(10000);
  }
  clear_lcd();
}

String post_bard_google(int temperature, int humidity) {
  HTTPClient http;
  String url = "http://jagoinstitute.pythonanywhere.com/async/bard?chat=suhu%20diruanganku%20" + String(temperature) + "derajat%20celcius%20dan%20" + String(humidity) +"%%20kelembaban,%20apakah%20lebih%20dingin%20dari%20suhu%20jakarta%20hari%20ini?%20beri%20aku%20saran%20singkat%20agar%20tidak%20sakit";
  http.begin(url);
  int httpResponCode = http.GET();
  String resp = http.getString();
  return resp;
}

String get_bard_google(String chat_id) {
  HTTPClient http;
  String url = "https://jagoinstitute.pythonanywhere.com/bard?id=" + String(chat_id);
  http.begin(url);
  int httpResponCode = http.GET();
  String resp = http.getString();
  return resp;
}

void write_lcd(int x, int y, String color, String text, int size) {
  tft.setCursor(x, y);
  if (color == "RED") {
    tft.setTextColor(ILI9341_RED);
  } else if (color == "BLUE") {
    tft.setTextColor(ILI9341_BLUE);
  } else if (color == "GREEN") {
    tft.setTextColor(ILI9341_GREEN);
  } else if (color == "ORANGE") {
    tft.setTextColor(ILI9341_ORANGE);
  } else {
    tft.setTextColor(ILI9341_WHITE);
  }
  tft.setTextSize(size);
  tft.println(text);
}

void clear_lcd() {
  tft.fillScreen(ILI9341_BLACK);
}