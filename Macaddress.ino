
#include"WiFi.h"

void setup() {
  Serial.begin(115200); // Setup Serial Monitor
  WiFi.mode(WIFI_MODE_STA);

  Serial.print("MAC Address: "); // Print MAC Address to Serial monitor
  Serial.println(WiFi.macAddress());
}
void loop() {
  
}
