#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String BrokerURL = "test.mosquitto.org";
const int BrokerPort = 1883;

const String SSID  = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200);
  Serial.println("Conecando ao Wifi"); //Apresenta essa msg na tela
  WiFi.begin(SSID,PASS); // Tenta conectar na rede
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); //mostra "......."
    delay(200);
  }
  Serial.println("\nConectado Com Sucesso!");

  Serial.println("Conectado ao Broker...")
  mqtt.setServer(BrokerURL.c_str(),BrokerPort);
  String BoardID = "NosTrem"
  BoardID += String(random(0xffff),HEX);
  mqtt.connect(BoardID.c_str(),BrokerURL.c_str(),BrokerPort.c_str());
  while(!mqtt.connected()){
  Serial.print(".");
  delay(200);
  }
  Serial.println("\nConectado ao Broker!");
}

void loop() {
  //void aa bb put your main code here, to run repeatedly:
String mensagem = "Nome: ";
mensagem += "oi";
mqtt.publish("topico-DSM14" , mensagem.c_str());
mqtt.loop();
delay(1000)
}
