#include <DHT.h>

// DHT11 Sensor
#define DHTPIN 2      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11 sensor type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the serial communication to monitor output
  Serial.begin(9600);
  
  // Initialize the DHT11 sensor
  dht.begin();
}

void loop() {
  // Read humidity and temperature values from the DHT11 sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Default is Celsius
  
  // Check if any reading failed and exit early
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature and humidity to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  |  Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Wait a few seconds between measurements
  delay(2000); // Delay 2 seconds between readings
}