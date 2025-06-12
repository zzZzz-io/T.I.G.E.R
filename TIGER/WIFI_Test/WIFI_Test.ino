#include <WiFi.h>
#include <Wire.h>

const char* ssid = "TIGER_Flight_Computer";
const char* password = "stevenjcenci";

WiFiServer server(23);
WiFiClient client;

void setup() {
  WiFi.softAP(ssid, password);
  Wire.begin(8, 9); // Use working pins

  server.begin();
}

void loop() {
  if (server.hasClient()) {
    client = server.available();
    client.println("Scanning I2C...");

    for (byte address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
      if (error == 0) {
        client.printf("Found device at 0x%02X\n", address);
      }
    }
    client.println("Done.");
    delay(5000);
  }
}
