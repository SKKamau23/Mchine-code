//Rx Tx Initialization
#define RXp2 16
#define TXp2 17
//Start of IoT initialization
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*Replace with your SSID and Password
const char* ssid     = "V2027";
const char* password = "risper123";*/

//Replace with your SSID and Password
const char* ssid     = "AndroidAP2C2D";
const char* password = "samuel2022";


// Replace with your unique IFTTT URL resource
const char* resource = "https://maker.ifttt.com/trigger/Risper/with/key/d2oRXg6g7hN8oVA20DsB6S";

// How your resource variable should look like, but with your own API KEY (that API KEY below is just an example):
//const char* resource = "/trigger/bme280_readings/with/key/nAZjOphL3d-ZO4N3k64-1A7gTlNSrxMJdmqy3";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

String Log = "13/12/2021 16.36";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 30 minutes = 1800 seconds
uint64_t TIME_TO_SLEEP = 900;
//End of IoT initialization

//String buffer
String incoming1, incoming2;

void setup() {
  Serial.begin(115200); 
  delay(2000);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  initWifi();
  makeIFTTTRequest();
    
  #ifdef ESP32
    // enable timer deep sleep
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
    Serial.println("Going to sleep now");
    // start deep sleep for 3600 seconds (60 minutes)
    esp_deep_sleep_start();
  #else
    // Deep sleep mode for 3600 seconds (60 minutes)
    Serial.println("Going to sleep now");
    ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); 
  #endif
}

void loop() {
  // sleeping so wont get here 
  incoming1 = Serial2.readString(); 
  incoming2 = '"' + incoming1 + '"';
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  /*Serial.println("Message Received: ");
  Serial.println(Serial2.readString());
  incomingString1 = Serial2.readString();*/
  
  //String jsonObject = String("{\"value1\":\"") + 2 + "\",\"value2\":\"" + "" + "\",\"value3\":\"" + "" + "\"}"; 
  String jsonObject = String("{\"value1\":\"") + incoming2 + "\"}";               
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}
