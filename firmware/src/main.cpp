#include <Arduino.h>


//******************* PANTALLA OLED ********************//
#include "SSD1306Wire.h"
#include "pantallaOled.h"
SSD1306Wire display(0x3c, 21, 20, GEOMETRY_128_32);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h










//***************** WIFI MANAGER **********************//
#include <wifimanager.h>
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
#include "sense.h"
#include <Wire.h>
#define PWMPIN 6
int PWM = 0;
float amps, volts, watts;
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

/*String readAmps() {
  float Vref = 1.1;
  float amplification = 4.0;
  float resistance = 0.05;

  float Vadc = (Vref / 4095.0) * analogRead(1); // Calcula el voltaje en la entrada analógica
  float voltage = Vadc /4; // Calcula el voltaje real en el divisor de voltaje
  amps = voltage / resistance;

  return String(amps);
}

String readvolts() {
  float Vref = 1.1; // Tensión de referencia del ADC
  float R1 = 25500.0; // Valor de la resistencia R1 del divisor de voltaje
  float R2 = 1000.0; // Valor de la resistencia R2 del divisor de voltaje
  float bits = analogRead(3)*1.02;
  float Vadc = (Vref / 4095.0) * bits; // Calcula el voltaje en la entrada analógica
  volts = Vadc * (R1 + R2) / R2; // Calcula el voltaje real en el divisor de voltaje
  return String(volts);
}

String readPower() {
  return String(volts*amps);
}
*/





//******************** THERMISTOR ***********************

float temperature;


//******************** Gate switch ***********************
#define gateSwitchPin 7
bool gateSwitch;


//******************** Selector ***********************

#include <RotaryEncoder.h>
const int pinSelectorMoveA = 10;
const int pinSelectorMoveB = 8;
const int pinSelectorButton = 9;
const int timeThreshold = 5;
long timeCounter = 0;

const int maxSteps = 255;
volatile int ISRCounter = 0;
int counter = 0;
bool button;

bool IsCW = true;

void doEncode()
{
  /*if (millis() > timeCounter + timeThreshold){
    if (digitalRead(pinSelectorMoveA) == digitalRead(pinSelectorMoveB)){
      ISRCounter++;
    }else{
      ISRCounter--;
    }
    timeCounter = millis();
  }*/
    

  if (millis() > timeCounter + timeThreshold)
  {
    if (digitalRead(pinSelectorMoveA) == digitalRead(pinSelectorMoveB))
    {
      IsCW = true;
      if (ISRCounter + 1 <= maxSteps) ISRCounter++;
    }
    else
    {
      IsCW = false;
      if (ISRCounter - 1 > 0) ISRCounter--;
    }
    PWM = ISRCounter;
    analogWrite(PWMPIN,PWM);
    timeCounter = millis();
  }
}
bool botoInici = false;
void encoderButton(){
  if (millis() > timeCounter + timeThreshold){
    button = true;
    timeCounter = millis();
  }
  //botoInici=true;
}



//******************** Timer interruption ***********************
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


String hostname = "my-esp32";

void setup() {
  
  Serial.begin(115200);
  Serial.println("holaa");
  
  display.init();
  display.flipScreenVertically();   //flip display option !!!
  for(int i = 0; i<101; i++){
    display.drawProgressBar(10, 15, 98, 10, i);
    display.display();
    delay(10);
  }

  pinMode(9, INPUT);
  pinMode(PWMPIN, OUTPUT);  //pwm
  pinMode(0, OUTPUT);       //fan
  //digitalWrite(0, HIGH);
  pinMode(pinSelectorMoveA, INPUT);
  pinMode(pinSelectorMoveB, INPUT);
  pinMode(pinSelectorButton, INPUT);
  pinMode(gateSwitchPin, INPUT);



  analogSetAttenuation(ADC_2_5db);  //https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/


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
  Serial.println(ssid);
  Serial.println(pass);



  if(initWiFi(ssid, pass, previousMillis, interval)) {  //******************************
    // Route for root / web page
      
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });
    server.serveStatic("/", SPIFFS, "/");
    
    server.on("/readA", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(amps).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readV", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(volts).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readP", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(watts).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/readT", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(temperature).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(amps).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/tension", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(volts).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(watts).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
      //digitalWrite(ledPin, HIGH);
      request->send_P(200, "text/plain", String(temperature).c_str());  //"/graphic.html", "text/html", numerorandom().c_str());
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
      request->send(200, "text/plain", "Done. ESP will restart, connect to your router. Connecto to IP indicated on oled screen.");// + ip);
      delay(1000);
      ESP.restart();
    });
    server.begin();
  }



  currentTime = millis();

  delay(1000);
  //analogWrite(PWMPIN,10);
  IPAddress myIP = WiFi.localIP();
  String adresa = String(myIP[0]) + String(".") + String(myIP[1]) + String(".") + String(myIP[2]) + String(".") + String(myIP[3])  ;
  imprimeixOled(adresa, display);  //String(random(10)), display);
  
  while(digitalRead(pinSelectorButton)){}
  //botoInici = false;
  // encoder
  //encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors

  attachInterrupt(digitalPinToInterrupt(pinSelectorMoveA), doEncode, RISING);  //call encoderISR()       every high->low or low->high changes
  //attachInterrupt(digitalPinToInterrupt(pinSelectorMoveB), doEncode, RISING);
  attachInterrupt(digitalPinToInterrupt(pinSelectorButton), encoderButton, FALLING); //call encoderButtonISR() every high->low              changes
  
  

  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
  
}

void loop(){

  readAmps(&amps);
  readvolts(&volts);
  readPower(&volts, &amps, &watts);
  readTemp(&temperature);
  readSwitch(&gateSwitch, digitalRead(gateSwitchPin));

  imprimeixOledValors(&amps, &volts, &watts, &gateSwitch, display);

  Serial.println(">");
  if(temperature > 50) digitalWrite(0, HIGH);
  if(temperature > 80) analogWrite(PWMPIN,0); 
  if(temperature < 40) digitalWrite(0, LOW);
  if(temperature < 60) analogWrite(PWMPIN,PWM);


  
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

  
  
  
  delay(100);

}

