/* Aether Code
    Will Langas and Jack Kelly
    4/15/2018
    Don't Steal our code plz
*/

//Simplified version, no toggling through display screens. 

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
int SensorReading; //Jack's stuff, stay away Will
float PPM;
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

  Display1.setCursor(30, 8);
  Display1.setTextSize(2);
  Display1.setTextColor(WHITE, BLACK);

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3C); //DISPLAY RIGHT
  Display2.clearDisplay();
  Display2.display();

  Display2.setCursor(5, 0);
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

/***************/
/*DHT FUNCTIONS*/
/***************/
void dhtOut() {
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

  Display1.setCursor(0, 8);
  Display1.setTextSize(1);
  Display1.clearDisplay();
  Display1.println("Humidity: ");
  Display1.setCursor(57, 8);
  float tf = (t * (9/5)) + 32; //Attempt at printing it in fahrenheit
  Display1.println(h);
  Display1.setCursor(90, 8);
  Display1.println("%");
  Display1.setCursor(0, 20);
  Display1.println("Temp: ");
  Display1.setCursor(30, 20);
  Display1.println(tf);
  Display1.display();
  Display2.clearDisplay();
  Display2.display();

  return h; //Just in case
  return t;
}

/**************/
/*PM FUNCTIONS*/
/**************/
void pmOut() {
  duration = pulseIn(pm, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
  concentration = 1.1 * pow(0, 3) - 3.8 * pow(0, 2) + 520 * 0 + 0.62; // using spec sheet curve
  Serial.print("concentration = ");
  Serial.print(concentration);
  Serial.println(" pcs/0.01cf");
  Serial.println("\n");
  Serial.println(ratio);
  lowpulseoccupancy = 0;
  starttime = millis();

  if(concentration >= 250){
    setColor2(255,0,0);
  } else if(concentration >= 50 && concentration < 250){
    setColor2(255,255,0);
  } else {
    setColor2(0,255,0);
  }
}

/***************/
/*VOC FUNCTIONS*/
/***************/
void vocOut() {
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
    else{ 
      Serial.println("ERROR!");
      while (1);
    }
  }

  if(voc >= 250){
    setColor(255,0,0);
  } else if(voc < 250 && voc >= 50){ //Red, green, or yellow on RGB?
    setColor(255,255,0);
  } else {
    setColor(0,255,0);
  }

  Display2.setCursor(0,20);
  Display2.println("VOC: ");
  Display2.setCursor(25,20);
  Display2.println(voc);
  Display2.setCursor(53,20);
  Display2.println("PPB");
  Display2.display();
}

/*********************/
/*CO SENSOR FUNCTIONS*/
/*********************/
void clean() {
  for (int i = 0; i < 60; i++) {
    digitalWrite(HeaterPin5, HIGH); //Clean function for CO sensor, 60 iterations
    Serial.println("cleaning");
    delay(1000);
    fastSensors();

    Display2.setTextSize(1);
    Display2.setCursor(0,8);
    Display2.println("CO: ");
    Display2.setCursor(25,8);
    Display2.println("Cleaning");
    Display2.display();
  }
  digitalWrite(HeaterPin5, LOW);
}

void co() {
  for (int i = 0; i < 90; i++) { //Averages CO values
    digitalWrite(HeaterPin15, HIGH);
    SensorReading = analogRead(A1);

    PPM = .5 * SensorReading - 19.355;
    Serial.print (PPM);
    Serial.println (" CO PPM");
    fastSensors();
    delay(1000);
  }
  digitalWrite(HeaterPin15, LOW);
  values[5] = PPM;
}

void fastSensors() {
  pmOut();
  dhtOut();
  vocOut();
}

void loop() {
  clean();
  co();
  fastSensors();
}
