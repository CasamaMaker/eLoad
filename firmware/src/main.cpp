#include <Arduino.h>


//******************* PINOUT ********************//
#define FAN_PIN               0
#define ISENSE_PIN            1
#define DIGITAL_LED_PIN       2
#define VSENSE_PIN            3 
#define TSENSE_PIN            4
#define MOSFET_PIN            6
#define GATE_SWITCH_PIN       7
#define PWM_PIN               6
#define SELECTOR_MOVE_B_PIN   8
#define SELECTOR_BUTTON_PIN   9
#define SELECTOR_MOVE_A_PIN   10


//******************* PANTALLA OLED ********************//
#include "SSD1306Wire.h"
#include "pantallaOled.h"
SSD1306Wire display(0x3c, 21, 20, GEOMETRY_128_32);


//***************** WIFI MANAGER **********************//
#include <wifimanager.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
AsyncWebServer server(80);  // Create AsyncWebServer object on port 80
const char* PARAM_INPUT_1 = "ssid";// Search for parameter in HTTP POST request
const char* PARAM_INPUT_2 = "pass";
String ssid;//Variables to save values from HTML form
String pass;
const char* ssidPath = "/ssid.txt";// File paths to save input values permanently
const char* passPath = "/pass.txt";
IPAddress localIP;
unsigned long previousMillis = 0;// Timer variables
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)


//****************** DIGITAL LED ******************************
/*#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 2
#define BRIGHTNESS  15
CRGB leds[NUM_LEDS];*/


//********************** SENSING *******************
#include "sense.h"
#include <Wire.h>
int PWM = 0;
float amps, volts, watts;   //eload measurements
unsigned long currentTime;
float temperature;          //ntc
bool gateSwitch;            //switch sense
#include <RotaryEncoder.h>
#define timeThreshold 5
long timeCounter = 0;
const int maxSteps = 255;
volatile int ISRCounter = 0;
int counter = 0;
bool button;
bool IsCW = true;


//******************** SELECTOR ***********************
void doEncode(){
  if (millis() > timeCounter + timeThreshold){
    if (digitalRead(SELECTOR_MOVE_A_PIN) == digitalRead(SELECTOR_MOVE_B_PIN)){
      IsCW = true;
      if (ISRCounter + 1 <= maxSteps) ISRCounter++;
    }else{
      IsCW = false;
      if (ISRCounter - 1 > 0) ISRCounter--;
    }
    PWM = ISRCounter;
    analogWrite(PWM_PIN,PWM);
    timeCounter = millis();
  }
}

bool botoInici = false;
void encoderButton(){
  if (millis() > timeCounter + timeThreshold){
    button = true;
    timeCounter = millis();
  }
}



//******************** TIMER INTERRUPTION ***********************
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     4
#include "ESP32_C3_TimerInterrupt.h"

bool IRAM_ATTR TimerHandler0(void * timerNo)
{
	/*static bool toggle0 = false;

	//timer interrupt toggles pin PIN_D39
	digitalWrite(PIN_D39, toggle0);
	toggle0 = !toggle0;
*/
	return true;
}

#define TIMER0_INTERVAL_MS        1000   //1000
ESP32Timer ITimer0(0);


//******************** SETUP ****************************** SETUP **************************** SETUP **********************************
void setup() {
  
  Serial.begin(115200);       //Serial config

  display.init();             //oled config
  display.flipScreenVertically();   //flip display option !!!

  pinMode(FAN_PIN, OUTPUT);       //Cooling fan
  pinMode(PWM_PIN, OUTPUT);  //pwm
  
  //digitalWrite(0, HIGH);
  pinMode(SELECTOR_MOVE_A_PIN, INPUT);
  pinMode(SELECTOR_MOVE_B_PIN, INPUT);
  pinMode(SELECTOR_BUTTON_PIN, INPUT);
  pinMode(GATE_SWITCH_PIN, INPUT);

  analogSetAttenuation(ADC_2_5db);  //https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/


  //WiFi.setHostname("my-esp32"); //not work

  //FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);      //NEOPIXEL
  //leds[0] = CRGB::Green;
  //FastLED.setBrightness( BRIGHTNESS );
  //FastLED.show();
  //FastLED.delay(8);


//******************** SPIFFS ***********************
  initSPIFFS();
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  //Serial.println(ssid);
  //Serial.println(pass);


//****************************** WEB PAGE ***********************
  if(initWiFi(ssid, pass, previousMillis, interval)) {  
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });

    server.serveStatic("/", SPIFFS, "/");

    server.on("/readA", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(amps).c_str());
    });

    server.on("/readV", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(volts).c_str());
    });

    server.on("/readP", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(watts).c_str());
    });

    server.on("/readT", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(temperature).c_str());
    });

    server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(amps).c_str());
    });

    server.on("/tension", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(volts).c_str());
    });

    server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(watts).c_str());
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(temperature).c_str());
    });

    server.begin();
  }else{  // Creating AP wifi with web server
    

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
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            writeFile(SPIFFS, ssidPath, ssid.c_str());  // Write file to save value
          }
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            writeFile(SPIFFS, passPath, pass.c_str());  // Write file to save value
          }
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router. Connect to IP indicated on oled screen.");// + ip);
      delay(1000);
      ESP.restart();
    });
    server.begin();
  }


  delay(1000);
  currentTime = millis();
  

  IPAddress myIP = WiFi.localIP();
  String adresa = String(myIP[0]) + String(".") + String(myIP[1]) + String(".") + String(myIP[2]) + String(".") + String(myIP[3])  ;
  imprimeixOled(adresa, display);
  
  while(digitalRead(SELECTOR_BUTTON_PIN)){}


//******************** START INTERRUPTIONS ***********************
  attachInterrupt(digitalPinToInterrupt(SELECTOR_MOVE_A_PIN), doEncode, RISING);
  //attachInterrupt(digitalPinToInterrupt(SELECTOR_MOVE_B_PIN), doEncode, RISING);
  attachInterrupt(digitalPinToInterrupt(SELECTOR_BUTTON_PIN), encoderButton, FALLING);
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
  
}


//******************** LOOP ****************************** LOOP **************************** LOOP **********************************
void loop(){


//******************** MONITORING ***********************
  readAmps(&amps);
  readvolts(&volts);
  readPower(&volts, &amps, &watts);
  readTemp(&temperature);
  readSwitch(&gateSwitch, digitalRead(GATE_SWITCH_PIN));

  imprimeixOledValors(&amps, &volts, &watts, &gateSwitch, display);


//******************** PROTECTIONS ***********************
  if(temperature > 50) digitalWrite(FAN_PIN, HIGH);     //Start fan
  if(temperature > 80) analogWrite(PWM_PIN,0);          //Stop mosfet as load
  if(temperature < 40) digitalWrite(FAN_PIN, LOW);      //Stop fan
  if(temperature < 60) analogWrite(PWM_PIN,PWM);        //Restart mosfet
  if(watts > 65){
    PWM -=10;
    analogWrite(PWM_PIN,PWM);       //Restart mosfet
  }
  if(watts < 60) analogWrite(PWM_PIN,PWM);//Restart mosfet

  
//******************** SERIAL PWM ***********************
  if (Serial.available() > 0) {
    int incomingvalue = Serial.parseInt();    
    if(incomingvalue > 0){
      if(incomingvalue == 300){
        PWM = 0;
      }else{
        PWM = incomingvalue;
      } 
      analogWrite(PWM_PIN,PWM);           
    }
    
  }
  delay(100);
}

