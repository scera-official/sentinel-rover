# Troubleshooting

## Receiver displays “NRF24 not found”

- Check CE and CSN wiring.
- Check SPI connections (Uno/Nano: D11 MOSI, D12 MISO, D13 SCK).
- Confirm the radio has a stable 3.3 V supply.
- Confirm ground is connected.
- Inspect module orientation and solder joints.

## Receiver stays on “Waiting for rover signal…”

- Confirm both boards are powered and running their respective sketches.
- Confirm both radios use the same address, data rate and compatible settings.
- Check transmitter Serial Monitor for `Send failed`.
- Check radio power and wiring on both units.
- Keep the two radios within a reasonable test distance and away from strong interference.

## Receiver displays “Signal lost”

The receiver has not received a packet within `SIGNAL_TIMEOUT_MS` (3 seconds in the supplied sketch). Check transmitter operation, radio power, wiring and communication settings.

## GPS coordinates remain zero or GPS fix says NO

- Move the GPS antenna/module to a location with a clearer view of the sky.
- Allow time for satellite acquisition.
- Confirm GPS TX connects to Nano D6 and GPS RX connects to Nano D7.
- Confirm the GPS serial baud rate is 9600.
- Check that the module is outputting NMEA data.

## LCD is blank or unreadable

- Check LCD power and ground.
- Confirm SDA → A4 and SCL → A5 on the Uno.
- Confirm the I2C address. The sketch defaults to `0x27`.
- Adjust the LCD contrast potentiometer if the backpack has one.

## Button does not change pages

- Confirm the button is connected between D2 and GND.
- The code uses `INPUT_PULLUP`, so the pressed state is LOW.
- Check the button and its connections.

## Buzzer does not sound

- Check buzzer polarity/type and the D3 connection.
- Confirm the alert condition is actually being received.
- The supplied code triggers alerts for flame detection or distance below 20 cm; it does not currently trigger a dedicated PIR alert.

## Distance reading is unusual

- Check TRIG D4 and ECHO D5.
- Ensure the sensor is aimed at a surface that can reflect ultrasound.
- The code returns `999` when no echo is received within its timeout.

## Upload errors

- Select the correct board and COM port.
- Close Serial Monitor before uploading.
- For Nano clones, try the appropriate processor/old bootloader option if required by the board.
- Use a known-good USB data cable.
