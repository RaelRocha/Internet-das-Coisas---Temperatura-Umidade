#include <DHTesp.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "PubSubClient.h"

// Conexão URL
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const String url = "https://api.thingspeak.com/update?api_key=F902XZ6K92U6WS51&";

// Configuração MQTT Broker
int port = 1883;
const char* server = "broker.hivemq.com";
const char* topico = "Senai_Internet_das_coisas_IDR";

// HTTP
DHTesp dhtSensor;
HTTPClient http;

//MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);


void setup() {
 // Inicio do setup
  Serial.begin(115200);
  Serial.println("Iniciando Setup");
  WiFi.begin(ssid, password);
 // Conexão Wi-Fi
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Conectando ao WiFi");
  }
  dhtSensor.setup(13,  DHTesp::DHT22);
  pinMode(14, OUTPUT);
  pinMode(23, OUTPUT);
  Serial.println("Finalizando Setup");

 //Conexão Broker
 Serial.println("Conectando ao Broker");
 mqttClient.setServer(server, port);
 char* clientId = "esp343#edssfsd_pok";

 mqttClient.connect(clientId);
 mqttClient.subscribe(topico);

 while(!mqttClient.connected()){
   delay(500);
   Serial.println("*");
 }
 Serial.println("Broker conectado");
}

void loop() {
    //capturando temperatura e umidade

  float temperatura = dhtSensor.getTemperature();
  float umidade = dhtSensor.getHumidity();
  String path = url + "field1=" + String(temperatura) + "&field2=" + String(umidade);

  http.begin(path);
  int httpCode = http.GET();
  String payload = http.getString();

    //imprimindo e enviando dados capturados
  Serial.println("Temperatura:" +String(temperatura) + "C");
  Serial.println("Umidade:" +String(umidade) + "%");
  Serial.println("***************************************************");
  Serial.println("HttpCode: " + String(httpCode));
  Serial.println("Payload: " + payload);
  Serial.println("***************************************************");

  // Msg para MQTT
  int msg = mqttClient.publish(topico, "{\"temperatura\": 25, \"umidade\": 70}");
  if(!msg){
    Serial.println("Problemas ao enviar a mensagem.");
    mqttClient.connect("esp343#edssfsd_pok");
    mqttClient.subscribe(topico);
  } else{
    Serial.println("Mensagem enviada.");
  }

  delay(3000);

  digitalWrite(14, HIGH);
  digitalWrite(23, HIGH);
  delay(3000);

  digitalWrite(14, LOW);
  digitalWrite(23, LOW);
  delay(3000);
}
