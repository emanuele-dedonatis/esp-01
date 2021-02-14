#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define wifi_ssid "ssid"
#define wifi_password "password"

#define mqtt_server "mqttip"
#define mqtt_user "user"
#define mqtt_password "password"

#define humidity_topic "sensor/esp1/humidity"
#define temperature_topic "sensor/esp1/temperature"

#define DHTTYPE DHT22
#define DHTPIN  2

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

void setup() {
  dht.begin();
  wifi_connect();
  client.setServer(mqtt_server, 1883);
}

void wifi_connect() {
  delay(10);
  WiFi.disconnect();
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void mqtt_connect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    if (!client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_connect();
  }
  if (!client.connected()) {
    mqtt_connect();
  }
  client.loop();

  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  client.publish(temperature_topic, String(temp).c_str(), true);
  client.publish(humidity_topic, String(hum).c_str(), true);
  
  delay(900000);
}
