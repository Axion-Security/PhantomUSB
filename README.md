# PhantomUSB 🚀🔌

**Professional-Grade Keystroke Injection Tool for Security Testing**  
*Remote HID emulation platform for penetration testing & red team scenarios*

![img.png](img.png)

---

## 📱 Video Showcase  
[![PhantomUSB Demo](https://img.shields.io/badge/TikTok-Showcase-FF0050?logo=tiktok)](https://vm.tiktok.com/ZNd1U685v/)  
*See it in action! 60-second demo of PhantomUSB capabilities*

---

## ✨ Features

- 📡 **Remote Keystroke Injection** via WiFi AP
- 💡 **Dual LED Status System** (Operation Feedback)
- 📜 **Embedded Scripting Language** ([Documentation](https://learn.axionsec.pro/Phantom))
- ⚡ **Low-latency Communication** with CH9329
- 🛠️ **Plug & Play Setup** with ESP32
- 🔒 **Secure Connection** (WPA2 Protected)

---

## 🛠 Hardware Setup

### 🔌 Pinout Diagram

| ESP32       | CH9329/LEDs      |
|-------------|------------------|
| GND         | GND              |
| VIN (5V)    | 5V               |
| GPIO 17     | TX               |
| GPIO 16     | RX               |
| GPIO 19     | LED Write (+)    |
| GPIO 18     | LED Status (+)   |

**LED Behavior:**  
- 💚 **Status LED (GPIO 18):** Blinks during script execution
- 💙 **Write LED (GPIO 19):** Solid = CH9329 Active

---

## 📶 WiFi Configuration

```ini
SSID: "PhantomUSB"
Password: "12345678910"
```

*Connect to this network to access control interface*

---

## 📜 Scripting Language Documentation

**Full Documentation Portal:**  
[https://learn.axionsec.pro/Phantom](https://learn.axionsec.pro/Phantom)

<sub>![GitHub Repo](https://img.shields.io/badge/Source_Code-GitHub-181717?logo=github)</sub>

### 🔑 Core Features

**Basic Commands:**
- ⌨️ `write "text"` - Type text with keyboard emulation
- ⏎ `writeLn "text"` - Type text + press Enter
- ⏳ `delay "ms"` - Pause execution (milliseconds)

**Advanced Features:**
- 🖥️ `terminal "OS"` - Auto-open system terminal (Windows/Linux/Mac)
- 🔢 `functionKey "F1-F24"` - Press function keys
- 🧭 `arrowKey "direction"` - Navigate with arrow keys

### � Example: Windows Recon Script

```plaintext
terminal "windows"
delay "1000"
writeLn "systeminfo"
delay "5500"
writeLn "ipconfig /all"
```

**Execution Flow:**
1. 🪟 Opens Windows Terminal
2. ⏲️ 1s delay for loading
3. 📝 Starts `systeminfo` command
4. ⏳ 5.5s pause before execution
5. 📡 Triggers network configuration dump

## 🚀 Getting Started

1. **Hardware Setup**  
   🔄 Connect components according to pinout diagram

2. **Power On**  
   ⚡ Wait for Status LED to light up solid Blue

3. **Connect to WiFi**  
   📱 Join "PhantomUSB" network on your device

4. **Access Web Interface**  
   🌐 Navigate to `192.168.4.1` in your browser

5. **Execute Scripts**  
   📜 Paste your PhantomScript code and watch the Write LED blink (Green)!

---

## 🎥 TikTok Showcase

[![PhantomUSB Demo Video](https://img.shields.io/badge/Video_Demo-%40AxionSecurity-blue?logo=tiktok)](https://vm.tiktok.com/ZNd1U685v/)  
*See real-world usage examples and creative implementations*