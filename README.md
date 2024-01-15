# live_weather_monitor
This program connects to a designated WiFi network, addresses failures via serial output and OLED display, blinks an LED for status. It then makes HTTP requests to Thingspeak channels, fetches weather data, parses it, and dynamically displays relevant information on an OLED screen.

**LIBRARY REQUIREMENTS:**<br />
Adafruit_GFX.h<br />
Adafruit_SH110X.h  (If you're using SH1106 driver-based display)<br />
Adafruit_SSD1306.h (If you're using SSD1306 driver-based display)<br />
ESP8266WiFi.h      (If you're using ESP8266-based boards)<br />

# Instructions for Data Retrieval and API Creation

These instructions guide you on fetching specific data from any website and creating a custom API.

## Steps:

1. **Visit the Source Website:**
   - Go to the website where your desired data is located.
2. **Inspect the Webpage:**
   - Right-click on the area of the webpage containing the information.
   - Select "Inspect" to reveal the HTML code.
3. **Identify Relevant Code:**
   - Locate the highlighted portion of the HTML code that handles the data you want.
4. **Copy Page URL:**
   - Copy the URL of the webpage and save it in a notepad.
5. **Copy XPath:**
   - Find the specific line of code with your desired data.
   - Right-click on the line and select "Copy XPath." Save this information securely.
6. **Choose an IoT Analytics Service:**
   - Select an IoT analytics service provider; for this guide, we'll use ThingSpeak.
7. **Create a ThingHTTP API:**
   - Go to [ThingSpeak's ThingHTTP](https://thingspeak.com/apps/thinghttp).
   - If not already registered, create an account.
8. **Generate API Key:**
   - Select "New ThingHTTP," and an API key will be generated along with additional information.
9. **Rename Your API:**
   - Give your API an appropriate name for clarity.
10. **Configure API Settings:**
    - Paste the copied URL into the "URL" tab.
    - Set the method to "GET." (GET is used to request data from a specified resource)
11. **Define XPath for Parsing:**
    - Paste the copied XPath into the "Parse String" tab. (XPath pinpoints the exact location of desired data by specifying the path from the HTML root to the element)
12. **Save Configuration:**
    - Click "Save ThingHTTP."
13. **Use Your API:**
    - The generated link now contains the extracted data needed for your program.

Now, you can follow these steps to customize the code for your specific purposes.

