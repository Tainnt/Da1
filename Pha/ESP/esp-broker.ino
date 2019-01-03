#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// CONSTANT VARIABLES
//***WIFI***
const char* ssid = "hahaha";
const char* password =  "987abc123";

//***SERVER***
const char* mqttServer = "m21.cloudmqtt.com";
const int mqttPort = 14531;
const char* mqttUser = "girlnujd";
const char* mqttPassword = "uzlMkPzoIxcs";

//***CODE***
char raw_electric_received[5];
char raw_water_received[5];
uint8_t i = 0;
uint8_t flag = 0;

//***Naming***
#define WATER_LENGTH    5
#define ELECTRIC_LENGTH 5

// Initialize a client
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  //init serial port
  Serial.begin(115200);
 
  //connect to wifi hostpot
  WiFi.begin(ssid, password);
  
  //verify wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  //configure the client instance to be ready for use
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  //verify client's connection to server
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) // arguments: clientID, username, password
    {
      Serial.println("connected");  
 
    }
    else
    {
      Serial.print("failed with state: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  client.publish("esp/test", "Data tu ESP");
  client.subscribe("esp/test");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();

  if((Serial.readBytesUntil('!', raw_electric_received, ELECTRIC_LENGTH) > 0)
  && (Serial.readBytesUntil('!', raw_water_received, WATER_LENGTH) > 0))
    flag = 1;

  if(flag == 1)
  {
    Serial.print("E;W = ");
    Serial.print(raw_electric_received);
    Serial.print(";");
    Serial.println(raw_water_received);

    
    client.publish("Dien/Thang12", raw_electric_received);
    client.publish("Nuoc/Thang12", raw_water_received);
  }
  flag = 0;
}
