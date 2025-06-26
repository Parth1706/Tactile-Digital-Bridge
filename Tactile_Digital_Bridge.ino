/*
  Tactile-Digital Bridge: Morse Code Messaging System
  Author: Parth Khare
  Description: Allows bidirectional communication using tactile Morse code input and a web-based interface hosted on ESP32.
*/
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "ssid";
const char* password = "password";


WebServer server(80);

const int dashButton = 26;
const int dotButton = 27;
#define BUZZER_PIN 12
#define LED_PIN 14
const int clearButton = 5;

String morseCode = "";
unsigned long lastInputTime = 0;
String decodedMessage = "";
String webMessage = "";

const char* morseAlphabet[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....",
  "-....", "--...", "---..", "----."
};
const char asciiChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

LiquidCrystal_I2C lcd(0x27, 16, 2);

void handleMorseInput();
void updateDisplay();
void decodeMorse();
void connectToWiFi();

void setup() {
  Serial.begin(115200);
  pinMode(dashButton, INPUT_PULLUP);
  pinMode(dotButton, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(clearButton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("WiFi Connecting");

  connectToWiFi();

  server.on("/", handleRoot);

  server.on("/send", HTTP_POST, []() {
    if (server.hasArg("message")) {
      Serial.println("Testing Buzzer and LED...");
      webMessage = server.arg("message");
      Serial.println("Received from browser: " + webMessage);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("Web: ");
      lcd.print(webMessage);
      tone(BUZZER_PIN, 2000, 100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing message");
    }
  });

  server.begin();
  Serial.println("HTTP Server Started");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Morse Code:");
  Serial.println("System Ready");
}

void loop() {
  server.handleClient();
  handleMorseInput();
}

void connectToWiFi() {
  Serial.println("\nAttempting WiFi connection...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(2000);
  } else {
    Serial.println("\nWiFi Connection Failed!");
    lcd.clear();
    lcd.print("WiFi Failed!");
    while (true) delay(1000);
  }
}

void handleMorseInput() {
  static unsigned long lastDebounceTime = 0;
  const int debounceDelay = 300;

  if (digitalRead(dashButton) == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    morseCode += "-";
    Serial.println("Added: -");
    updateDisplay();
    lastInputTime = millis();
    lastDebounceTime = millis();
  }

  if (digitalRead(dotButton) == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    morseCode += ".";
    Serial.println("Added: .");
    updateDisplay();
    lastInputTime = millis();
    lastDebounceTime = millis();
  }
  if (digitalRead(clearButton) == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    clearLCD();
  }

  if (millis() - lastInputTime > 1000 && morseCode.length() > 0) {
    Serial.print("Decoding: ");
    Serial.println(morseCode);
    decodeMorse();
    morseCode = "";
  }
}
void clearLCD() {
  morseCode = "";
  decodedMessage = "";
  webMessage = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Morse Code:");

  delay(500);
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(morseCode);
  Serial.print("Current Morse: ");
  Serial.println(morseCode);
}

void decodeMorse() {
  for (int i = 0; i < 36; i++) {
    if (morseCode == morseAlphabet[i]) {
      char decodedChar = asciiChars[i];
      Serial.print("Decoded '");
      Serial.print(morseCode);
      Serial.print("' as: ");
      Serial.println(decodedChar);

      lcd.setCursor(0, 1);
      lcd.print(" -> ");
      lcd.print(decodedChar);

      decodedMessage += decodedChar;

      tone(BUZZER_PIN, 2000, 100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      return;
    }
  }

  Serial.print("Unknown Morse: ");
  Serial.println(morseCode);
  lcd.setCursor(0, 1);
  lcd.print(" -> ?");
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Tactile-Digital Bridge</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="2">
  <style>
    body {
      font-family: 'Segoe UI', sans-serif;
      background: linear-gradient(to right, #1e1e2f, #2c2f4a);
      color: #f5f5f5;
      margin: 0;
      padding: 0;
    }
    .header {
      background: #12121c;
      padding: 20px;
      text-align: center;
      border-bottom: 2px solid #00bcd4;
    }
    .header h1 {
      margin: 0;
      font-size: 24px;
      color: #00bcd4;
    }
    .device-name {
      font-size: 14px;
      color: #aaa;
      margin-top: 5px;
    }
    .container {
      max-width: 600px;
      margin: 20px auto;
      background: #2b2e48;
      border-radius: 10px;
      padding: 20px;
      box-shadow: 0 0 10px #00000050;
    }
    .section {
      margin-bottom: 20px;
    }
    .label {
      font-size: 13px;
      color: #bbb;
      margin-bottom: 5px;
    }
    .about-box {
      background: #202235;
      padding: 15px;
      border-radius: 8px;
      font-size: 14px;
      line-height: 1.6;
      color: #ddd;
    }
    .chatbox {
      background: #191c2b;
      padding: 10px 15px;
      border-radius: 6px;
      min-height: 40px;
      font-size: 16px;
      word-wrap: break-word;
    }
    input, button {
      width: 100%;
      padding: 10px;
      margin-top: 10px;
      font-size: 14px;
      border: none;
      border-radius: 5px;
    }
    button {
      background: #00bcd4;
      color: #fff;
      cursor: pointer;
    }
    .footer {
      text-align: center;
      margin-top: 25px;
      font-size: 13px;
      color: #888;
    }
  </style>
</head>
<body>
  <div class="header">
    <h1>🟢 Tactile-Digital Bridge</h1>
    <div class="device-name">Morse Messaging System</div>
  </div>

  <div class="container">
    <div class="section">
      <div class="label">ℹ️ About Tactile-Digital Bridge:</div>
      <div class="about-box">
        <ul>
          <li>⚡ A real-time Morse Code communication system built on the powerful ESP32 platform.</li>
          <li>🖲️ Accepts tactile Morse input through intuitive dot and dash buttons.</li>
          <li>🔍 Instantly decodes Morse code into readable text with high accuracy.</li>
          <li>📟 Displays messages on an integrated 16x2 I2C LCD screen for quick feedback.</li>
          <li>🌐 Hosts a lightweight, embedded web server for wireless interaction over Wi-Fi.</li>
          <li>🔄 Enables seamless two-way messaging between the ESP32 device and any web browser.</li>
          <li>🔔 Provides tactile (buzzer) and visual (LED) alerts for decoded or received messages.</li>
          <li>🚀 Designed for portability, real-time operation, and full independence from external servers.</li>
        </ul>
        <hr style="border: 0; border-top: 1px solid #555; margin: 10px 0;">
        <p style="font-size: 13px; color: #bbb; text-align: center;">
          Made by <strong>Parth Khare (22102011)</strong>
        </p>
      </div>
    </div>

    <div class="section">
      <div class="label">📨 Decoded Message (ESP32 → PC):</div>
      <div class="chatbox">)rawliteral" + decodedMessage + R"rawliteral(</div>
    </div>

    <div class="section">
      <div class="label">💬 PC → ESP32 (Send Message):</div>
      <input type="text" id="msgInput" placeholder="Type your message...">
      <button onclick="sendMessage()">Send</button>
    </div>

    <div class="footer">
      Device IP: )rawliteral" + WiFi.localIP().toString() + R"rawliteral( <br>
      Powered by ESP32 | Morse Chat v1.0
    </div>
  </div>

<script>
function sendMessage() {
  let msg = document.getElementById('msgInput').value;
  if (msg.trim() === "") return;
  let xhr = new XMLHttpRequest();
  xhr.open("POST", "/send", true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  xhr.send("message=" + encodeURIComponent(msg));
  document.getElementById('msgInput').value = "";
}
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}
