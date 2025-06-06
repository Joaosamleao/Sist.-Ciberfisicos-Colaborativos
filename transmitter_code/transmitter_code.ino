#include <esp_now.h> 
#include <WiFi.h> 
#include <DHT.h> 
 
#define DHT_PIN 5 
#define DHT_TYPE DHT11 
 
DHT dht(DHT_PIN, DHT_TYPE); 
 
// REPLACE WITH THE MAC Address of your receiver 
uint8_t receiverMacAddress[] = {0xEC, 0x64, 0xC9, 0x86, 0x21, 0x0C}; 
 
// Define a data structure for the message 
typedef struct message_structure { 
  float temperature; 
  float humidity; 
  int sensorValue; 
  bool controlFlag; 
} message_structure; 
 
// Create a structured message 
message_structure sensorData; 
 
// Callback function called when data is sent 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
  Serial.print("Send Status: "); 
  if (status == ESP_NOW_SEND_SUCCESS) { 
    Serial.println("Success"); 
  } else { 
    Serial.println("Failed"); 
  } 
} 
  
void setup() { 
  dht.begin(); 
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
  esp_now_register_send_cb(OnDataSent); 
   
  // Register peer 
  esp_now_peer_info_t peerInfo = {}; 
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6); 
  peerInfo.channel = 0;   
  peerInfo.encrypt = false; 
   
  // Add peer         
  if (esp_now_add_peer(&peerInfo) != ESP_OK) { 
    Serial.println("Failed to add peer"); 
    return; 
  } 
   
  Serial.println("ESP-NOW Transmitter initialized successfully"); 
} 
  
void loop() { 
  // Simulate sensor readings (replace with actual sensor code if needed) 
  sensorData.temperature = dht.readTemperature();         // 20-30°C 
  sensorData.humidity = dht.readHumidity();            // 40-90% 
  sensorData.sensorValue = analogRead(A0);         // Read analog pin A0 
  sensorData.controlFlag = (millis() % 20000 > 10000);  // Toggle every 10 seconds 
   
  // Print values to serial monitor 
  Serial.print("Sending data - Temperature: "); 
  Serial.print(sensorData.temperature); 
  Serial.print("°C, Humidity: "); 
  Serial.print(sensorData.humidity); 
  Serial.print("%, Sensor: "); 
  Serial.print(sensorData.sensorValue); 
  Serial.print(", Flag: "); 
  Serial.println(sensorData.controlFlag); 
   
  // Send message via ESP-NOW 
  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &sensorData, 
sizeof(sensorData)); 
   
  if (result != ESP_OK) { 
    Serial.println("Error sending the data"); 
  } 
   
  // Wait before next transmission 
  delay(3000); 
}