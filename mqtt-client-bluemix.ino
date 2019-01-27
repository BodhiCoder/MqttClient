// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>

#define HOST "pc2xp0.messaging.internetofthings.ibmcloud.com"
#define CLIENT_ID "d:pc2xp0:Sensor:AccelSensor01"
#define AUTHMETHOD "use-token-auth"
#define AUTHTOKEN "xxx"

#define PUBLISH_TOPIC "iot-2/evt/status/fmt/json"
#define SUBSCRIBE_TOPIC "iot-2/cmd/+/fmt/json"

char ssid[] = "Bogdan's iPhone"; //RomTelecom02-WEP
char pass[] = "12345678";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;
String _state;

//char js[100];

void connect() {
  Serial.print("\nChecking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected to wifi!");
  //Serial.print("\nIP address: ", WiFi.localIP());
  while (!client.connect(CLIENT_ID, AUTHMETHOD, AUTHTOKEN)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected to Bluemix broker!");
  _state="on";
  client.subscribe(SUBSCRIBE_TOPIC);
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  
  //parse paylaod to see if valid json
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  
  //Serial.println(" json ok");
  int cmd = root["cmd"];
  if (cmd == 0) {
    _state="off";}
  Serial.println("Incoming command " + _state);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin(HOST, 1883, net);
  client.onMessage(messageReceived);
  connect();
}

int read_accel(){
  // dummy accel
  int max=21; 
  int min=1;
  int randNum = rand()%(max-min + 1) + min; 
  return randNum;     
}

int read_accel_from_ADL() {
  //  read accel from ADLX345
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  //do {
      if (millis() - lastMillis > 1000) {
          lastMillis = millis();
        
          //char js[] = "{\"accel\":".append()  or string js = "111".append(222).append(333);
          //daca vrei sa trimiti un string, trebuie sa il incadrezi intre ghilimele: 
          //sprintf(buffer, "{\"temp\":\"%s\"}", msg);
          //sprintf(js, "{\"temp\":%s}", 2);
        
          StaticJsonBuffer<100> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();
          if (_state == "on") {
              root["accel"] = read_accel();}
          else {
              root["accel"] = "-";}
          
          root["status"] = _state;
          //root["status"] = "on";

          String js;
          root.printTo(js);
          Serial.print("\nJSON string: ");
          Serial.println(js);
       
        // JsonArray& data = root.createNestedArray("data");
        //data.add(48.756080); //data.add(2.302038);
        
        client.publish(PUBLISH_TOPIC, js);
    }
  //} while (_state == "on");
}
