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

// Define Task class
class Task {
  public:
    // Attributes
    std::string text;
    bool complete;
    
    // Constructor
    Task(std::string x, bool y) {
      text = x;
      complete = y;
    }
    // Toggles task completeness
    void toggle(){
      complete = !complete;
    }
    bool getComplete(){
      return complete;
    }
    std::string getText(){
      return text;
    }
};

// Define Task objects
Task task1("Take Vitamins", false);
Task task2("Do Physio", false);
Task task3("Check Calendar", false);

// Put objects into array
Task tasks[3] = {task1,task2,task3};

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

  // Debugging
  Serial.print("Menu Option: ");
  Serial.println(menuOption);
  Serial.print("Task text: ");
  Serial.println(tasks[menuOption].text.data());
  Serial.print("Task Completeness: ");
  Serial.println(tasks[menuOption].getComplete());
  Serial.println("");
}

void onPressedForDuration() {
  Serial.println("Button Long Press");
  // Toggles task completeness
  tasks[menuOption].toggle();
  
  Serial.println(tasks[menuOption].getComplete());
  
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
    // Draws Menu Boxes and check boxes
    for (int i = 0; i < 3; i++) {
      // Draws Tasks text
      if (i == menuOption) {
        u8g2.setFont(u8g2_font_t0_11b_tf);
      } else {
        u8g2.setFont(u8g2_font_t0_11_tf);
      }
      u8g2.drawStr(3,17+(i*22),(tasks[i].text).data());
      
      u8g2.drawRFrame(0,i*22,103,20,3); //Draws Textboxes
      u8g2.drawRFrame(105,i*22,22,20,3); //Draws Checkboxes

      // Define cross draw coordinates
      int crossX = 109;
      int crossY = 16+(i*22);

      // Draw cross or tick based on task complete
      if (tasks[i].getComplete() == false) {
        u8g2.setFont(u8g2_font_unifont_t_symbols);
        u8g2.drawGlyph(crossX,crossY, 0x2715);
      } else if (tasks[i].getComplete() == true){
        u8g2.setFont(u8g2_font_unifont_t_symbols);
        u8g2.drawGlyph(crossX,crossY, 0x2714);
      }
      
    }
    
  } while ( u8g2.nextPage() );

  // Poll buttons
  button1.read();
}
