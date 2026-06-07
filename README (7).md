# Bluetooth Home Automation — Arduino

## Project Overview
This project implements a **Bluetooth-controlled Home Automation system** using an Arduino UNO and HC-05 Bluetooth module. Three household devices — a Lamp, a Motor (fan/pump), and an LED — are individually controlled via relay modules driven by 2N2222 NPN transistors. Commands are sent wirelessly from a smartphone Bluetooth terminal app.

---

## Features
- Wireless control via HC-05 Bluetooth (up to ~10 m range)
- 3 independent relay channels: Lamp, Motor, LED
- Individual ON/OFF + Toggle commands per device
- Master ALL ON / ALL OFF commands
- Real-time status query via Serial/Bluetooth
- Active-HIGH relay design using 2N2222 NPN transistor drivers
- Simulated in Proteus Design Suite

---

## Hardware Components
| Component | Specification | Qty |
|---|---|---|
| Microcontroller | Arduino UNO (ATmega328P) | 1 |
| Bluetooth Module | HC-05 | 1 |
| Relay Module | 5V Active HIGH | 3 |
| NPN Transistor | 2N2222 | 3 |
| Lamp | AC/DC bulb (L1) | 1 |
| Motor | DC Motor / Fan | 1 |
| LED | Yellow LED (D5) | 1 |
| Resistor | 1kΩ base resistor | 3 |
| Power Supply | 5V DC + AC mains | 1 |

---

## Pin Configuration
| Arduino Pin | Connected To | Function |
|---|---|---|
| D0 (RX) | HC-05 TX | Bluetooth receive |
| D1 (TX) | HC-05 RX | Bluetooth transmit |
| D13 | Relay 1 (Lamp) | Lamp control |
| D12 | Relay 2 (Motor) | Motor control |
| D7 | Relay 3 (LED) | LED control |
| 5V | HC-05 VCC, Relay VCC | Power |
| GND | All GND | Common ground |

---

## Bluetooth Commands
| Command | Action |
|---|---|
| `1` | Lamp ON |
| `0` | Lamp OFF |
| `T` | Lamp Toggle |
| `2` | Motor ON |
| `3` | Motor OFF |
| `U` | Motor Toggle |
| `4` | LED ON |
| `5` | LED OFF |
| `V` | LED Toggle |
| `A` | ALL Devices ON |
| `X` | ALL Devices OFF |
| `S` | Status of all devices |

---

## How to Use
1. Upload `D3_Home_Automation.ino` to Arduino UNO
2. Pair HC-05 with your smartphone (PIN: `1234` or `0000`)
3. Open a Bluetooth terminal app (e.g., "Serial Bluetooth Terminal")
4. Connect to HC-05 device
5. Send single-character commands from the table above
6. Monitor responses in the Bluetooth terminal

---

## Relay Driver Circuit (per channel)
```
Arduino Pin (D13/D12/D7)
        |
       1kΩ
        |
     2N2222 Base
     2N2222 Collector → Relay coil (+)
     2N2222 Emitter   → GND
     Relay coil (-)   → 5V
     Flyback diode across relay coil
```

---

## Libraries Required
No external libraries — uses only built-in `Serial` (Hardware UART on pins 0/1).

> ⚠️ **Note:** Disconnect HC-05 from pins 0/1 before uploading sketch, as it shares the hardware UART.

---

## Simulation
Simulated in **Proteus** using HC-05 virtual Bluetooth component and virtual relay models.

---

## Author
Engineering Project — IoT / Embedded Systems / Home Automation

## License
For educational use only.
