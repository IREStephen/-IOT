# This ESP32 solar monitoring system is something I put together for tracking solar energy in real time. It uses the ESP32 board to grab data from sensors and send it over to ThingSpeak, this IoT platform, and then shows everything on a web dashboard with graphs that updates. I think the way it streams the info makes it feel pretty responsive.

## One thing that stands out is how it handles multiple sensors at once. There are 7 parameters it monitors, like light intensity in lux, voltage, current, power output, temperature, and humidity levels. The dashboard refreshes automatically with these sensor cards, and it connects to the cloud for logging data and making visuals. The setup avoids blocking, so the web server stays up even while sending stuff to ThingSpeak. It seems like that keeps things smooth during transmissions.

## For hardware I used a BH1750 sensor for light, connected via I2C on pins 21 and 22. Then the INA219 handles current and voltage, also on the same I2C bus. The DHT11 for temp and humidity goes to pin 32 digitally. Power is 3.3V shared, and ground is common for all. Pin connections are straightforward, GPIO21 for SDA to both I2C sensors, GPIO22 for SCL, and GPIO32 straight to DHT11 data.

## On the software side, you need a few libraries from the Arduino manager. BH1750 , Adafruit INA219 and DHT sensor. Configuration is just swaping in your Wi-Fi SSID and password in the main .ino file, and put the ThingSpeak Write API key in the code.

## You have to set up a free channel on thingspeak.com first. Enable fields like Field 1 for light lux, 2 for bus voltage in volts, 3 shunt voltage millivolts, 4 current milliamps, 5 power milliwatts, 6 temperature Celsius, 7 humidity percent. The project files include ESP32Project.ino as the main code, homepage.h with the HTML template for the dashboard, and this README.

## How does it run? The dashboard pulls updates every two seconds through AJAX to get fresh sensor readings. ThingSpeak gets points every 20 seconds.

## To use it, upload via Arduino IDE to the ESP32. Open serial monitor at 115200 baud, and it reveals the IP address to use. Type it into a browser on the same network. Then check live charts on your ThingSpeak channel URL for history.

## API wise, root path gives the full HTML dashboard. Serial output shows IP connection, MDNS start, HTTP server ready, and ThingSpeak confirms with 200 codes regularly.

## Troubleshooting can be annoying if things go wrong. Like sensors not showing, check SDA SCL wires not swapped, I2C addresses maybe clashing. DHT11 errors. ThingSpeak failing, API key wrong or Wi-Fi down. Web access issue, make sure devices on same network.

## License is MIT, free for school or business stuff. Author Stephen Duffy, Atlantic TU second year IoT project.

## License
MIT License. Free for educational and commercial use.

## Author
Stephen Duffy. Atlantic TU, 2nd Year IoT Project.