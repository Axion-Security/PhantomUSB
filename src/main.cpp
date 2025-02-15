#include "main.h"

// ESP32   | CH9329
// GND     | GND
// VIN     | 5V
// GPIO 17 | TX
// GPIO 16 | RX

#define W_LED_PIN 19 // WRITE Indicator
#define S_LED_PIN 18 // STATUS Indicator
#define MAX_INIT_ATTEMPTS 10

WebServer server(80);
auto ssid = "PhantomUSB";
auto password = "12345678910";

HardwareSerial ch9329Serial(1);
CH9329_Keyboard_ Keyboard;
bool ch9329Initialized = false;

void controlLED(const bool state, const uint8_t LED_PIN) {
    digitalWrite(LED_PIN, state);
}

bool initializeCH9329() {
    ch9329Serial.begin(CH9329_BAUDRATE, SERIAL_8N1, TX_PIN, RX_PIN);
    delay(100);

    Keyboard.begin(ch9329Serial, KeyboardLayout_en_US);
    ch9329Serial.write(0x00);
    Serial.println("Initializing CH9329...");

    controlLED(false, S_LED_PIN);

    int attempts = 0;
    while (ch9329Serial.available() == 0 && attempts < MAX_INIT_ATTEMPTS) {
        Serial.print("Waiting for response (Attempt ");
        Serial.print(attempts + 1);
        Serial.println("/10)");
        delay(200);
        ch9329Serial.write(0x00); // Resend handshake
        attempts++;
    }

    controlLED(true, S_LED_PIN);

    if (attempts >= MAX_INIT_ATTEMPTS) {
        Serial.println("CH9329 initialization failed!");
        return false;
    }

    while (ch9329Serial.available() > 0) ch9329Serial.read();

    Serial.println("CH9329 initialized successfully");
    return true;
}

void setup() {
    pinMode(W_LED_PIN, OUTPUT);
    pinMode(S_LED_PIN, OUTPUT);
    Serial.begin(BAUDRATE);
    while (!Serial) {}

    ch9329Initialized = initializeCH9329();

    WiFi.softAP(ssid, password);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [] {
        server.send(200, "text/html", webPage);
    });

    server.on("/sendText", HTTP_POST, [] {
        if (!ch9329Initialized) {
            server.send(500, "text/plain", "CH9329 not initialized");
            return;
        }

        if (server.hasArg("text")) {
            const String inputText = server.arg("text");
            controlLED(true, W_LED_PIN);
            transpiler::transpile(inputText.c_str());
            controlLED(false, W_LED_PIN);
            server.send(200, "text/plain", "Text sent successfully");
        } else {
            server.send(400, "text/plain", "No text received");
        }
    });

    server.begin();
}

void loop() {
    server.handleClient();
}