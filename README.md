# IoT GSM-Enabled Scrolling LED Display

### Arduino UNO | SIM900D GSM | MAX7219 × 8 | MD_MAX72XX Library

---

## Project Overview

This project implements a **GSM-enabled Scrolling LED Display** that receives messages via **SMS** and displays them as scrolling text on an **8-module MAX7219 daisy-chained 8×8 LED matrix**. An **Arduino UNO (ATmega328P)** acts as the central controller, communicating with the **SIM900D GSM module** via **SoftwareSerial** (AT commands) and driving the LED matrix through **SPI** using the **MD_MAX72XX** library.

When a new SMS is received, the SIM900D sends a `+CMT:` notification to the Arduino over UART. The firmware parses the SMS text and updates the scrolling message buffer in real time.

Fully designed and simulated in **Proteus 8 Professional**.

---

## Hardware Components

| Component            | Specification            | Qty | Function                      |
| -------------------- | ------------------------ | --- | ----------------------------- |
| Microcontroller      | Arduino UNO (ATmega328P) | 1   | Central MCU                   |
| GSM Module           | SIM900D                  | 1   | SMS Reception via AT Commands |
| LED Driver IC        | MAX7219                  | 8   | 8×8 LED Matrix Driver         |
| LED Matrix           | 8×8 Common Cathode       | 8   | Scrolling Text Display        |
| SIM Card             | Any GSM operator         | 1   | Mobile number for SMS         |
| Power Supply         | 5V USB + 12V/2A Adapter  | 1   | Arduino + SIM900D power       |
| Decoupling Capacitor | 100µF / 0.1µF            | 2   | GSM power filtering           |

---

## Pin Configuration

| Arduino Pin   | Connected To  | Function            |
| ------------- | ------------- | ------------------- |
| Pin 7 (SW-RX) | SIM900D TX    | GSM Serial Receive  |
| Pin 8 (SW-TX) | SIM900D RX    | GSM Serial Transmit |
| Pin 11 (DATA) | MAX7219 DIN   | SPI Data            |
| Pin 13 (CLK)  | MAX7219 CLK   | SPI Clock           |
| Pin 10 (CS)   | MAX7219 LOAD  | SPI Chip Select     |
| 5V / GND      | Arduino Power | System Power        |
| 12V / 2A      | SIM900D VCC   | GSM Module Power    |

---

## Project Structure

```text
iot_based_gsm_enbled_scrolling_led_display/
├── B3_GSM_Scrolling_LED_display/
│   ├── B3_GSM_Scrolling_LED_display.ino
│   └── build/
│       ├── arduino.avr.nano/
│       │   └── B3_GSM_Scrolling_LED_display.ino.hex
│       └── arduino.avr.uno/
│           └── B3_GSM_Scrolling_LED_display.ino.hex
├── iot_based_gsm_enbled_scrolling_led_display.pdsprj
└── Backup Of iot_based_gsm_enbled_scrolling_led_display.pdsbak
```

---

## AT Command Flow

```text
Arduino Powers Up
       ↓
gsmInit() sends:
  AT
  AT+CMGF=1
  AT+CNMI=1,2,0,0,0
       ↓
Phone sends SMS to SIM900D number
       ↓
SIM900D pushes → +CMT: "sender","timestamp"
                  <SMS text on next line>
       ↓
parseGSM() detects "+CMT:"
  → extracts last line (actual SMS text)
  → copies to newMessage[]
  → sets newMsgAvailable = true
       ↓
loop() detects newMsgAvailable
  → strcpy(curMessage, newMessage)
  → display scrolls new text
```

---

## Scroll Engine

```cpp
// Callback registered with MD_MAX72XX
uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t) {
  // Returns column data character by character from curMessage[]
  // State machine: fetch char → push columns → add spacing
}

void scrollText() {
  static uint32_t prevTime = 0;
  if (millis() - prevTime >= SCROLL_DELAY) {
    mx.transform(MD_MAX72XX::TSR);
    prevTime = millis();
  }
}
```

---

## Key Firmware Parameters

```cpp
#define MAX_DEVICES   8
#define CLK_PIN      13
#define DATA_PIN     11
#define CS_PIN       10
#define GSM_RX_PIN    7
#define GSM_TX_PIN    8
#define SCROLL_DELAY  150
#define CHAR_SPACING  1
#define BUF_SIZE      100
```

---

## Libraries Used

| Library          | Purpose                                      |
| ---------------- | -------------------------------------------- |
| `MD_MAX72XX`     | MAX7219 LED matrix driver with scroll engine |
| `SoftwareSerial` | UART communication with SIM900D on Pin 7/8   |

---

## How to Run

### Simulation (Proteus 8)

1. Open `iot_based_gsm_enbled_scrolling_led_display.pdsprj`
2. Load HEX from `build/arduino.avr.uno/` into Arduino component
3. Configure SIM900D with COMPIM virtual serial port
4. Send `+CMT:` formatted string via Virtual Terminal to simulate SMS
5. Watch scrolling text update on LED matrix

### Hardware

1. Install library: Arduino IDE → `MD_MAX72XX` by MajicDesigns
2. Upload `B3_GSM_Scrolling_LED_display.ino` to Arduino UNO
3. Insert active SIM card into SIM900D
4. Power SIM900D separately with 12V/2A adapter
5. Send SMS to SIM900D's number and the text will scroll on the LED matrix

---

## Features

* Real-time SMS-to-display update via GSM network
* 8-module MAX7219 daisy chain (512 LEDs total)
* Smooth scrolling via MD_MAX72XX TSR transform (150ms/step)
* AT Command based SMS parsing (`+CMT:` handler)
* Default startup message: "Waiting for SMS..."
* SoftwareSerial on Pin 7/8 (hardware Serial free for debug)
* Dual build targets: Arduino Nano and Arduino UNO
* Proteus 8 simulation with COMPIM virtual port

---

## Author

**Shreyas**
Electronics & Embedded Systems Engineer
Maharashtra, India

---

## License

Educational and portfolio use only.
