#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3

const char* ssid = "AS_712B_2.4G";
const char* password = "speech712b";



OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);




const int httpPort = 80;
int interval = 8000;
const char* host = "api.thingspeak.com";
String url = "/update?api_key=K8ERISEY0FUL76S3";  

String working() { 
  sensors.requestTemperatures();
  return(String("field1=")+String(sensors.getTempCByIndex(0)));
}

void delivering(String payload) { 
  WiFiClient client;

  
  if (!client.connect(host, httpPort)) {
    return;
  }
  
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 temperature");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  
  while (client.connected()) {
    String line = client.readStringUntil('\n');

  }
}

void connect_ap() {
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  connect_ap();
  sensors.begin();
}

unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     String payload = working();
     delivering(payload);
  }
}
