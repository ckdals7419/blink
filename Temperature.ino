#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3
//#define THINGSPEAK
#define WEBHOOKS

const char* ssid = "AS_712B_2.4G";
const char* password = "speech712b";

#ifdef THINGSPEAK
const char* host = "api.thingspeak.com";
String url = "/update?api_key=I53JCA8ARP1VC90B";  
#endif

#ifdef WEBHOOKS
const char *host = "maker.ifttt.com";
String url = "/trigger/hot_temp/with/key/c18DFs0EYrEwWOm7FMrXhq";
#endif

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String working() { 
  sensors.requestTemperatures();
#ifdef THINGSPEAK
  String res = String("value1=") + String(sensors.getTempCByIndex(0));
#endif
#ifdef WEBHOOKS
  String res = String(sensors.getTempCByIndex(0));
#endif
  return res;
}

const int httpPort = 80;
int interval = 8000;

void delivering(String payload) { 
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    return;
  }
  
  #ifdef THINGSPEAK
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  #endif

  #ifdef WEBHOOKS
  
  String jsonStr = "{ \"value1\" : \"" + String(payload) + "\"" + "}";               
  String str = "POST " + String(url) + " HTTP/1.1\r\n" 
                    + "HOST: " + String(host) + "\r\n" 
                    + "Content-Type: application/json\r\n"
                    + "Content-Length: " + String(jsonStr.length()) + "\n\n";                 
  String getheader = str + jsonStr;                
  #endif
  
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
