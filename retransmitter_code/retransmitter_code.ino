#include <WiFi.h> 
#include <esp_now.h> 
 
typedef struct struct_message { 
  float temperatura; 
  float humidade; 
  bool flag; 
} struct_message; 
 
struct_message dadosRecebidos; 
 
uint8_t enderecoDestino[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};
do receptor final 
 
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) { 
  memcpy(&dadosRecebidos, incomingData, sizeof(dadosRecebidos)); 
  Serial.print("Dados recebidos - Temp: "); 
  Serial.print(dadosRecebidos.temperatura); 
  Serial.print(" °C, Hum: "); 
  Serial.print(dadosRecebidos.humidade); 
  Serial.print(" %, Flag: "); 
  Serial.println(dadosRecebidos.flag ? "true" : "false"); 
 
  esp_err_t result = esp_now_send(enderecoDestino, (uint8_t *)&dadosRecebidos, 
sizeof(dadosRecebidos)); 
 
  if (result == ESP_OK) { 
    Serial.println("Dados retransmitidos com sucesso!"); 
  } else { 
    Serial.print("Erro ao retransmitir: "); 
    Serial.println(result); 
  } 
} 
 
void setup() { 
  Serial.begin(115200); 
 
  WiFi.mode(WIFI_STA); 
  Serial.print("MAC do retransmissor: "); 
  Serial.println(WiFi.macAddress()); 
 
  if (esp_now_init() != ESP_OK) { 
    Serial.println("Erro ao inicializar ESP-NOW"); 
    return; 
  } 
 
  esp_now_register_recv_cb(OnDataRecv); 
 
  esp_now_peer_info_t peerInfo = {}; 
  memcpy(peerInfo.peer_addr, enderecoDestino, 6); 
  peerInfo.channel = 0;   
  peerInfo.encrypt = false; 
 
  if (!esp_now_is_peer_exist(enderecoDestino)) { 
    if (esp_now_add_peer(&peerInfo) != ESP_OK) { 
      Serial.println("Erro ao adicionar peer"); 
      return; 
    } 
  } 
 
  Serial.println("Repetidor iniciado."); 
} 
 
void loop() { 
} 
