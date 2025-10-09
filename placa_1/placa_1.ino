#include <WiFi.h>

const String SSID = "nome";
const String PASS = "senha";

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi"); //apresenta essa msg na tela
  WiFi.begin("SSID,PASS");  //tenta conectar na rede 
  while(WiFi.status() != WL_CONNECTED                                                                                               ){
    Serial.print("."); //Mostra "......."
    delay(200);
  }
  Serial.println("\nConectado com Sucesso!")

}

void loop() {
  // put your main code here, to run repeatedly:

}
