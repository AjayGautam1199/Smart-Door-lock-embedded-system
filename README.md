# Smart Door Lock Embedded System

This project showcases a smart door lock system using an ESP32 WROOM module, a TFT display, a motor for door operations, and a matrix keypad. Future enhancements will include camera integration and cloud-based monitoring to track visitors.

## Features
- TFT display for user interaction
- Motor control for opening and closing the door
- Matrix keypad for inputting access codes
- Planned future updates: camera integration and cloud-based visitor monitoring
![Demo](lock_sys.gif)
## Hardware Required
- ESP32 WROOM module
- TFT display (compatible with SPI interface)
- DC motor with driver or servo motor
- Matrix keypad
- Breadboard and jumper wires
- Power supply (5V)

## Software Required
- Arduino IDE
- ESP32 board support package for Arduino IDE
- TFT display library (e.g., Adafruit_GFX and Adafruit_ILI9341)
- Keypad library

## Wiring Diagram
| ESP32 WROOM Pin | Component         | Pin        |
| --------------- | ----------------- | ---------- |
| 3.3V            | TFT Display       | VCC        |
| GND             | TFT Display       | GND        |
| GPIO  5         | TFT Display       | CS         |
| GPIO  2         | TFT Display       | DC/RS      |
| GPIO 23         | TFT Display       | MOSI       |
| GPIO 19         | TFT Display       | MISO       |
| GPIO 18         | TFT Display       | SCK        |
| GPIO 4          | TFT Display       | RST        |
| GPIO 15         | Motor Driver/Servo| Signal     |
| GPIO 13,12,14,27| Keypad Rows       | R1-R4      |
| GPIO 26,25,33,32| Keypad Columns    | C1-C4      |
| GPIO  34        | redled            |            |
| GPIO  36        | greenled          |            |
| GPIO  35        | buzzer            |            |
**Note:** Adjust the GPIO pins as per your wiring.

## Installation
1. Clone the repository:

2. Install the required libraries in Arduino IDE:
    - **ESP32**
    - **Adafruit GFX Library**
    - **Adafruit ILI9341**
    - **Keypad**

3. Open the `doorlock.ino` file in Arduino IDE.

4. Select your board and port:
    - **Board**: `ESP32 Wrover Module`
    - **Port**: Select the correct port for your ESP32.

5. Upload the code to your ESP32.

## Usage
After uploading the code and powering on the system, you can use the keypad to input access codes. The TFT display will show the status, and the motor will control the door based on the input. Future updates will enable camera-based monitoring and cloud integration.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

Feel free to contribute to this project by opening issues or submitting pull requests.

Enjoy your smart door lock system!
