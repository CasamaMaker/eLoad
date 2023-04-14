#include <Arduino.h>


//******************* PANTALLA OLED ********************//
#include "SSD1306Wire.h"
#include "pantallaOled.h"
SSD1306Wire display(0x3c, 21, 20, GEOMETRY_128_32);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h










//***************** WIFI MANAGER **********************//
#include <wifimanager.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";

//Variables to save values from HTML form
String ssid;
String pass;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";

IPAddress localIP;

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

// Set LED GPIO
const int ledPin = 2;
// Stores LED state

String ledState;
//***********************************************************



//****************** DIGITAL LED ******************************
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 2
#define BRIGHTNESS  15
CRGB leds[NUM_LEDS];
//***********************************************************







//********************** WEB WITH GRAPHIC *******************

#include <Wire.h>

long int randNumber;






int i = 0;

String p;
float t;
float v;


#define PWMPIN 6
int PWM = 0;

String readBME280Temperature() {
  // Read temperature as Celsius (the default)
  /*t = t + random(-2, 2)/10.0;
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }*/

  //Serial.print(" - amps: ");
  t = 4*analogRead(1)*3.3/4095;
  return String(t);
}


String randomVolts() {
  // Read temperature as Celsius (the default)
  /*v = v + random(-2, 2)/10.0;
  if (isnan(v)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "";
  }
  else {
    Serial.println(v);
    return String(v);
  }*/

  //Serial.print(" - volts: ");
  v = 25.5*analogRead(3)*3.3/4095;  

  return String(v);
}




void setup() {
  
  Serial.begin(115200);
  Serial.println("holaa");
  
  display.init();
  display.flipScreenVertically();   //flip display option !!!
  display.display();
  delay(200);

  pinMode(9, INPUT);
  pinMode(PWMPIN, OUTPUT);  //pwm





  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);      //NEOPIXEL
  //leds[0] = CRGB::Green;
  //FastLED.setBrightness( BRIGHTNESS );
  //FastLED.show();
  //FastLED.delay(8);




  initSPIFFS();

  // Set GPIO 2 as an OUTPUT
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  //ip = readFile(SPIFFS, ipPath);
  //gateway = readFile (SPIFFS, gatewayPath);
  Serial.println(ssid);
  Serial.println(pass);
  //Serial.println(ip);
  //Serial.println(gateway);

  if(initWiFi(ssid, pass, previousMillis, interval)) {  //******************************
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });
    server.serveStatic("/", SPIFFS, "/");
    
    // Route to set GPIO state to HIGH
    server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readBME280Temperature().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/tension", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", randomVolts().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    // Route to set GPIO state to LOW
    /*server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
      digitalWrite(ledPin, LOW);
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });*/
    server.begin();
  }
  else {
    // Connect to Wi-Fi network with SSID and password ************************************
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("ESP-WIFI-MANAGER", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(SPIFFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(SPIFFS, passPath, pass.c_str());
          }
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + String(WiFi.localIP()));// + ip);
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }










  delay(1000);
  //analogWrite(PWMPIN,10);
}

void loop(){
  Serial.print(i++);
  Serial.print(" - ");
  //imprimeixOled("Num: " + String(i), display);  //String(random(10)), display);
  Serial.println(WiFi.localIP());
  
  
  
  
  



/*
  if(!digitalRead(9)){
    ssid = "reset";
    Serial.println("SSID deleted");
    writeFile(SPIFFS, ssidPath, ssid.c_str());

    pass = "reset";
    Serial.println("PASS deleted"); 
    writeFile(SPIFFS, passPath, pass.c_str());

    ESP.restart();
  }*/

/*
  leds[0] = CRGB::Black;
  FastLED.show();
  //FastLED.delay(500);
  FastLED.setBrightness( BRIGHTNESS );
  delay(500);
 
  leds[0] = CRGB::Red;
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.show();
  //FastLED.delay(500);
  delay(500);
  */








  if (Serial.available() > 0) {
    int incomingvalue = Serial.parseInt();    
    if(incomingvalue > 0){
      if(incomingvalue == 300){
        PWM = 0;
      }else{
        PWM = incomingvalue;
      } 
      analogWrite(PWMPIN,PWM);           
    }
    
  }
  

  Serial.print(PWM);
  Serial.print(" - volts: ");
  Serial.print(25.5*analogRead(3)*3.3/4095);  
  Serial.print(" - amps: ");
  Serial.println(4*analogRead(1)*3.3/4095);
  delay(1000);

}

