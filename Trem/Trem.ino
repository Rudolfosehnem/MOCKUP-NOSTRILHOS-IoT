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
  mqtt.subscribe("Topico-DSM14");
  mqtt.setCallback(callbacck);
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
mqtt.publish("Iluminacao" , "Acender"());
mqtt.loop();
delay(1000)
}

void callback(chat* topic, byte* payload, usigned int length){
  string msg = "";
  for(int i = 0; i < lenght; i++){
    msg += (char) payload[i];
  }
 if(topic == "Iluminacao" && msg == "Acender"){
  digitalWrite(2,HIGH);
}else if(topic == "Iluminacao" &&¨msg == "Apagar"){
  digitalWrite(2,LOW);
  }
}
