#include <Wire.h> //Include wire library 

void setup(){
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);
  Serial.println("I2C Scanner, Intialized");
}
 
void loop(){
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning");
 
  nDevices = 0; 
  for(address = 1; address < 127; address++ ){ //Loops through all bit addresses
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    
    } else if (error==4){
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);  // wait 5 seconds for next scan
} //End of void loop
