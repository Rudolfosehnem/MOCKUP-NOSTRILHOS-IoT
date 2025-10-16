#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String BrokerURL = "test.mosquitto.org";
const int BrokerPort = 1883;
const String BrokerUser = "";
const String BrokerPass = "";

const String SSID = "FIESC_IoT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta essa msg na tela
  WiFi.begin("SSID,PASS");  //tenta conectar na rede 
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

  Serial.println("\nConectado ao Broker!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
