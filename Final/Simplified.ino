/* Aether Code
    Will Langas and Jack Kelly
    4/15/2018
    Don't Steal our code plz
*/

//Simplified version, no toggling through display screens.

float index = 100;
float finalPPM = 0;
/***********/
/*INTERFACE*/
/***********/
int toggleButton = 2; //For the array toggling on OLEDs

float h; //Humidity
float t; //Temperature

/****************/
/*OLED VARIABLES*/
/****************/
#include <SPI.h>
#include <Wire.h> //l2c Library 
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

Adafruit_SSD1306 Display1(OLED_RESET); //Initializes a new display object (Display1)
Adafruit_SSD1306 Display2(OLED_RESET); //Second Display (Display2)
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

/**************/
/*CO VARIABLES*/
/**************/
int SensorReading;
float PPMnow;
float PPMaverage;

int HeaterPin5 = 44;
int HeaterPin15 = 22;

/***************/
/*CCS VARIABLES*/
/***************/
#include "Adafruit_CCS811.h" //Library for VOC sensor
Adafruit_CCS811 ccs;


/***************/
/*DHT VARIABLES*/
/***************/
#include <dht.h> //DHT library 
#define dataPin 4 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object

/***************/
/*RGB VARIABLES*/
/***************/
int redPin = 33; //Each pin corresponds to a R G or B pin on an LED
int greenPin = 35;
int bluePin = 37;

int redPin2 = 39;
int greenPin2 = 41;
int bluePin2 = 43;

int redPin3 = 45;
int greenPin3 = 47;
int bluePin3 = 49;

/**************/
/*PM VARIABLES*/
/**************/
int pm = 8; //Input pin for PM sensor
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/**************/
/*   ARRAYS   */
/**************/
String measures[] = {"Humidity", "Temp", "PM", "VOC", "CO2", "CO"}; //UNUSED
float values[6];

void setup() {

  pinMode(toggleButton, INPUT); //UNUSED

  Serial.begin(9600);

  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3D); //DISPLAY LEFT
  Display1.clearDisplay(); //Clears the display
  Display1.display();

  Display1.setCursor(30,8);
  Display1.setTextSize(2);
  Display1.setTextColor(WHITE, BLACK);

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3C); //DISPLAY RIGHT
  Display2.clearDisplay();
  Display2.display();

  Display2.setCursor(5,0);
  Display2.setTextSize(1);
  Display2.setTextColor(WHITE, BLACK);
  Display2.clearDisplay();

  Display1.println("Aether");
  Display1.display();
  Display2.clearDisplay();
  Display2.println("Digital Electronics"); //Intro/Loading Screen
  Display2.setCursor(30, 10);
  Display2.println("Will Langas");
  Display2.display();
  Display2.setCursor(30, 22);
  Display2.println("Jack Kelly");
  Display2.display();

  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }

  while (!ccs.available());
  float temp = ccs.calculateTemperature(); //Starts up VOC sensor
  ccs.setTempOffset(temp - 25.0);

  Serial.begin(9600);
  pinMode(pm, INPUT);
  starttime = millis(); //Current time

  pinMode(redPin, OUTPUT); //RGB Pins
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);
  pinMode(redPin3, OUTPUT);
  pinMode(greenPin3, OUTPUT);
  pinMode(bluePin3, OUTPUT);
  pinMode(9,OUTPUT); //Reset pin
  pinMode(HeaterPin5, OUTPUT);
  pinMode(HeaterPin15, OUTPUT);

  delay(5000);
  Display1.clearDisplay();
  Display2.clearDisplay();
  bothDisplay();
}

/****************/
/*OLED FUNCTIONS*/
/****************/
void bothDisplay() {
  Display1.display();
  Display2.display();
}

void bothClear() {
  Display1.clearDisplay();
  Display2.clearDisplay();
  bothDisplay();
}

/***************/
/*RGB FUNCTIONS*/
/***************/
void setColor(int redValue, int greenValue, int blueValue) { //Writes an RGB value to the LED setColor1(255,0,0)
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setColor2(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin2, redValue);
  analogWrite(greenPin2, greenValue);
  analogWrite(bluePin2, blueValue);
}

void setColor3(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin3, redValue);
  analogWrite(greenPin3, greenValue);
  analogWrite(bluePin3, blueValue);
}

int everything(float in) {
  
  
  
  //DHT
  int readData = DHT.read22(dataPin); // Reads the data from the sensor

  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity

  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");




  //PM
  duration = pulseIn(8, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
 
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    lowpulseoccupancy = 0;
    starttime = millis();


  //VOC
  int voc = ccs.getTVOC();
  int co2 = ccs.geteCO2(); //Acquires VOC and CO2 Measurements, REMOVE CO2 COMPLETELY???
  if (ccs.available()) {
    float temp = ccs.calculateTemperature();
    if (!ccs.readData()) {
      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.print("ppm, TVOC: ");
      Serial.print(voc);
      Serial.print("ppb   Temp:");
      Serial.println(temp);
    }
    else {
      Serial.println("ERROR!");
      while (1);
    }
  }

 //INDEX CALCULATION
  float vocIndex = (-0.06 * voc) - 4;
  float pmIndex = (-0.096227 * concentration) - 3.3404;
  if(in == 0){
    pmIndex =- 0;
  } else {
    pmIndex =  pmIndex + ((-0.1765 * in) - 12.88);
  }
  
  float index = 100 + vocIndex + pmIndex;

    //RGB CODE
    if (concentration >= 250) { //FOR PM
    setColor2(255, 0, 0);
  } else if (concentration >= 50 && concentration < 250) {
    setColor2(255, 255, 0);
  } else {
    setColor2(0, 255, 0);
  }

  if (voc >= 250) { //FOR VOC
    setColor(255, 0, 0);
  } else if (voc < 250 && voc >= 50) { //Red, green, or yellow on RGB?
    setColor(255, 255, 0);
  } else {
    setColor(0, 255, 0);
  }

  if (index <= 80){
    setColor3(255,0,0);
  } else if(index < 90 && index >= 80){
    setColor3(255,255,0);
  } else {
    setColor3(0,255,0);
  }

 

  //OLED PRINTING

  Display2.setTextSize(1);   //PM AND VOC PRINTING 
  Display2.setCursor(0, 0);
  Display2.println("Concentration: ");
  Display2.setCursor(85, 0);
  Display2.println(concentration); 
   
  Display2.setCursor(0, 20);
  Display2.println("VOC: ");
  Display2.setCursor(25, 20);
  Display2.println(voc);
  Display2.setCursor(53, 20);
  Display2.println("PPB");
  Display2.display();
  Display1.clearDisplay();

  Display1.setTextSize(1);
  Display1.setCursor(0,0);
  Display1.println("Index: ");
  Display1.setCursor(38,0);
  Display1.println(index);
  
  Display1.setCursor(0, 8);  //HUMIDITY AND TEMP
  Display1.println("Humidity: ");
  Display1.setCursor(57, 8);
  Display1.println(h);
  Display1.setCursor(90, 8);
  Display1.println("%");
  Display1.setCursor(0, 20);
  Display1.println("Temp: ");
  Display1.setCursor(30, 20);
  Display1.println(t);
  
  Display1.display();
  Display2.clearDisplay();
}

void clean() {
  for (int i = 0; i < 60; i++) {
    digitalWrite(HeaterPin5, HIGH); //Clean function for CO sensor, 60 iterations
    Serial.println("cleaning");
    delay(1000);
    
    Display2.setTextSize(1);
    Display2.setCursor(0, 8);
    Display2.println("CO: ");
    Display2.setCursor(25, 8);
    Display2.println("Cleaning");
    everything(0);
  }
  digitalWrite(HeaterPin5, LOW);
}

float co() {
  float sumPPM[90];
  
  for (int i = 0; i < 90; i++) {
    digitalWrite(HeaterPin15, HIGH);
    SensorReading = analogRead(A1);
    finalPPM = 0;
    PPMnow = .5 * SensorReading - 19.355;
    Serial.print(PPMnow);
    Serial.println(" CO PPM");
    Display2.setCursor(25,8);
    Display2.println(PPMnow);
    sumPPM[i] = PPMnow;
    
    Display2.setTextSize(1);
    Display2.setCursor(0, 8);
    Display2.println("CO: ");
    Display2.setCursor(25, 8);
    Display2.println(finalPPM);
    everything(finalPPM);
    delay(1000); 
  }
  
  float sum;
  
  digitalWrite(HeaterPin15, LOW);
  for (int i = 0; i < 90; i++) {
    sum += sumPPM[i];
    return sum;
  }

  finalPPM = sum / 90;
  float coIndex = (-0.1765 * finalPPM) - 12.88;

  return finalPPM;
}

void loop(){
  clean();
  co();

}
