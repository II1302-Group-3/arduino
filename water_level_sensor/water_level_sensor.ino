

// Include the Wire library for I2C


const int waterPin = 7;
int val = 0;


void setup() {  
  //Set water pin as input
  pinMode(waterPin, INPUT);

  //start serial comm over USB
  Serial.begin(9600);
}

void loop() {

  // 1 - not enough, 0 - enough
  val = digitalRead(waterPin);
  if(val == 0){
    Serial.println("Enough water");
  }
  else{
    Serial.println("Achtung! Not enough water");
  }
  delay(1000);
}