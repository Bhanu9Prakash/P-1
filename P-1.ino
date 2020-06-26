//This code is edited by LocaLFarm Automation Team
//Controlling Appliances and monitoring sensor's data over Internet using Ubidots MQTT server

/****************************************
   Include Libraries
 ****************************************/
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal.h>
#define WIFISSID "Hacker" // Put your WifiSSID here
#define PASSWORD "ramesh@123" // Put your wifi password here
#define TOKEN "BBFF-mEhoNMW3eUgFBKn3laeOuU45nUwof9" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "LocaLFarm_Auto01" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
//it should be a random and unique ascii string and different from all other devices

/****************************************
   Define Constants
 ****************************************/
#define VARIABLE_LABEL "timer" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_ON "onval"
#define VARIABLE_LABEL_SUBSCRIBE_OFF "offval"// Assing the variable label
#define DEVICE_LABEL "esp32" // Assig the device label

#define ledPin 17 // Set the GPIO26 as RELAY

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char topicSubscribeOn[100];
char topicSubscribeOff[100];
// Space to store values to send
char str_sensor[10];
int dtOn = 1;
int dtOff = 1;
int butValOnH;
int butValOnL;
int butValOffH;
int butValOffL;

int butPinOnH = T7; //grey
int butPinOnL = T8;
int butPinOffH = T6; //grey
int butPinOffL = T5;

LiquidCrystal lcd(23, 22, 5, 18, 19, 21);
/****************************************
   Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
      client.subscribe(topicSubscribeOn);
      client.subscribe(topicSubscribeOff);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
//**********************ON LED FROM CLOUD********************************
void callbackfuck(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  char cmpOn[] = "/v1.6/devices/esp32/onval/lv";
  char cmpOff[] = "/v1.6/devices/esp32/offval/lv";
  if (strcmp(topic, cmpOn) == 0) {
    dtOn = message.toInt();
  }
  if (strcmp(topic, cmpOff) == 0) {
    dtOff = message.toInt();
  }
  Serial.write(payload, length);
  Serial.println();
}
//*******************************************************************************

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as INPUT
  pinMode(ledPin, OUTPUT);
  pinMode(butPinOnH, INPUT_PULLUP);
  pinMode(butPinOnL, INPUT_PULLUP);
  pinMode(butPinOffH, INPUT_PULLUP);
  pinMode(butPinOffL, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);

  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callbackfuck);

  sprintf(topicSubscribeOn, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_ON);
  sprintf(topicSubscribeOff, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_OFF);
  client.subscribe(topicSubscribeOn);
  client.subscribe(topicSubscribeOff);
}
void loop() {
  for (unsigned int i = 0 ; i < dtOn * 600 ; i++) {
    if (!client.connected()) {
      client.subscribe(topicSubscribeOn);
      client.subscribe(topicSubscribeOff);
      reconnect();
    }
    //reading the button value
    butValOnH = touchRead(butPinOnH);
    butValOnL = touchRead(butPinOnL);
    butValOffH = touchRead(butPinOffH);
    butValOffL = touchRead(butPinOffL);

    //checking if the button is pressed
//        if(butValOnH<10){
//          dtOn+=1;
//        }
//        if(butValOnL<10){
//          dtOn-=1;
//        }
//        if(butValOffH<10){
//          dtOff+=1;
//        }
//        if(butValOffL<10){
//          dtOff-=1;
//        }

    //setting the limit (1,109)min
    if (dtOn < 1) {
      dtOn = 1;
    }
    if (dtOn > 109) {
      dtOn = 109;
    }
    if (dtOff < 1) {
      dtOff = 1;
    }
    if (dtOff > 109) {
      dtOff = 109;
    }
    if (i % 599 == 0) {
      sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", ""); // Cleans the payload
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
      int sensor = i / 599;
      Serial.print("Value of Sensor is:- ");
      Serial.println(sensor);
      /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
      dtostrf(sensor, 4, 0, str_sensor);
      sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
      Serial.println("Publishing data to Ubidots Cloud");
      Serial.println(payload);
      client.publish(topic, payload);
      delay(100);
    }
    client.loop();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ON           OFF");
    lcd.setCursor(3, 0);
    lcd.print("<-");
    lcd.setCursor(6, 0);
    lcd.print(i / 600);
    lcd.setCursor(0, 1);
    lcd.print(dtOn);
    lcd.setCursor(3, 1);
    lcd.print("min");
    lcd.setCursor(9, 1);
    lcd.print(dtOff);
    lcd.setCursor(12, 1);
    lcd.print("min");

    Serial.println("OnLoop");
    Serial.print(dtOn);
    Serial.print("-");
    Serial.print(dtOff);
    Serial.print("-");
    Serial.println(i / 10);

    //setting the led HIGH for 100 milli seconds
    digitalWrite(ledPin, HIGH);
    delay(100);
  }

  for (unsigned int i = 0 ; i < dtOn * 600 ; i++) {
    if (!client.connected()) {
      client.subscribe(topicSubscribeOn);
      client.subscribe(topicSubscribeOff);
      reconnect();
    }
    
    //reading the button value
    butValOnH = touchRead(butPinOnH);
    butValOnL = touchRead(butPinOnL);
    butValOffH = touchRead(butPinOffH);
    butValOffL = touchRead(butPinOffL);

    //checking if the button is pressed
//        if(butValOnH<10){
//          dtOn+=1;
//        }
//        if(butValOnL<10){
//          dtOn-=1;
//        }
//        if(butValOffH<10){
//          dtOff+=1;
//        }
//        if(butValOffL<10){
//          dtOff-=1;
//        }

    //setting the limit (1,109)min
    if (dtOn < 1) {
      dtOn = 1;
    }
    if (dtOn > 109) {
      dtOn = 109;
    }
    if (dtOff < 1) {
      dtOff = 1;
    }
    if (dtOff > 109) {
      dtOff = 109;
    }
    if (i % 599 == 0) {
      sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
      sprintf(payload, "%s", ""); // Cleans the payload
      sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
      int sensor = -1 * (i / 599);
      Serial.print("Value of Sensor is:- ");
      Serial.println(sensor);
      /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
      dtostrf(sensor, 4, 0, str_sensor);
      sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
      Serial.println("Publishing data to Ubidots Cloud");
      Serial.println(payload);
      client.publish(topic, payload);
    }
    client.loop();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ON           OFF");
    lcd.setCursor(11, 0);
    lcd.print("->");
    lcd.setCursor(6, 0);
    lcd.print(i / 600);
    lcd.setCursor(0, 1);
    lcd.print(dtOn);
    lcd.setCursor(3, 1);
    lcd.print("min");
    lcd.setCursor(9, 1);
    lcd.print(dtOff);
    lcd.setCursor(12, 1);
    lcd.print("min");

    Serial.println("OffLoop");
    Serial.print(dtOn);
    Serial.print("-");
    Serial.print(dtOff);
    Serial.print("-");
    Serial.println(i / 10);

    //setting the led HIGH for 100 milli seconds
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
