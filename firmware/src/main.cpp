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







//********************** SENSING *******************
//#include "sense.h"
#include <Wire.h>
#define PWMPIN 6
int PWM = 0;
float amps;
float volts;
unsigned long currentTime;

/*String readAmps() {
  float sumaDeu;
  for(int i=10;i>0;i--){
    amps = 4*analogRead(1)*3.3/4095;
    sumaDeu += amps;
  }
  amps = sumaDeu/10;
  return String(amps);
}*/

String readAmps() {
  //float sumaDeu;
  //for(int i=0;i<10;i++){
    amps = 4*analogRead(1)*3.3/4095;
  //  sumaDeu += amps;
  //  delay(100);
  //}
  //amps = sumaDeu/10;
  return String(amps);
}

String readvolts() {
  //float sumaDeu;
  //for(int i=0;i<10;i++){
    volts = 24*analogRead(3)*3.3/4095;      //26.18 ---1261
    Serial.println(analogRead(3));
  //  sumaDeu += volts;
  //  delay(100);
  //}
  //volts = sumaDeu/10;
  return String(volts);
}

String readPower() {
  return String(volts*amps);
}






//******************** THERMISTOR ***********************

float temp, adc, volt, resis, ohms, k;

#define THERMISTOR_PIN 4
#define REFERENCE_RESISTANCE 200000
#define REFERENCE_VOLTAGE 1.0
#define THERMISTOR_NOMINAL 100000
#define TEMPERATURE_NOMINAL 25.0
#define B_COEFFICIENT 3950

String readTemp() {
  float sumaDeu;
  for(int i=10;i>0;i--){    
    int rawValue = analogRead(THERMISTOR_PIN);
    float voltage = rawValue * REFERENCE_VOLTAGE / 4095.0;
    float resistance = REFERENCE_RESISTANCE * voltage / (REFERENCE_VOLTAGE - voltage);

    float steinhart;
    steinhart = resistance / THERMISTOR_NOMINAL;      // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= B_COEFFICIENT;                       // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart - 273.15;             // Invertir y convertir a Celsius
    sumaDeu += steinhart;
  }

  float final = sumaDeu/10;

  return String(final);
}




String hostname = "my-esp32";

void setup() {
  
  Serial.begin(115200);
  Serial.println("holaa");
  
  display.init();
  display.flipScreenVertically();   //flip display option !!!
  display.display();
  delay(200);

  pinMode(9, INPUT);
  pinMode(PWMPIN, OUTPUT);  //pwm

  //WiFi.setHostname("my-esp32");
  WiFi.setHostname(hostname.c_str());


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
    
    server.on("/readA", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", /*String(amps).c_str()*/readAmps().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readV", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", /*String(volts).c_str()*/readvolts().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readP", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readPower().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readT", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readTemp().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readAmps().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/tension", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readvolts().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readPower().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", readTemp().c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

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
      delay(1000);
      ESP.restart();
    });
    server.begin();
  }








  currentTime = millis();

  delay(1000);
  //analogWrite(PWMPIN,10);
}

void loop(){

  if ((currentTime +1000) <= millis()){
    //Serial.print(i++);
    //Serial.print(" - ");
    IPAddress myIP = WiFi.localIP();
    String adresa = String(myIP[0]) + String(".") + String(myIP[1]) + String(".") + String(myIP[2]) + String(".") + String(myIP[3])  ;
    imprimeixOled(adresa, display);  //String(random(10)), display);
    Serial.print(WiFi.localIP());
    Serial.print("  ::  ");

    Serial.print("PWM: ");
    Serial.print(PWM);
    Serial.print(" / Volts: ");
    Serial.print(25.5*analogRead(3)*3.3/4095);  
    Serial.print(" / Amps: ");
    Serial.println(4*analogRead(1)*3.3/4095);
    
    /*Serial.print("  *********************  ");
    adc = analogRead(4) & 4095;
    Serial.print(" / ");
    Serial.print(adc);
    ohms= 100000 * ((4095 / adc)-1);
    k = 0.25;
    temp=k*ohms;
    Serial.print(" / ");
    Serial.println(temp/1000);*/


    /*Serial.print("  ------------------->>  ");
    int rawValue = analogRead(THERMISTOR_PIN);
    float voltage = rawValue * REFERENCE_VOLTAGE / 4095.0;
    float resistance = REFERENCE_RESISTANCE * voltage / (REFERENCE_VOLTAGE - voltage);

    float steinhart;
    steinhart = resistance / THERMISTOR_NOMINAL;      // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= B_COEFFICIENT;                       // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart - 273.15;             // Invertir y convertir a Celsius

    Serial.print("Temperature: ");
    Serial.print(steinhart);
    Serial.println(" C");*/



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
    currentTime = millis();
  }







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
  
  
  //delay(1000);

}

