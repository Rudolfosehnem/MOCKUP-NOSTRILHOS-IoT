#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient client;
PubSubClient mqtt(client);



void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta essa msg na tela
  WiFi.begin(WIFI_SSID,WIFI_PASS);  //tenta conectar na rede 
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); //Mostra "......."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!");

  mqtt.setServer(BROKER_URL,BROKER_PORT);
  String BoardID = "Trem";
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str() , BROKER_USER  , BROKER_PASS);

  while(!mqtt.connected()){
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe("TOPIC_ILUM"); //recebe a mensagem
  mqtt.setCallback(callback)
  Serial.println("\nConectado ao Broker!");
}

void loop() {
  // put your main code here, to run repeatedly:
  String mensagem = "Rudolfo:";
  mensagem += "oi";
  mqtt.publish("TOPIC_ILUM" , "Acender"); //envia mensagens 

  mqtt.loop();
  delay(1000);

}

void callback(char* topic, byte* payload, unsigned int length){     //processa a mensagem recebida
  String msg = "";
  for(int i = 0;< length; i++){
    msg += (char) payload[i];
  }
  if(topic == "TOPIC_ILUM"&& msg == "Acender"){
    digitalWrite(2,HIGH);
  }else if(topic == "TOPIC_ILUM"&& msg == "Apagar"){
    digitalWrite(2,LOW);
 }
}