/* Aether Code
    Will Langas and Jack Kelly
    4/15/2018
    Don't Steal our code plz
*/

/***********/
/*INTERFACE*/
/***********/
int toggleButton = 2; 

/****************/
/*OLED VARIABLES*/
/****************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

Adafruit_SSD1306 Display1(OLED_RESET); //Initializes a new display object
Adafruit_SSD1306 Display2(OLED_RESET);
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

/**************/
/*CO VARIABLES*/
/**************/
int SensorReading;
float PPM;

int HeaterPin5 = 44;
int HeaterPin15 = 22;

/***************/
/*CCS VARIABLES*/
/***************/
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

/***************/
/*DHT VARIABLES*/
/***************/
#include <dht.h>
#define dataPin 4 // Defines pin number to which the sensor is connected
dht DHT; // Creats a DHT object

/***************/
/*RGB VARIABLES*/
/***************/
int redPin = 33;
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
int pm = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/**************/
/*   ARRAYS   */
/**************/
String measures[] = {"Humidity", "Temp", "PM", "VOC", "CO2", "CO"};
int textSizes[] = {2, 3, 3, 3, 3, 3};
float values[6];

void setup() {

  pinMode(toggleButton, INPUT);
  
  Serial.begin(9600);
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3D); //DISPLAY LEFT
  Display1.clearDisplay();
  Display1.display();

  Display1.setCursor(30, 8);
  Display1.setTextSize(2);
  Display1.setTextColor(WHITE, BLACK);

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3C); //DISPLAY RIGHT
  Display2.clearDisplay();
  Display2.display();

  Display2.setCursor(30, 8);
  Display2.setTextSize(1);
  Display2.setTextColor(WHITE, BLACK);
  Display2.clearDisplay();

  Display2.println("Will");
  Display2.display();
  Display1.println("AETHER");
  Display1.display();

  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }

  while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  Serial.begin(9600);
  pinMode(pm, INPUT);
  starttime = millis(); //Current time

  pinMode(redPin, OUTPUT);
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
void setColor(int redValue, int greenValue, int blueValue) {
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

  values[0] = h;
  values[1] = t;
}

/**************/
/*PM FUNCTIONS*/
/**************/
void pmOut() {
  duration = pulseIn(pm, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
  concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
  Serial.print("concentration = ");
  Serial.print(concentration);
  Serial.println(" pcs/0.01cf");
  Serial.println("\n");
  lowpulseoccupancy = 0;
  starttime = millis();

  if (concentration >= 200) {
    setColor(255, 0, 0);
  } else {
    setColor(0, 255, 0);
  }
  values[2] = concentration;
}

/***************/
/*VOC FUNCTIONS*/
/***************/
void vocOut() {
  int voc = ccs.getTVOC();
  int co2 = ccs.geteCO2();
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

  if (voc >= 150) {
    setColor2(255, 0, 0);
  } else {
    setColor2(0, 255, 0);
  }
  values[3] = voc;
  values[4] = co2;
}

/*********************/
/*CO SENSOR FUNCTIONS*/
/*********************/
void clean() {
  for (int i = 0; i < 60; i++) {
    digitalWrite(HeaterPin5, HIGH);
    Serial.println("cleaning");
    delay(1000);
    fastSensors();
  }
  digitalWrite(HeaterPin5, LOW);
}

void co() {
  for (int i = 0; i < 90; i++) {
    digitalWrite(HeaterPin15, HIGH);
    SensorReading = analogRead(A1);
    PPM = .5 * SensorReading - 19.355;
    Serial.print (PPM);
    Serial.println (" CO PPM");
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

  int c = 0;

  if(toggleButton == 1){
    delay(500);
    bothClear();
    Display1.setTextSize(textSizes[c]);
    Display1.println(measures[c]);
    Display2.println(values[c]);
    bothDisplay();
  }
}
