#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String BrokerURL = "test.mosquitto.org";
const int BrokerPort = 1883;
const String BrokerUser = "";
const String BrokerPass = "";

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta essa msg na tela
  WiFi.begin(SSID,PASS);  //tenta conectar na rede 
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); //Mostra "......."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!");

  mqtt.setServer(BrokerURL.c_str(),BrokerPort);
  String BoardID = "Trem";
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str() , BrokerUser.c_str() , BrokerPass.c_str());

  while(!mqtt.connected()){
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe("Iluminacao"); //recebe a mensagem
  mqtt.setCallback(callback)
  Serial.println("\nConectado ao Broker!");
}

void loop() {
  // put your main code here, to run repeatedly:
  String mensagem = "Rudolfo:";
  mensagem += "oi";
  mqtt.publish("Iluminacao" , "Acender"); //envia mensagens 

  mqtt.loop();
  delay(1000);

}

void callback(char* topic, byte* payload, unsigned int length){     //processa a mensagem recebida
  String msg = "";
  for(int i = 0;< length; i++){
    msg += (char) payload[i];
  }
  if(topic == "Iluminacao"&& msg == "Acender"){
    digitalWrite(2,HIGH);
  }else if(topic == "Iluminacao"&& msg == "Apagar"){
    digitalWrite(2,LOW);
 }
}







