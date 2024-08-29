#include<DHT.h>
#include<WiFi.h>
#include<PubSubClient.h>

const char* ssid = "GuhpamWiFi";
const char* password = "";
const char* mqtt_server = "192.168.1.100";
const int mqtt_port = 1883;

DHT dht(15, DHT22);

WiFiClient espClient;
PubSubClient client(espClient);

const char* temp_topic = "daq1/suhu";
const char* hum_topic = "daq1/kelembapan";

void reconnect(){
  while(!client.connected()){
    Serial.print("Mencoba reconnect...");
    if(client.connect("ESP32Client")){
      Serial.println("terhubung");
    }
    else{
      Serial.print("Gagal");
      Serial.print(client.state());
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    reconnect();
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String tempStr = String(temperature);
  client.publish(temp_topic, tempStr.c_str());
  
  String humStr = String(humidity);
  client.publish(hum_topic, humStr.c_str());
  delay(1000);
}
