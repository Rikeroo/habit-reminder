#include <Arduino.h>
#include <EasyButton.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int neoPin = 5;
int numPixels = 3;

int boxLength = 103; // Menu box length
int boxWidth = 20; // Meny box width

int button1Pin = 35; // Left most button pin
int duration = 2000; // Long Press Duration

// Define buttons
EasyButton button1(button1Pin);

void tickBox() {
  
}

Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

void buttonPress() {
  Serial.println("Button pressed");
  pixels.setPixelColor(1, pixels.Color(0,150,0));
  pixels.show();
}

void onPressedForDuration() {
  Serial.println("Button Long Press");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8g2.begin();
  pixels.begin();
  
  // Initialise Buttons
  button1.begin();

  // Sets task LEDs to default red
  pixels.clear();
  for(int i = 0; i < numPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(150,0,0));
    pixels.show();
  }

  // Attatch Callbacks for single and long press
  button1.onPressed(buttonPress);
  button1.onPressedFor(duration, onPressedForDuration);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(10); // this speeds up the simulation

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_squeezed_r7_tr);

    // Draws Tasks Text
    u8g2.drawStr(3,17,"Take Multi-vitamins");

    u8g2.drawStr(3,39,"Do Physio Exercises");

    u8g2.drawStr(3,61,"Check Calendar");

    // Draws Menu Boxes and check boxes
    for (int i = 0; i < 3; i++) {
      u8g2.drawRFrame(0,i*22,103,20,3); //Draws Textboxes
      u8g2.drawRFrame(105,i*22,22,20,3); //Draws Checkboxes

      // Define cross draw coordinates
      int crossX = 109;
      int crossY = 16+(i*22);
      
      // Draw crosses in boxes
      u8g2.setFont(u8g2_font_unifont_t_symbols);
      u8g2.drawGlyph(crossX,crossY, 0x2716);
      
    }
  } while ( u8g2.nextPage() );

  // Poll buttons
  button1.read();
}
