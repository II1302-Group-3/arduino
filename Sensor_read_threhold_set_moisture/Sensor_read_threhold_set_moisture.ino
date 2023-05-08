#include <dht.h>
// Include the Wire library for I2C
#include <Wire.h>
dht DHT;

#define DHT11_PIN 7
#define FALSE 0
#define TRUE 1

const int relayPin = 8;
unsigned char recieve_buffer[3]; // star bit , 2 bits for int
unsigned char sendData[6]; // 2 bytes for temp, 2 for humiidity, 2 for moisture
int index = 0;

double moisture = 0;
double moisture_threshold = 0;
char print_flag = 0;

char water_off_flag = FALSE;


void setup(){
  // Setup pin 13 as output and turn LED off
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  //start serial comm over USB
  Serial.begin(9600);

   // Join I2C bus as slave with address 8
  Wire.begin(0x8);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);

  // send data to arduino
  Wire.onRequest(sendEvent);
}
//callback for sending data
  void sendEvent(){
 
    Wire.write(sendData[index]);
    ++index;
    if(index >= sizeof(sendData)){
      index = 0;
    }
  }

// Function that executes whenever data is received from master

void receiveEvent(int howMany) {

  while (Wire.available()) { 
    int i = 0;
    while(Wire.available()){ // loop through all but the last
      recieve_buffer[i] = Wire.read();
      i++;
    }
    int num = recieve_buffer[1] | recieve_buffer[2] << 8;
    if(num > 1000 && num < 2000){
       water_off_flag = TRUE;
    }
    if(num> 2000){
       water_off_flag = FALSE;
    }
    else{
      moisture_threshold = num;
    }
  }
  //print_flag = 1;
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  
  moisture = analogRead(2);
    if(moisture < moisture_threshold && water_off_flag == FALSE){
      digitalWrite(relayPin, 0x1);
      delay(500);
      digitalWrite(relayPin, 0x0);
      delay(5000);
    }

  /*
  if(print_flag == 1){
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    Serial.print("Moisture Sensor Value:");
    Serial.println(moisture);
    Serial.print("Moisture threshold: ");
    Serial.println(moisture_threshold);
    print_flag = 0;
    
  }
  */

  //prepare temp data for i2c write
  float temp = DHT.temperature;
  int tempsend = (int)temp;
  sendData[0] = (tempsend >>8);
  sendData[1] = (tempsend >>0);
  //prepare humidity data for i2c write
  float humidity = DHT.humidity;
  int humiditysend = (int)humidity;
  sendData[2] = (humiditysend >>8);
  sendData[3] = (humiditysend >>0);
  //prepare moisture data for i2c write
  int moisturesend = (int)moisture;
  sendData[4] = (moisturesend >>8);
  sendData[5] = (moisturesend >>0);

  /*Serial.print("moisturesend: ");
  Serial.println(moisturesend);
  Serial.print("moisture: ");
  Serial.println(moisture);*/
  /*
  Serial.println(sendData)
  Serial.println(DHT.temperature);
  Serial.print(sendData[0]);
  Serial.print(" ");
  Serial.print(sendData[1]);
  Serial.print(" ");
  Serial.print(sendData[2]);
  Serial.print(" ");
  Serial.println(sendData[3]);
  */
  delay(2000);
  }
