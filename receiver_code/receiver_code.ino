#include <esp_now.h> 
#include <WiFi.h> 
 
// Define the data structure for the message 
typedef struct message_structure { 
  float temperature; 
  float humidity; 
  int sensorValue; 
  bool controlFlag; 
} message_structure; 
 
// Create a structured variable to store received message 
message_structure receivedData; 
 
// Callback function executed when data is received 
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) { 
  if (len == sizeof(message_structure)) { 
    memcpy(&receivedData, incomingData, sizeof(receivedData)); 
     
    // Print the received data 
    Serial.println("Data received!"); 
    Serial.print("Temperature: "); 
    Serial.print(receivedData.temperature); 
    Serial.println(" °C"); 
     
    Serial.print("Humidity: "); 
    Serial.print(receivedData.humidity); 
    Serial.println(" %"); 
     
    Serial.print("Sensor value: "); 
    Serial.println(receivedData.sensorValue); 
     
    Serial.print("Control flag: "); 
    Serial.println(receivedData.controlFlag ? "ON" : "OFF"); 
    Serial.println(); 
  } 
} 
  
void setup() { 
  // Initialize Serial Monitor 
  Serial.begin(115200); 
   
  // Set device as a Wi-Fi Station 
  WiFi.mode(WIFI_STA); 
   
  // Initialize ESP-NOW 
  if (esp_now_init() != ESP_OK) { 
    Serial.println("Error initializing ESP-NOW"); 
    return; 
  } 
   
  // Register callback function 
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv)); 
   
  Serial.println("ESP-NOW Receiver initialized successfully"); 
} 
  
void loop() { 
 
}