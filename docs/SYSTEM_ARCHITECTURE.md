# System Architecture

## 1. Two-unit design

SentinelRover separates sensing and data collection from operator-side presentation.

### Mobile monitoring unit

**Controller:** Arduino Nano

Inputs:
- Flame sensor digital state
- PIR motion state
- Ultrasonic distance
- NEO-6M GPS serial data

Output:
- Structured sensor packet over nRF24L01

### Remote receiver unit

**Controller:** Arduino Uno

Inputs:
- Wireless `SensorPacket`
- Push-button input

Outputs:
- 16×2 I2C LCD
- Buzzer notification
- Serial diagnostic output

## 2. Wireless packet

Both sketches define the same structure:

| Field | Type | Meaning |
|---|---|---|
| `latitude` | `float` | Latitude in decimal degrees |
| `longitude` | `float` | Longitude in decimal degrees |
| `speedKmph` | `float` | GPS speed in km/h |
| `altitudeM` | `float` | GPS altitude in metres |
| `satellites` | `uint8_t` | Satellite count |
| `gpsFix` | `uint8_t` | `1` when location is valid, otherwise `0` |
| `flameDetected` | `uint8_t` | Flame sensor state |
| `motionDetected` | `uint8_t` | PIR state |
| `distanceCm` | `uint16_t` | Ultrasonic distance in centimetres |

**Compatibility requirement:** Keep field order, types, and compiler/platform assumptions aligned in both sketches. If the packet changes, update both ends together.

## 3. Radio configuration in the supplied firmware

| Setting | Value |
|---|---|
| Pipe address | `SNTNL` |
| Data rate | `RF24_250KBPS` |
| Power level | `RF24_PA_LOW` |
| Transmitter mode | Writing pipe |
| Receiver mode | Reading pipe 1 |
| Transmit interval | Approximately 500 ms plus processing time |

## 4. Receiver behaviour

- Displays a waiting message before receiving data.
- Refreshes the current LCD page after receiving a packet.
- Cycles through three GPS pages when the button is pressed.
- Displays `Signal lost` when no packet has arrived within the configured 3-second timeout.
- Generates a buzzer tone for flame detection or a measured distance below the configured 20 cm threshold.

## 5. Current limitations visible in code

- PIR state is transmitted, but no PIR-specific alert is implemented in the receiver.
- The receiver's alert display writes a warning on LCD line 2, which may temporarily replace the second line of the current GPS page.
- The supplied sketches do not include motor control.
- The current packet has no explicit sequence number or checksum at application level.
- Radio write success is printed on the transmitter serial monitor; no application-level acknowledgement is shown in the receiver sketch.
