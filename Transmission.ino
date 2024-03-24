#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>

//Define DHT11 parameters
#define DHTPin 4
#define DHTType DHT11

DHT dht(DHTPin, DHTType); //Create DHT Object

float temp; // Variables for temperature and humidity
float humid;

uint8_t broadcastAddress[] = { 0x08, 0xD1, 0xF9, 0x27, 0xAE, 0xF4};  //Responder MAC Address

// Define data structure
typedef struct struct_message {
  float a;
  float b;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// Sent data callback function
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  Serial.begin(115200); // Setup Serial monitor
  delay(100);

  dht.begin(); // Initiate DHT11

  WiFi.mode(WIFI_STA);

// Initialize ESP-
  if (esp_now_init() != 0){
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

}

void loop(){

  // Read DHT11 module values
  temp = dht.readTemperature();
  delay(10);
  humid = dht.readHumidity();
  delay(10);

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humid: ");
  Serial.println(humid);

// Add to structured data object
  myData.a = temp;
  myData.b = humid;

// Send 
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000);

}
