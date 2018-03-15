#include <dht.h>
#define dataPin 8 // Defines pin number to which the sensor is connected

dht DHT; // Creats a DHT object

void setup() {
  Serial.begin(9600);
}

void loop() {
  int readData = DHT.read22(dataPin); // Reads the data from the sensor
  float temp = DHT.temperature; // Gets the values of the temperature
  float humid = DHT.humidity; // Gets the values of the humidity
  
  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(humid);
  Serial.println(" % ");
  
  delay(4000); // Delays 4 seconds
}
