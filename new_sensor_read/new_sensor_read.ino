#include <dht.h>
// Include the Wire library for I2C
#include <Wire.h>

dht DHT;

#define DHT11_PIN 7

const int relayPin = 8; 
char buff[10];
//unsigned char sendData[4];
unsigned char sendData[4];
int index = 0;

double moisture;
double moisture_threshold = 0;
double light_threshold;
double c = 0;
char print_flag = 0;


//converter union structure

union {
    float ival;
    byte bval[4];
} floatAsBytes;

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


byte* toByteArray(float val,int size){
    floatAsBytes.ival = val;
    byte* byteArr = new byte[size];   // Dynamically allocate byte array
    for(int i = 0; i < 4; i++){
        byteArr[i] = floatAsBytes.bval[i];
    }
    return byteArr;
}



// Function that executes whenever data is received from master
void receiveEvent(int howMany) {

  while (Wire.available()) { // loop through all but the last
    //char c = Wire.read(); // receive byte as a character
  moisture_threshold = Wire.read()*4;
  }
  print_flag = 1;
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  
  moisture = analogRead(2);
  
  if(moisture < moisture_threshold){
    digitalWrite(relayPin, 0x1);
    delay(500);
    digitalWrite(relayPin, 0x0);
    delay(5000);
  }
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
  for(int i = 0; i < 4; i++){
    float test = DHT.temperature;
    sendData[i] = toByteArray(test,4)[i];
    Serial.println(test);
    
  }

  delay(2000);
  }

  /*void getTemp(){
       for(int i = 0; i < 4; i++){
        sendData[i] = toByteArray(DHT.temperature,4)[i];
    }
  }*/

  void sendEvent(){
    /*floatAsBytes.ival = DHT.humidity;
    for(int i = 0; i < 4; i++){
        sendData[i] = floatAsBytes.bval[i];
    }*/
 
    Wire.write(sendData[index]);
    ++index;
    if(index >= sizeof(sendData)){
      index = 0;
    }
  }
