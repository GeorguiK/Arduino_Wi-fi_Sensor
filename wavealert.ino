/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "mysssid"
#define STAPSK  "mypass"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.100"; //server IP address
const uint16_t port = 8080;
const char* sensorName="sensor1";  //should be unique
const uint16_t request_interval = 500;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  long rssi = WiFi.RSSI();

  Serial.print("rssi : ");
  Serial.println(rssi);

  String requestPrefix = "GET /addData/";
  String request = requestPrefix + sensorName + "/" + rssi + " HTTP/1.1";

  Serial.print("Sending request : ");
  Serial.println(request);
  
  // This will send the request to the server
  //client.println("GET /addData/sensort/55  HTTP/1.1");
  client.println(request);
  client.println("User-Agent: curl/7.16.3 libcurl/7.16.3");
  client.println("Host: www.example.com");
  client.println("Accept-Language: en");
  client.println("");
  client.println("");

  //read back one line from server
  Serial.println("Response : ");
  String line = client.readStringUntil('\r');
  line = client.readStringUntil('\r');
  Serial.println(line);

  //Serial.println("closing connection");
  client.stop();

  delay(request_interval);
}
