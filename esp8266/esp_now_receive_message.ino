#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED 16

// Exemplo de estrutura de dados:
typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;

message myMessage;

// Callback do recebimento de mensagem:
void onReceive(uint8_t *macAddress, uint8_t *data, uint8_t length) {
  digitalWrite(LED, LOW);

  memcpy(&myMessage, data, sizeof(myMessage));

  Serial.print("Mensagem recebida: ");
  Serial.println(length);
  Serial.print("a: ");
  Serial.println(myMessage.a);
  Serial.print("b: ");
  Serial.println(myMessage.b);
  Serial.print("c: ");
  Serial.println(myMessage.c);
  Serial.print("d: ");
  Serial.println(myMessage.d);
  Serial.print("e: ");
  Serial.println(myMessage.e);
  Serial.println();

  delay(10);

  digitalWrite(LED, HIGH);
};

void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  // Configurando o dispositivo como estação Wi-Fi:
  WiFi.mode(WIFI_STA);

  Serial.print("Meu endereço MAC: ");
  Serial.println(WiFi.macAddress());

  // Inicialização do ESP-Now:
  if (esp_now_init() != 0) {
    Serial.println("Falha ao inicializar o ESP-Now");

    return;
  };

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onReceive);
};

void loop(){};
