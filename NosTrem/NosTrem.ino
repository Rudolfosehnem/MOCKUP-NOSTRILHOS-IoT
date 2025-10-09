#include <WiFi.h>

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
}

void loop() {
  //void aa bb put your main code here, to run repeatedly:

}
