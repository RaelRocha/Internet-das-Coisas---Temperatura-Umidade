#include <DHTesp.h>
#include "WiFi.h"
#include "HTTPClient.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const String url = "https://api.thingspeak.com/update?api_key=F902XZ6K92U6WS51&";

DHTesp dhtSensor;
HTTPClient http;


void setup() {
 
  Serial.begin(115200);
  Serial.println("Iniciando Setup");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Conecting to WiFi");
  }
  dhtSensor.setup(13,  DHTesp::DHT22);
  pinMode(14, OUTPUT);
  pinMode(23, OUTPUT);
  Serial.println("Finalizando Setup");
}

void loop() {
    //capturando temperatura e umidade

  float temperatura = dhtSensor.getTemperature();
  float umidade = dhtSensor.getHumidity();
  String path = url + "field1=" + String(temperatura) + "&field2=" + String(umidade);

  http.begin(path);
  int httpCode = http.GET();
  String payload = http.getString();

    //imprimindo dados capturados
  Serial.println("Temperatura:" +String(temperatura) + "C");
  Serial.println("Umidade:" +String(umidade) + "%");
  Serial.println("***************************************************");
  Serial.println("HttpCode: " + String(httpCode));
  Serial.println("Payload: " + payload);
  Serial.println("***************************************************");

  delay(3000);

  digitalWrite(14, HIGH);
  digitalWrite(23, HIGH);
  delay(3000);

  digitalWrite(14, LOW);
  digitalWrite(23, LOW);
  delay(3000);
}