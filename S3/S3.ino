#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h"
#include <ESP32Servo.h>

WiFiClientSecure client;
PubSubClient mqtt(client);



//ultrassônico
const byte SERVO_PIN = 18;
Servo Servo1;

const byte SERVO_PIN2 = 19;
Servo Servo2;

const byte TRIGGER_PIN = 25;
const byte ECHO_PIN = 26;



void setup() {
  Serial.begin(115200);

//config servo
  Servo1.attach(SERVO_PIN);
  Servo2.attach(SERVO_PIN2);

  //config ultrassonico
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

//wificonex
  client.setInsecure();
  Serial.println("Conectando ao WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.print("\nConectado com Sucesso!");
  Serial.println("Conectando ao Broker...");

  //mqttconfig
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  mqtt.setCallback(callback);

  //idaleatorioESP32
  String BoardID = "S2";
  BoardID += String(random(0xffff, HEX));
  mqtt.connect(BoardID.c_str(), BROKER_USER, BROKER_PASS);
  while (!mqtt.connected()) {
    Serial.println(".");
    delay(200);
  }
  Serial.println("\nConectado ao Broker!");

  //mqtttopicos
  mqtt.subscribe(TOPIC_ILUM);
  mqtt.subscribe(TOPIC_PRESENCA1);
  mqtt.subscribe(TOPIC_PRESENCA2);
  mqtt.subscribe(TOPIC_PRESENCA3);
}

//funcaolerdistancia
long lerDistancia() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  //comecamediçao
  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  return distancia;
}

//loopprincipal
void loop() {

  mqtt.loop();

  long distancia = lerDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  //mostraseobjetoperto
  if (distancia < 10) {
    Serial.println("Objeto próximo!");
    mqtt.publish(TOPIC_DISTANCIA, "Objeto próximo!");
  }

  delay(1000);

}

//montamsg
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

Serial.print("TOPICO: "); Serial.println(topic);
  Serial.print("MSG: "); Serial.println(msg);

  //controleluz
  if (strcmp(topic, TOPIC_ILUM) == 0) {
    if (msg == "ligar") digitalWrite(2, HIGH);
    if (msg == "desligar") digitalWrite(2, LOW);

  //servo1mov
  }else if (strcmp(topic, TOPIC_PRESENCA1) == 0) {
    if (msg == "servo1") {
      Servo1.write(0);
      Serial.println("Servo1 → posição 1 (0°)");
    }

    //servo2mov
  }else if (strcmp(topic, TOPIC_PRESENCA2) == 0) {
    if (msg == "servo2") {
      Servo2.write(0);
      Serial.println("Servo2 → posição 1 (0°)");
    }

    //abrirservos
  }else if (strcmp(topic, TOPIC_PRESENCA3) == 0) {
    if (msg == "abrir") {
      Servo1.write(90);
      Servo2.write(90);
      Serial.println("Servo1 e Servo2 → posição 2 (90°)");
    }
  }
}
