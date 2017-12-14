#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

// change to 0 if you want to connect to WiFi
#define WIFI_AP_MODE 1

// put wifi credentials here
#define WIFI_SSID "dht-sensor"
//#define WIFI_PASS "your password"

// reading interval in milliseconds
// anything below 2s will not work - slow sensor
#define READING_INTERVAL 10000UL

// pin assignments - do not change
#define PIN_DHT D7
#define PIN_LED D2


// variables
ESP8266WebServer server(80);
DHT dht(PIN_DHT, DHT22);
float temperature;
float humidity;
uint32_t lastReading;


void setup() {
  // setup serial debug
  Serial.begin(9600);
  Serial.println("DHT sensor!");

  // setup indicator LED
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);


  // setup DHT sensor readouts
  dht.begin();
  temperature = 0;
  humidity = 0;
  lastReading = 0;


#if WIFI_AP_MODE
  // start WiFi AP
  Serial.println("Starting WiFi AP");
  WiFi.mode(WIFI_AP);
#if WIFI_PASS
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
#else
  WiFi.softAP(WIFI_SSID);
#endif

#else
  // connect to AP
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int counter = 0;
  // blink LED fast when connecting
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    counter++;
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);

    if (counter > 75) {
      Serial.println("Connect timed out");
      break;
    }
  }
  Serial.println();

  // print IP after connected
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect!");
  }
#endif

  // server endpoints
  server.on("/", HTTP_GET, handleHtml);
  server.on("/temp.json", HTTP_GET, handleJson);
  server.begin();
}

void handleHtml() {
  String result = "<html><head><title>Temp and humidity sensor</title></head><body><h2>Temperature</h2><p>";
  result += temperature;
  result += "&nbsp;C</p><h2>Humidity</h2><p>";
  result += humidity;
  result += " %</p></body></html>";
  server.send(200, "text/html", result);
}

void handleJson() {
  String result = "{\"temp\":";
  result += temperature;
  result += ",\"hum\":";
  result += humidity;
  result += ",\"read\":";
  result += millis() - lastReading;
  result += ",\"uptime\":";
  result += millis();
  result += "}\n";
  server.send(200, "text/json", result);
}

void loop() {
  // is it time for next sensor reading?
  if (millis() - lastReading > READING_INTERVAL) {
    // turn on LED
    digitalWrite(PIN_LED, HIGH);

    // read from DHT sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if read is successful
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      // store read values
      temperature = t;
      humidity = h;
    }

    // turn off LED
    digitalWrite(PIN_LED, LOW);

    // mark last reading time
    lastReading = millis();
  }

  server.handleClient();
}
