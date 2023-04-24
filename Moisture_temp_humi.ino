#include <dht.h>

dht DHT;

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
}
#comment for the comment gods
void loop(){
  double moisture;
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  Serial.print("Moisture Sensor Value:");
  moisture = analogRead(2);
  Serial.println(moisture);

  delay(2000);
}
