#include "main.h"

// ESP32   | CH9329
// GND     | GND
// VIN     | 5VHello, World!Hello, World!
// GPIO 16 | TX
// GPIO 17 | RX

WebServer server(80);
auto ssid = "PhantomUSB";
auto password = "12345678910";

HardwareSerial ch9329Serial(1);
CH9329_Keyboard_ Keyboard;

void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial) {}

    WiFi.softAP(ssid, password);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, []() {
        const String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Phantom - USB Script</title>
    <style>
        :root {
            --primary: #007bff;
            --hover: #0056b3;
            --background: #1a1a1a;
            --container: #2d2d2d;
        }
        body {
            font-family: 'Segoe UI', sans-serif;
            background: var(--background);
            color: #fff;
            margin: 0;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        .container {
            background: var(--container);
            border-radius: 12px;
            padding: 2rem;
            width: 90%;
            max-width: 500px;
            box-shadow: 0 8px 16px rgba(0,0,0,0.25);
        }
        h1 {
            text-align: center;
            margin: 0 0 1.5rem;
            font-size: 1.8rem;
            color: var(--primary);
        }
        #messageBox {
            width: 100%;
            height: 150px;
            padding: 12px;
            border: 2px solid #404040;
            border-radius: 8px;
            background: #252525;
            color: #fff;
            font-size: 14px;
            resize: vertical;
            margin-bottom: 1rem;
        }
        #actionButton {
            width: 100%;
            padding: 12px;
            background: var(--primary);
            color: white;
            border: none;
            border-radius: 6px;
            font-weight: 600;
            cursor: pointer;
            transition: background 0.2s;
        }
        #actionButton:hover {
            background: var(--hover);
        }
        .status {
            text-align: center;
            margin-top: 1rem;
            font-size: 0.9rem;
            color: #00ff00;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🛸 Phantom Controller</h1>
        <textarea id="messageBox" placeholder="Enter your script here..."></textarea>
        <button id="actionButton">Execute Script</button>
        <div id="status" class="status"></div>
    </div>

    <script>
        const button = document.getElementById('actionButton');
        const textarea = document.getElementById('messageBox');
        const statusDiv = document.getElementById('status');
        let isExecuting = false;

        async function sendScript() {
            if (isExecuting) {
                // Add logic to stop execution if needed
                statusDiv.textContent = "⏹ Script stopped";
                statusDiv.style.color = "#ff9900";
                button.textContent = "Execute Script";
                isExecuting = false;
                return;
            }

            const script = textarea.value.trim();
            if (!script) {
                statusDiv.textContent = "Please enter a script!";
                statusDiv.style.color = "#ff4444";
                return;
            }

            isExecuting = true;
            button.textContent = "Stop";

            try {
                const response = await fetch('/sendText', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/x-www-form-urlencoded',
                    },
                    body: 'text=' + encodeURIComponent(script)
                });

                if (!response.ok) throw new Error('Server error');

                const result = await response.text();
                statusDiv.textContent = "✅ " + result;
                statusDiv.style.color = "#00ff00";

            } catch (error) {
                statusDiv.textContent = "Error: " + error.message;
                statusDiv.style.color = "#ff4444";
            }

            isExecuting = false;
            button.textContent = "Execute Script";
        }

        button.addEventListener('click', sendScript);
    </script>
</body>
</html>
)rawliteral";
        server.send(200, "text/html", html);
    });

    server.on("/sendText", HTTP_POST, []() {
        if (server.hasArg("text")) {
            const String inputText = server.arg("text");

            ch9329Serial.begin(CH9329_BAUDRATE, SERIAL_8N1, TX_PIN, RX_PIN);
            delay(100);
            Keyboard.begin(ch9329Serial, KeyboardLayout_en_US);
            ch9329Serial.write(0x00);
            Serial.println(F("Waiting for CH9329 response..."));

            while (ch9329Serial.available() <= 0) {
                Serial.println(F("Waiting..."));
                delay(200);
                ch9329Serial.write(0x00);
            }

            transpiler::transpile(inputText.c_str());

            server.send(200, "text/plain", "Script executed successfully!");
        } else {
            server.send(400, "text/plain", "No script received");
        }
    });

    server.begin();
}

void loop() {
    server.handleClient();
}