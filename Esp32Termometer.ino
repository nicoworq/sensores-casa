/*
 Display all the fast rendering fonts.

 This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8
 
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 2000

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

 
#define SEALEVELPRESSURE_HPA (1013.25)

const char* ssid     = "Fibertel WiFi006 2.4GHz";       ///EDIIIT
const char* password = "0043300524"; //EDI8IT
const char* mqtt_server = "198.74.55.112";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

Adafruit_BME280 bme;

 char msg[50];

int delaySensing = 180000;
int delayMqtt = 600000;

unsigned long timeNowSensing = 0;
unsigned long timeNowMqtt = 0;

float temp = 0;
float hum = 0;
float presion = 0;

 
void setup(void) {

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  
  Serial.begin(115200);

  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76); 
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }


  while (!Serial) {
    ;  // wait for serial port to initialize
  }
  
  Serial.println("setup");
   
  tft.init();
  tft.setRotation(0);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
   
  tft.print("Connecting to ");
  tft.println(ssid);
  /*
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  }
  */
  Serial.println("connected");
  
  tft.println("");
  tft.println("WiFi connected.");
  tft.println("IP address: ");
  //tft.println(WiFi.localIP());
  //delay(3000);
  tft.fillScreen(TFT_BLACK);

  Serial.println("end setup");
}

void loop() {
 
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
 

     if(millis() > timeNowSensing + delaySensing){
        timeNowSensing = millis();
        // leer data
        getData();
        
    }

      if(millis() > timeNowMqtt + delayMqtt){
        timeNowMqtt = millis();
        // enviar data
         sendData();
    }
    
   
}

float tempMax = 0;
float tempMin = 200;

void getData(){
    tft.setCursor(0, 0, 2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // temp
     temp = bme.readTemperature();

    if(temp > tempMax){
      tempMax = temp;
    }

    if(temp < tempMin){
      tempMin = temp;
    }
    tft.println("Temp");
    tft.print(temp);
    tft.print(" *C");
    Serial.println(temp);

    //hum        

    hum = bme.readHumidity();
    tft.println("");
    tft.println("Hum:");
    tft.print(hum);
    tft.print(" %");
    Serial.println(hum);


    //presion        

    presion = bme.readPressure();
   
    Serial.println(presion);

    tft.println("");
    tft.println("Min/Max:");
    tft.print(tempMin);
    tft.println(" *C");
    tft.print(tempMax);
    tft.print(" *C");
}

void sendData(){
  // enviar data
  
  Serial.println("mqttSend");
  char msg_out[20]; 
  
  //temp
  dtostrf(temp,2,2,msg_out);
  Serial.println(msg_out);
  mqttClient.publish("/casa/temperatura", msg_out);

//hum
  dtostrf(hum,2,2,msg_out);
  Serial.println(msg_out);
  mqttClient.publish("/casa/humedad", msg_out);

//presion
  dtostrf(presion/100,2,2,msg_out);
  Serial.println(msg_out);
  mqttClient.publish("/casa/presion", msg_out);
  
}

 void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("Weather1")) {

      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic", "hello world");
      // ... and resubscribe
       
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

 void setup_wifi() {
 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
}

 
