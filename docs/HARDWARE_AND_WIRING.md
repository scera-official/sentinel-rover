# Hardware and Wiring Reference

This document records the pin assignments stated in the supplied Arduino sketches. Verify your exact module pin labels and board variants before wiring.

## 1. Rover transmitter — Arduino Nano

| Module / signal | Nano pin | Notes |
|---|---:|---|
| Flame sensor digital output | D2 | Code treats `LOW` as flame detected |
| PIR sensor output | D3 | `HIGH` represents motion |
| Ultrasonic TRIG | D4 | Trigger output |
| Ultrasonic ECHO | D5 | Echo input |
| GPS TX → Nano RX | D6 | `SoftwareSerial` receive pin |
| GPS RX ← Nano TX | D7 | `SoftwareSerial` transmit pin |
| nRF24L01 CE | D9 | Radio control |
| nRF24L01 CSN | D10 | Radio chip select |
| nRF24L01 MOSI | D11 | Hardware SPI |
| nRF24L01 MISO | D12 | Hardware SPI |
| nRF24L01 SCK | D13 | Hardware SPI |
| nRF24L01 VCC | 3.3 V | **Never use 5 V** |
| nRF24L01 GND | GND | Common ground |

### GPS serial connection

- GPS module TX connects to Nano D6.
- GPS module RX connects to Nano D7.
- Firmware uses 9600 baud.

The GPS module's supply-voltage requirements depend on the breakout board. Check its documentation rather than assuming every NEO-6M board has the same regulator or logic-level arrangement.

## 2. Remote receiver — Arduino Uno

| Module / signal | Uno pin | Notes |
|---|---:|---|
| nRF24L01 CE | D9 | Radio control |
| nRF24L01 CSN | D10 | Radio chip select |
| nRF24L01 MOSI | D11 | Hardware SPI |
| nRF24L01 MISO | D12 | Hardware SPI |
| nRF24L01 SCK | D13 | Hardware SPI |
| LCD SDA | A4 | I2C data |
| LCD SCL | A5 | I2C clock |
| Push button | D2 | Other button terminal to GND; `INPUT_PULLUP` |
| Buzzer | D3 | Tone output |
| LCD I2C address | `0x27` | Change in code if your backpack uses another address |
| nRF24L01 VCC | 3.3 V | **Never use 5 V** |
| nRF24L01 GND | GND | Common ground |

## 3. Power and radio notes

- nRF24L01 modules require a stable 3.3 V supply. A weak supply can cause intermittent communication.
- Do not connect the radio VCC pin to the Arduino 5 V rail.
- Keep radio wiring short and ensure the module has a reliable ground.
- The transmitter and receiver must use compatible radio settings and the same address.
- Power the motor system and logic system appropriately; avoid allowing motor noise or voltage dips to reset the microcontrollers.
- Do not connect or disconnect modules while the system is powered.

## 4. Chassis and motor wiring

The supplied transmitter and receiver sketches do not include motor-driver pin assignments or motor-control code. Therefore, this repository does not specify motor-driver wiring. Use the documentation for the actual motor driver and chassis hardware, and keep its power requirements separate from the microcontroller logic supply where appropriate.

## 5. Before powering on

- [ ] Confirm Nano and Uno board pin labels.
- [ ] Confirm nRF24L01 VCC is 3.3 V, not 5 V.
- [ ] Confirm common ground within each unit.
- [ ] Confirm GPS TX/RX are crossed as documented.
- [ ] Confirm LCD I2C address.
- [ ] Confirm the button connects D2 to GND when pressed.
- [ ] Check for short circuits and loose connections.
