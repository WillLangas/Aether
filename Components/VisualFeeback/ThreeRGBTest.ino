//Cycles through various RGB colors on the three LEDs
//Ensures each has been wired correctly and is functioning

int redPin = 5;
int greenPin = 6;
int bluePin = 7;

int redPin2 = 8;
int greenPin2 = 9;
int bluePin2 = 10;

int redPin3 = 11;
int greenPin3 = 12;
int bluePin3 = 13;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);
  pinMode(redPin3, OUTPUT);
  pinMode(greenPin3, OUTPUT);
  pinMode(bluePin3, OUTPUT);
}

void loop() {

  setColor(255, 0, 0); // Red Color
  setColor2(255, 0, 0);
  setColor3(255, 0, 0);
  delay(1000);

  setColor(0, 255, 0); // Green Color
  setColor2(0, 255, 0);
  setColor3(0, 255, 0);
  delay(1000);

  setColor(0, 0, 255); // Blue Color
  setColor2(0, 0, 255);
  setColor3(0, 0, 255);
  delay(1000);

  setColor(255, 255, 255); // White Color
  setColor2(255, 255, 255);
  setColor3(255, 255, 255);
  delay(1000);

  setColor(170, 0, 255); // Purple Color
  setColor2(170, 0, 255);
  setColor3(170, 0, 255);
  delay(1000);
}

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

