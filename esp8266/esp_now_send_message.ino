#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED 16

// Nó destinatário:
uint8_t broadcastAddress[] = { 0xA0, 0x20, 0xA6, 0x17, 0x45, 0x32 };

// Exemplo de estrutura de dados:
typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;

unsigned long lastMillis = 0;

message myMessage;

// Callback do envio de mensagem:
void onSent(uint8_t *macAddress, uint8_t deliveryStatus) {
  digitalWrite(LED, LOW);

  Serial.print("Mensagem enviada com status: ");

  if (deliveryStatus == 0) {
    Serial.println("sucesso");
  } else {
    Serial.println("falha");
  };

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

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onSent);

  // Registro do destinatário:
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
};

void loop() {
  if (millis() - lastMillis > 3000) {
    strcpy(myMessage.a, "HELLO WORLD!");
    myMessage.b = random(1, 100);
    myMessage.c = random(1, 100) / 100.0;
    myMessage.d = "Hello!";
    myMessage.e = (bool)random(0, 2);

    esp_now_send(broadcastAddress, (uint8_t *)&myMessage, sizeof(myMessage));

    lastMillis = millis();
  };
};
