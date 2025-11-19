#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h"
#include "DHT.h"


WiFiClientSecure client;
PubSubClient mqtt(client);

const byte LDR_PIN = 34;
const byte LED_PIN = 19;
const byte Ultra_echo = 23;
const byte Ultra_trig = 22;

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(Ultra_trig, OUTPUT);
  pinMode(Ultra_echo, INPUT);

  dht.begin();

  //pinmode pro ultra echo e trigg

  client.setInsecure();
  Serial.println("Conectando ao WiFi");  //apresenta essa msg na tela
  WiFi.begin(WIFI_SSID, WIFI_PASS);      //tenta conectar na rede
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  //Mostra "......."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  String BoardID = "Trem";
  BoardID += String(random(0xffff), HEX);
  mqtt.connect(BoardID.c_str(), BROKER_USER, BROKER_PASS);

  while (!mqtt.connected()) {
    Serial.print(".");
    delay(200);
  }
  mqtt.setCallback(callback);
  mqtt.subscribe(TOPIC_ILUM);  //recebe a mensagem
  Serial.println("\nConectado ao Broker!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //le e publica o ldr
  mqtt.publish(TOPIC_ILUM, lerLDR(LDR_PIN));  //envia mensagens

  //le e publica o ultrassonico
  long distancia = lerDistancia(Ultra_trig, Ultra_echo);
  mqtt.publish(TOPIC_PRESEN, String(distancia).c_str());


  //le e publica a temperatura e umidade
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro na leitura do DHT11");
  }
  mqtt.publish(TOPIC_UMID, String(umidade).c_str());
  mqtt.publish(TOPIC_TEMP, String(temperatura).c_str());

  mqtt.loop();
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {  //processa a mensagem recebida
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(msg);
  if (strcmp(topic, TOPIC_ILUM) == 0 && msg.equals("Ambiente escuro")) {
    digitalWrite(LED_PIN, HIGH);
  } else if (strcmp(topic, TOPIC_ILUM) == 0 && msg.equals("Ambiente claro")) {
    digitalWrite(LED_PIN, LOW);
  }
}

char* lerLDR(byte pino) {
  int leituraLDR = analogRead(pino);
  float tensao = (leituraLDR * 3.3) / 4095.0;

  if (leituraLDR > 2000) {
    return "Ambiente escuro";
  } else {
    return "Ambiente claro";
  }
}

int lerDistancia(byte TRIGGER_PIN, byte ECHO_PIN) {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  Serial.println(distancia);
  return distancia;

  delay(2000);
}
