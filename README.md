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



