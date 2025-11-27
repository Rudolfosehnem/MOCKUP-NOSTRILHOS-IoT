#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

//criaçaomqttewifi
WiFiClient client;
PubSubClient mqtt(client);

//roda tudo quando liga o ESP32
void setup() {
  Serial.begin(115200);
  Serial.println("Conectando no WiFi");

  //wificonex
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("\nConectado com Sucesso no WiFi!");

  //brokerconfig
  Serial.println("Conectando ao Broker...");
  mqtt.setServer(BROKER_URL,BROKER_PORT);

  //idaleatorio
  String BoardID = "Trem";
  BoardID += String(random(0xffff),HEX);

  //conexaobroker
  mqtt.connect(BoardID.c_str() , BROKER_USER , BROKER_PASS);
  while(!mqtt.connected()){
    Serial.print(".");
    delay(200);
  }  
  mqtt.subscribe(TOPIC_ILUM);

  //funçaocallback
  mqtt.setCallback(callback); // Recebe a mensagem
  Serial.println("\nConectado ao Broker!");
}


void loop() {
  mqtt.publish(TOPIC_ILUM , "Acender"); //mqttpublicaçao
  mqtt.loop();
  delay(1000);
}

//montamsg
void callback(char* topic, type* payload, unsigned int length){
  String msg = "";
  for(int i = 0; i < length; i++){
    msg += (char) payload(1);
  }

  //acende e apaga led
  if(topic=TOPIC_ILUM && msg = "Acender"){
    digitalWrite(2,HIGH);
    
  } else if (topic=TOPIC_ILUM && msg = "Apagar"){
    digitalWrite(2,LOW);
  }
}
