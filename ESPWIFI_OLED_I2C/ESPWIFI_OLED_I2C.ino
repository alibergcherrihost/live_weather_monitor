/********************************************************************/
/* ESPWIFI_OLED_I2C.ino
 * Author: Gabriel Christopher
 * Date: 2022/11/11
 * Function: Displays real-time weather information for a specified location obtained from an API.
 * Mini weather monitor that can be powered by battery.
 * This code is for a 128x64 pixel Monochrome OLED display 
 * based on SSD1306/SH1106 drivers using I2C to communicate.
 * 3 pins are required to interface (two I2C and one reset).
 * Version: 1.1 */
/********************************************************************/

#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SH110X.h>  // uncomment if you're using an SH1106 driver
//#include <Adafruit_SSD1306.h> // uncomment if you're using an SSD1306 driver
#include <string.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans6pt7b.h>

//--------------------------------------------------------------------
// Let's create the OLED display
// uncomment the below lines if you are using an SH1106 based display
/*
#define OLED_RESET -1                 // Reset pin # (or -1 if sharing Arduino reset pin)
#define i2c_Address 0x3c              // initialize with the I2C addr 0x3c Typically Adafruit OLED's
Adafruit_SH1106G d1 = Adafruit_SH1106G(128, 64, &Wire, OLED_RESET);  //let's define our display
*/
// uncomment the below lines if you are using an SSD1306 based display
/*
#define OLED_RESET  4                 // Reset pin # (or -1 if sharing Arduino reset pin)
#define i2c_Address 0x3D              // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 d1(128, 64, &Wire, OLED_RESET);
*/
//-------------------------------------------------------------------------------------------------
boolean connectWifi(); // Establishes a WiFi connection with predefined credentials

uint8_t ledState = LOW;               // Variable to set the LED state
unsigned long previousMillis = 0;     // Last time the LED was updated
const long interval = 500;            // Interval at which to blink the LED (milliseconds)

// WiFi Credentials
const char* ssid = "TP-LINK-2.4";
const char* password = "feddyduke94";

boolean wifiConnected = false;        // Flag to track WiFi connection status
//-------------------------------------------------------------------------------
// hex array needed for the icons.
const unsigned char PROGMEM droplet [] = {
  0x00, 0x18, 0x18, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 
};
//-------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);                // Uncomment to enable debug output on the Serial port during development
  pinMode(LED_BUILTIN, OUTPUT); 
  delay(250); // Wait for the OLED to power up
  //d1.begin(SSD1306_SWITCHCAPVCC, i2c_Address); // uncomment if you are using an SSD1306 based display
  //d1.begin(i2c_Address, true);                 // uncomment if you are using an SH1106 based display  
  d1.setTextColor(SH110X_WHITE);                 // Use SSD1306_WHITE for SSD1306 & SH110X_WHITE for SH1106 
  d1.clearDisplay();      
  d1.setContrast(0); // Dim the display
  Serial.println();
  Serial.println();
  Serial.println();
  
  // Initialize WiFi connection
  wifiConnected = connectWifi();
}

//------------------------------------------------------------------------------------------
void loop() {
  if (wifiConnected) {
    WiFiClient client;
    HTTPClient http, http2, http3, http4, http5;
    
    // Fetch data from the first Thingspeak channel
    if (http.begin(client, "http://api.thingspeak.com/apps/thinghttp/send_request?api_key=TTCGPRLRMU6M7DJJ")) {
      // Start connection and send HTTP header
      int httpCode = http.GET();
      // Check if the HTTP request was successful
      if (httpCode > 0) {
        // Check if the file was found at the server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // Parse and display weather data from the response
          String payload = http.getString();
          Serial.println(payload);
          d1.clearDisplay();
          d1.setFont();
          d1.setTextSize(2);
          d1.setCursor(43,2);    // Coordinates for temperature symbol
          d1.println((char)248); // Display degree character
          d1.setTextSize(1);
          d1.setFont(&FreeSans18pt7b);
          d1.setCursor(0,25);    // Coordinates for main weather data
          d1.print(payload);
          d1.setTextSize(1);
          d1.setFont(&FreeSans12pt7b);
          d1.setCursor(52,25);   // Coordinates for 'C' (Celsius)
          d1.println("C");
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
       }
      http.end();
    }
    // Similar blocks for fetching data from other Thingspeak channels (http2, http3, http4, http5)...
//--------------------------------------------------------------------------------------------------------------
    if (http2.begin(client, "http://api.thingspeak.com/apps/thinghttp/send_request?api_key=HYULMRJCST28OVIQ")) {
      int httpCode2 = http2.GET();
      if (httpCode2 > 0) {
        if (httpCode2 == HTTP_CODE_OK || httpCode2 == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload2 = http2.getString();
          Serial.println(payload2);
          //if(payload2.length()<=10)d1.setFont(&FreeSans9pt7b);
          d1.setFont(&FreeSans6pt7b);
          d1.setTextSize(1);
          d1.setCursor(0,43);
          d1.println(payload2);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http2.errorToString(httpCode2).c_str());
      }
      http2.end();
    }
//--------------------------------------------------------------------------------------------------------------
    if (http3.begin(client, "http://api.thingspeak.com/apps/thinghttp/send_request?api_key=HVWDBT5YHWW4DDX9")) {
      int httpCode3 = http3.GET();
      if (httpCode3 > 0) {
        if (httpCode3 == HTTP_CODE_OK || httpCode3 == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload3 = http3.getString();
          Serial.println(payload3);
          //if(payload2.length()<=10)d1.setFont(&FreeSans9pt7b);
          d1.drawBitmap(91,1,droplet,8,8,1);
          d1.setFont(&FreeSans6pt7b);
          d1.setTextSize(1);
          d1.setCursor(103,8);
          d1.print(payload3);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http3.errorToString(httpCode3).c_str());
      }
      http3.end();
    }
//--------------------------------------------------------------------------------------------------------------
    if (http4.begin(client, "http://api.thingspeak.com/apps/thinghttp/send_request?api_key=KFJ0YAN5T2E0TE6H")) {
      int httpCode4 = http4.GET();
      if (httpCode4 > 0) {
        if (httpCode4 == HTTP_CODE_OK || httpCode4 == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload4 = http4.getString();
          Serial.println(payload4);
          d1.setFont(&FreeSans6pt7b);
          d1.setTextSize(1);
          d1.setCursor(86,25);
          d1.print(payload4.substring(10));
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http4.errorToString(httpCode4).c_str());
      }
      http4.end();
  }
//--------------------------------------------------------------------------------------------------------------
    if (http5.begin(client, "http://api.thingspeak.com/apps/thinghttp/send_request?api_key=EG0PDG5KDHXWBODY")) {
      int httpCode5 = http5.GET();
      if (httpCode5 > 0) {
        if (httpCode5 == HTTP_CODE_OK || httpCode5 == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload5 = http5.getString();
          Serial.println(payload5);
          d1.setFont(&FreeSans6pt7b);
          d1.setTextSize(1);
          d1.setCursor(0,60);
          d1.print(payload5);       
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http5.errorToString(httpCode5).c_str());
      }
      http5.end();
  }
//--------------------------------------------------------------------------------------------------------------
  } else {
    // If WiFi connection fails, enter an infinite loop displaying an error message
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
  // Update the OLED display
  d1.display();
}

//------------------------------------------------------------------------------------
void ledblink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Toggle the LED state
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
}
//----------------------------------------------------------------------------------------------------------
// Connect to WiFi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  d1.clearDisplay();
  d1.setCursor(0,0);
  d1.print("Connecting..");
  d1.display();

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    ledblink();
    Serial.print(".");
    if (i > 20) {
      state = false; break;  // Break out of the loop if unsuccessful after 20 attempts
    }
    i++;
  }
  Serial.println("");
  if (state) {
    digitalWrite(LED_BUILTIN,HIGH);  // Turn on the LED for successful connection
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    d1.clearDisplay();
    d1.setCursor(0,0);
    d1.print("Connected to\n\n");
    d1.println(ssid);
    d1.display();
    delay(1000);
  } else {   
    Serial.println("Connection failed.");
    d1.clearDisplay();
    d1.setCursor(0,0); //55,43
    d1.print("Connection Failed!");
    d1.display();
    delay(2000);
  }
  return state;
}
//----------------------------------------------------------------------------------------
