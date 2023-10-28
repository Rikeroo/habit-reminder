#include <Arduino.h>
#include <EasyButton.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int neoPin = 5;
int numPixels = 3;

int boxLength = 103; // Menu box length
int boxWidth = 20; // Menu box width
int symbolX = 109; // Symbol X draw coord
int symbolY = 16; // Initial symbol Y

int button1Pin = 35; // Left most button pin
int duration = 2000; // Long Press Duration

// Menu naviagtion parameters
int menuOption = 0;

// Define task class
class Task {
  public:
    std::string text;
    bool complete;
};

// Define Tasks
Task task1 = {"Take Multi-Vitamins", false};
Task task2 = {"Do Physio Exercises", false};
Task task3 = {"Check Calendar", false};

// Define buttons
EasyButton button1(button1Pin);

Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

void buttonPress() {
  Serial.println("Button pressed");
  pixels.setPixelColor(1, pixels.Color(0,150,0));
  pixels.show();
  
  // Increment option and reset at end
  if (menuOption < 2) {
    menuOption = menuOption + 1;
  } else {
    menuOption = 0;
  }
  // Set Symbol Y based on menu option
  symbolY = 16 + (menuOption*22);
  
  Serial.println(menuOption);
}

void onPressedForDuration() {
  Serial.println("Button Long Press");

  // Draw tick at selected option
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(symbolX,symbolY, 0x2714); //Draw tick
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
  u8g2.firstPage();
  do {
    //u8g2.setFont(u8g2_font_squeezed_b7_tr);
    u8g2.setFont(u8g2_font_squeezed_r7_tr);

    // Draws Tasks Text
    u8g2.drawStr(3,17,(task1.text).data());
    u8g2.drawStr(3,39,(task2.text).data());
    u8g2.drawStr(3,61,(task3.text).data());

    // Draws Menu Boxes and check boxes
    for (int i = 0; i < 3; i++) {
      u8g2.drawRFrame(0,i*22,103,20,3); //Draws Textboxes
      u8g2.drawRFrame(105,i*22,22,20,3); //Draws Checkboxes

      // Define cross draw coordinates
      int crossX = 109;
      int crossY = 16+(i*22);

      // Draw only first time
      for (int j = 0; j<1; j++) {
        // Draw crosses in boxes
        u8g2.setFont(u8g2_font_unifont_t_symbols);
        u8g2.drawGlyph(crossX,crossY, 0x2715);
      }
      
    }
    // Menu navigation
    
    
  } while ( u8g2.nextPage() );

  // Poll buttons
  button1.read();
}
