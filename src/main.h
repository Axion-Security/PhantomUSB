#ifndef MAIN_H
#define MAIN_H

using namespace std;

// Keyboard
#include <Arduino.h>
#include "CH9329_Keyboard.h"
#include <SoftwareSerial.h>

// WiFi
#include <WiFi.h>
#include <WebServer.h>

// Transpiler
#include "helper/transpiler.h"

// Constants
constexpr uint8_t TX_PIN = 17;
constexpr uint8_t RX_PIN = 16;
constexpr uint16_t CH9329_BAUDRATE = 9600;
constexpr uint32_t BAUDRATE = 115200;

// Webpage
const String webPage = R"rawliteral(
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

#endif //MAIN_H
