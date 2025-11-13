#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "env.h"


WiFiClientSecure client;
PubSubClient mqtt(client);

const byte LDR_PIN = 34;
const byte LED_PIN = 19;

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN,INPUT);
  pinMode(LED_PIN,OUTPUT);

  client.setInsecure();
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
  mqtt.setCallback(callback);
  mqtt.subscribe(TOPIC_ILUM); //recebe a mensagem
  Serial.println("\nConectado ao Broker!");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt.publish(TOPIC_ILUM , lerLDR(LDR_PIN)); //envia mensagens 
  mqtt.publish(TOPIC_PRESEN , "Presente");

  mqtt.publish(TOPIC_UMID , "Umido");
  mqtt.publish(TOPIC_TEMP , "Temperatura");


  mqtt.loop();
  delay(1000);

}

void callback(char* topic, byte* payload, unsigned int length){     //processa a mensagem recebida
  String msg = "";
  for(int i = 0; i < length; i++){
    msg += (char) payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(msg);
  if(strcmp(topic, TOPIC_ILUM) == 0 && msg.equals("Ambiente escuro")){
    digitalWrite(LED_PIN,HIGH);
  }else if(strcmp(topic, TOPIC_ILUM) == 0 && msg.equals("Ambiente claro")){
    digitalWrite(LED_PIN,LOW);
 }
}

char* lerLDR(byte pino){
  int leituraLDR = analogRead(pino);
  float tensao = (leituraLDR * 3.3) / 4095.0;
  
  if (leituraLDR > 2000) {
    return "Ambiente escuro";
  } else {
    return "Ambiente claro";
  }
}