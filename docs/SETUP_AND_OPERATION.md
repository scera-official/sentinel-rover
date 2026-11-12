# Setup and Operation

## 1. Prepare Arduino IDE

1. Install Arduino IDE.
2. Connect the Arduino Nano and install/select the appropriate board package.
3. Connect the Arduino Uno and select its board package.
4. Open Library Manager and install:
   - `RF24`
   - `TinyGPSPlus`
   - `LiquidCrystal_I2C`
5. Select the correct board and serial port before uploading each sketch.

## 2. Upload the rover transmitter

1. Open `firmware/transmitter/SentinelRover_Transmitter.ino`.
2. Select **Arduino Nano**.
3. Select the correct processor/bootloader option for your Nano clone, if applicable.
4. Select the Nano's COM port.
5. Upload the sketch.
6. Open Serial Monitor at **9600 baud** to inspect transmitter status and readings.

## 3. Upload the receiver

1. Open `firmware/receiver/SentinelRover_Receiver.ino`.
2. Select **Arduino Uno** and its COM port.
3. Upload the sketch.
4. Power the LCD and receiver radio.
5. The LCD should show the startup message, followed by a waiting message until a packet arrives.

## 4. Normal operation

1. Power the receiver and rover.
2. Ensure both radio modules are powered from stable 3.3 V supplies.
3. Allow the GPS module to acquire satellites. Initial acquisition may take time and depends on sky visibility.
4. Confirm that the receiver begins displaying data.
5. Press the receiver button to cycle through the three GPS pages.

## 5. Configuration values

The following values are defined near the top of the receiver sketch:

```cpp
const uint16_t ALERT_DISTANCE_CM = 20;
const unsigned long DEBOUNCE_MS = 200;
const unsigned long SIGNAL_TIMEOUT_MS = 3000;
```

Change these only after understanding their effect. Keep transmitter and receiver packet definitions consistent.

The LCD address is configured as:

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

If your LCD backpack uses a different address, update this value.

## 6. Expected display pages

| Page | Information |
|---|---|
| 0 | Latitude and longitude |
| 1 | Speed and altitude |
| 2 | Satellite count and GPS fix status |

The push button is configured with `INPUT_PULLUP`; pressing it connects D2 to GND.
