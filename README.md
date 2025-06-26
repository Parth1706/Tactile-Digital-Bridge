# Tactile-Digital Bridge: A Morse Code Messaging System

A hybrid communication platform that combines tactile Morse code input with a modern web-based messaging interface using ESP32. This project serves as an educational tool, assistive communication device, and a proof-of-concept for integrating legacy telegraphy with contemporary protocols.

## 📌 Features

- **Morse Code Input** via physical buttons
- **WebSocket-Based Web Interface** hosted on ESP32
- **Real-Time Notifications** using buzzer and LED
- **Local Display** using 16x2 I2C LCD
- **Message History** navigation through tactile buttons

## 🔧 Hardware Used

- ESP32 microcontroller
- 16x2 I2C LCD
- Tactile buttons (dot, dash, up, down)
- Buzzer (2kHz tone)
- LED (status & notifications)

## 💻 Software Components

- `ESPAsyncWebServer` & `AsyncTCP` libraries
- WebSocket (RFC 6455) server
- Finite State Machine for Morse decoding
- HTML/JS web interface (stored in SPIFFS)

## 🚀 Getting Started

1. **Install Libraries**:
   - [`ESPAsyncWebServer`](https://github.com/me-no-dev/ESPAsyncWebServer)
   - [`AsyncTCP`](https://github.com/me-no-dev/AsyncTCP)

2. **Prepare Web Interface Files**:
   - Place your HTML/JS files in the `/data` folder
   - Upload them to ESP32 using the **ESP32 Sketch Data Upload** tool

3. **Upload the Sketch**:
   - Open `Tactile_Digital_Bridge.ino` in Arduino IDE
   - Select the correct ESP32 board and port
   - Compile and upload

4. **Access the Interface**:
   - Connect to the ESP32's Wi-Fi hotspot
   - Open browser at `http://192.168.4.1` to use the chat interface

## 📚 Applications

- Assistive communication for individuals with motor disabilities
- Emergency signaling in low-connectivity environments
- Hands-on learning of Morse code and WebSocket protocols




