#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Current_SSID";
const char* password = "Current_Password";
const char* serverName = "http://Current_Server_IP:5000/data";
WiFiClient client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connecting to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected!");
}

void loop() {
  // Print sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");

    // Post
    String postData = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
    int httpResponse = http.POST(postData);

    if (httpResponse > 0) {
      // Response
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error ");
      Serial.println(httpResponse);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(1000);
}
