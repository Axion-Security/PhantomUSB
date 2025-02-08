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
constexpr uint8_t TX_PIN = 16;
constexpr uint8_t RX_PIN = 17;
constexpr uint16_t CH9329_BAUDRATE = 9600;
constexpr uint32_t BAUDRATE = 115200;

#endif //MAIN_H
