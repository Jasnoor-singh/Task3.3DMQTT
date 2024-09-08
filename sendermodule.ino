#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Jasnoor";
const char* password = "12345678";

// MQTT broker details
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttTopicWave = "SIT210/wavesJasnoor";
const char* mqttTopicPat = "SIT210/patsJasnoor";

WiFiClient espClient;
PubSubClient client(espClient);

// Pin definitions
const int trigPin = 2;
const int echoPin = 3;

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to WiFi
  connectToWiFi();

  // Setup MQTT
  client.setServer(mqttServer, mqttPort);

  // Connect to MQTT
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Simulate pat detection for demonstration purposes
  if (detectPat()) {
    Serial.println("Pat detected. Publishing 'pat'...");
    client.publish(mqttTopicPat, "Jasnoor Pat");
    delay(5000);
  }
  else{
    Serial.println("Wave detected. Publishing 'wave'...");
    client.publish(mqttTopicWave, "Jasnoor Wave");
    delay(5000);
  }
}

void connectToWiFi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("SenderClient")) {
      Serial.println("Connected to MQTT");
    } else {
      delay(5000);
    }
  }
}



bool detectPat() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Receive echo signal
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  if (distance > 20) {
    return true;
  }
  if (distance > 0 && distance < 20) {
    return false;
  }
}
