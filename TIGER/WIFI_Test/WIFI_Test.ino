#include <WiFi.h>

const char* ssid = "T.I.G.E.R. Flight Computer";
const char* password = "stevenjcenci";

WiFiServer server(23);  // Telnet uses port 23, but you can use any port

WiFiClient client;

void setup() {
  Serial.begin(115200); // Local serial debug
  WiFi.softAP(ssid, password);
  Serial.println("AP started. IP: " + WiFi.softAPIP().toString());
  server.begin();
}

void loop() {
  if (server.hasClient()) {
    if (!client || !client.connected()) {
      client = server.available();
      Serial.println("Client connected");
    } else {
      server.available().stop(); // Only allow one client
    }
  }

  if (client && client.connected()) {
    if (client.available()) {
      String incoming = client.readStringUntil('\n');
      Serial.println("From client: " + incoming);
      client.println("Echo: " + incoming); // Optional echo back
    }
  }
}
