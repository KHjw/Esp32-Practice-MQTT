#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "KT_GiGA_6C64";
const char* password = "ed46zx1198";
const char* mqtt_server = "172.30.1.44";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";       // Create a random client ID
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {   // Attempt to connect
      Serial.println("connected");
      client.publish("mainOS", " Connected");
      client.subscribe("BD");
      client.subscribe("ALL");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    Serial.println("ED Publish: FUcK");
    Serial.println("ALL Publish: YOU");
    client.publish("ED", "FUcK");
    client.publish("ALL", "YOU");
  }
}