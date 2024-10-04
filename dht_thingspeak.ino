#include <DHT.h>
#include <WiFi.h>  // Use WiFi library for ESP32

// DHT11 Sensor
#define DHTPIN 4      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11 sensor type

DHT dht(DHTPIN, DHTTYPE);

// WiFi settings
const char* ssid = "Robomanthan4g";         // Your WiFi network name
const char* password = "12345678"; // Your WiFi password

// ThingSpeak settings
const char* server = "api.thingspeak.com";
String apiKey = "T79X60M0G9INH98K";  // Replace with your ThingSpeak API key
const char* channelID = "2664690";  // Replace with your ThingSpeak Channel ID

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Read temperature and humidity from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Check if any readings failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Send data to ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = "api_key=" + apiKey;  // Use the API key for authentication
    postStr += "&field1=";
    postStr += String(h);  // Temperature
    postStr += "&field2=";
    postStr += String(t);  // Humidity
    postStr += "\r\n\r\n";

    client.print("POST /update?api_key=" + apiKey + "&channel_id=" + String(channelID) + " HTTP/1.1\n");
    client.print("Host: " + String(server) + "\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees C, Humidity: ");
    Serial.print(h);
    Serial.println("%");

    delay(1000);  // Wait to receive server response
    while (client.available()) {
      String response = client.readString();
      Serial.println("Server response: ");
      Serial.println(response); // Print the server's response for debugging
    }
  }
  
  client.stop();
  
  // Wait 15 seconds before sending again (ThingSpeak has a limit of 15 seconds between updates)
  delay(5000);
}