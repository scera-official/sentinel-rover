# SentinelRover — Project Overview

## 1. Introduction

SentinelRover is a tracked, multi-purpose robotic monitoring prototype. It combines embedded controllers, environmental sensors, GPS, wireless communication and a remote display to collect and present selected information about the rover's surroundings.

The system is divided into a mobile monitoring unit and a remote receiver unit. An Arduino Nano operates the rover-side sensing and transmission firmware. An Arduino Uno receives the wireless data and presents selected information to an operator.

## 2. Problem addressed

Monitoring remote or difficult-to-access areas may require people to approach the location or observe several separate instruments. SentinelRover explores how a mobile platform can collect multiple types of readings and relay them to a separate monitoring point.

This is an educational prototype and should not be relied upon as a certified fire alarm, industrial safety system, or emergency-response platform.

## 3. Objectives

- Build a tracked mobile monitoring platform.
- Sense flame-related signals, motion and nearby-object distance.
- Obtain GPS position and movement information.
- Transmit sensor and GPS data wirelessly.
- Present key GPS parameters on a compact LCD.
- Provide audible notifications for configured flame and proximity conditions.
- Demonstrate integration of robotics and embedded electronics.

## 4. Main hardware

| Subsystem | Component | Purpose |
|---|---|---|
| Rover controller | Arduino Nano | Reads sensors/GPS and sends packets |
| Receiver controller | Arduino Uno | Receives packets and manages LCD/alerts |
| Radio | 2 × nRF24L01 | Wireless data link |
| Positioning | NEO-6M GPS | Coordinates, speed, altitude, satellites and fix |
| Fire-related sensing | Digital flame sensor | Flame-related digital signal |
| Motion sensing | PIR sensor | Motion state |
| Proximity | Ultrasonic sensor | Distance measurement |
| Display | 16×2 I2C LCD | Remote information display |
| Input | Tactile push button | LCD page navigation |
| Audio | Buzzer | Configured warning notifications |
| Mobility | Tracked chassis | Mobile platform |

## 5. Data flow

1. Sensors and GPS provide readings to the Arduino Nano.
2. The transmitter creates a `SensorPacket`.
3. The Nano sends the packet through its nRF24L01 module.
4. The receiver nRF24L01 passes the packet to the Arduino Uno.
5. The Uno updates the LCD and checks the configured alert conditions.
6. The operator uses the button to change the displayed GPS page.

## 6. Six GPS parameters

The receiver organises the six GPS fields into three pages:

1. Latitude and longitude.
2. Speed and altitude.
3. Satellite count and GPS fix status.

A GPS fix is required for valid coordinates. Until a fix is available, the transmitter sends zero coordinates and a `gpsFix` value of `0`.

## 7. Current firmware boundaries

The supplied firmware implements sensor acquisition, GPS parsing, radio transmission/reception, LCD page navigation, signal timeout handling and flame/proximity buzzer alerts.

The supplied two sketches do not implement motor-driver control, autonomous navigation, camera operation, map visualisation, mobile-app connectivity or persistent data logging. These are possible future developments, not current firmware features.
