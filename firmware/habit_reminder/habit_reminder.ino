#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Arduino.h>
#include <EasyButton.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// WiFi Details
const char* ssid = "BLAH";
const char* password = "BLAH";

// Setup NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request String heaeder;
String header;

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// NeoPixel Constants
int neoPin = 5;
int numPixels = 3;

// Define menu box parameters
int boxLength = 103; // Menu box length
int boxWidth = 20; // Menu box width
int symbolX = 109; // Symbol X draw coord
int symbolY = 16; // Initial symbol Y

// Define button pins
int button1Pin = 32; // Left-most button pin
int button2Pin = 33; // Middle button
int button3Pin = 27; // Right-most button

int duration = 2000; // Long Press Duration

// Define LED Pins
int led1Pin = 18;
int led2Pin = 5;
int led3Pin = 17;

// Define LED class
class Led {
  public:
    // Atributes
    int pin;
    bool lit;
    // Constructor
    Led(int x, bool y) {
      pin = x;
      lit = y;
      pinMode(pin, OUTPUT);
    }
    void toggle(){
      if (lit == true) {
        digitalWrite(pin, LOW);
      } else {
        digitalWrite(pin, HIGH);
      }
    }
    void turnOn(){
      digitalWrite(pin, HIGH);
    }
    void turnOff(){
      digitalWrite(pin, LOW);
    }
    bool getStatus(){
      return lit;
    }
};

// Define Led objects - default state on
Led led1(led1Pin,true);
Led led2(led2Pin,true);
Led led3(led3Pin,true);

// Put Led objects into array
Led leds[3] = {led1,led2,led3};

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
    void resetTask(){
      complete = false;
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
EasyButton button2(button2Pin);
EasyButton button3(button3Pin);

// Put buttons into Array
EasyButton buttons[3] = {button1,button2,button3};

// Initialise display
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// Define function for each button (required)
void onPress1() {bPress(0);}
void onPress2() {bPress(1);}
void onPress3() {bPress(2);}
void bPress (int num) {
  tasks[num].toggle();
  Serial.println(num);
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8g2.begin();
  pixels.begin();
  WiFi.begin(ssid, password);
  
  // Initialise Buttons
  button1.begin();
  button2.begin();
  button3.begin();

  // Attatch Callbacks for buttons
  button1.onPressed(onPress1);
  button2.onPressed(onPress2);
  button3.onPressed(onPress3);

  // Sets task LEDs to default red
  pixels.clear();
  for(int i = 0; i < numPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(150,0,0));
    pixels.show();
  }
  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print (".");
  }

  timeClient.begin();
  
  // Print local IP adress
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Begin server
  server.begin();
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  u8g2.firstPage();
  do {
    // Draws Menu Boxes and check boxes
    for (int i = 0; i < 3; i++) {
      // Draws Tasks text
      u8g2.setFont(u8g2_font_t0_11_tf);
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

  // Update LED conditions
  for (int i=0; i<3; i++) {
    // Turn off Led if task complete
    if (tasks[i].getComplete() == true) {
      leds[i].turnOff();
    } else {
      leds[i].turnOn();
    }
  }

  // Update time client
  timeClient.update();
  Serial.println(timeClient.getHours());

  // Reset Tasks at 1 am
  if (timeClient.getHours() == 01) {
    for (int i = 0; i < 3; i++){
      tasks[i].resetTask();
    }
  }
  
  // Poll buttons
  button1.read();
  button2.read();
  button3.read();
}
