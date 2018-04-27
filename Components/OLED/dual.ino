//Use for testing two OLED Screens at once
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 Display1(OLED_RESET);
Adafruit_SSD1306 Display2(OLED_RESET);

int i, j;

void setup()   {
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display2.clearDisplay();
  Display2.display();
}

void loop() 
  for (i = 0; i < 270; i += 10) {
    Display1.setTextSize(2);
    Display1.setTextColor(WHITE, BLACK);
    Display1.setCursor(0, 0 );
    Display1.println("Display 1");
    Display1.fillCircle(i, 30,  10, 1);
    Display1.display();
    Display1.clearDisplay();

    Display2.setTextSize(2);
    Display2.setTextColor(WHITE, BLACK);
    Display2.setCursor(0, 0);
    Display2.println("Display 2");
    Display1.fillCircle(i - 127, 30,  10, 1);
    Display2.display();
    Display2.clearDisplay();
  }
}
