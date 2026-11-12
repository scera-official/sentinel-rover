/*
  SentinelRover - TRANSMITTER (Arduino Nano on the rover)

  Sensors:
    Flame sensor  -> D2 (digital, LOW = flame detected on most modules)
    PIR sensor    -> D3 (HIGH = motion)
    Ultrasonic    -> TRIG D4, ECHO D5
    NEO-6M GPS    -> SoftwareSerial RX D6, TX D7 (@9600 baud)
  Radio:
    nRF24L01      -> CE D9, CSN D10 (SPI: D11 MOSI, D12 MISO, D13 SCK)

  Libraries needed (install via Library Manager):
    TinyGPSPlus, RF24
*/

#include <SPI.h>
#include <RF24.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

// ---------- Pin definitions ----------
const uint8_t FLAME_PIN = 2;
const uint8_t PIR_PIN   = 3;
const uint8_t TRIG_PIN  = 4;
const uint8_t ECHO_PIN  = 5;
const uint8_t GPS_RX    = 6;   // Nano receives from GPS TX
const uint8_t GPS_TX    = 7;   // Nano transmits to GPS RX
const uint8_t NRF_CE    = 9;
const uint8_t NRF_CSN   = 10;

// ---------- Objects ----------
RF24 radio(NRF_CE, NRF_CSN);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;

// Same address on both units
const byte address[6] = "SNTNL";

// ---------- Data packet ----------
struct SensorPacket {
  float    latitude;
  float    longitude;
  float    speedKmph;
  float    altitudeM;
  uint8_t  satellites;
  uint8_t  gpsFix;       // 1 = valid fix, 0 = no fix
  uint8_t  flameDetected;
  uint8_t  motionDetected;
  uint16_t distanceCm;
};

// ---------- Helpers ----------
uint16_t readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 30 ms timeout (~5 m max range)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;            // no echo
  return (uint16_t)(duration / 58);         // microseconds -> cm
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(FLAME_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!radio.begin()) {
    Serial.println(F("nRF24L01 not responding. Check wiring."));
    while (1);
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println(F("SentinelRover transmitter ready."));
}

void loop() {
  // Feed GPS parser continuously
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  SensorPacket pkt;

  pkt.gpsFix = gps.location.isValid() ? 1 : 0;
  pkt.latitude  = pkt.gpsFix ? gps.location.lat() : 0.0;
  pkt.longitude = pkt.gpsFix ? gps.location.lng() : 0.0;
  pkt.speedKmph = gps.speed.isValid()     ? gps.speed.kmph()   : 0.0;
  pkt.altitudeM = gps.altitude.isValid()  ? gps.altitude.meters() : 0.0;
  pkt.satellites = gps.satellites.isValid() ? gps.satellites.value() : 0;

  pkt.flameDetected  = (digitalRead(FLAME_PIN) == LOW) ? 1 : 0;
  pkt.motionDetected = digitalRead(PIR_PIN);
  pkt.distanceCm     = readDistanceCm();

  bool ok = radio.write(&pkt, sizeof(pkt));
  Serial.print(ok ? F("Sent: ") : F("Send failed: "));
  Serial.print(pkt.latitude, 6); Serial.print(F(", "));
  Serial.print(pkt.longitude, 6); Serial.print(F(" | Spd "));
  Serial.print(pkt.speedKmph, 1); Serial.print(F(" | Alt "));
  Serial.print(pkt.altitudeM, 1); Serial.print(F(" | Sats "));
  Serial.print(pkt.satellites); Serial.print(F(" | Fix "));
  Serial.print(pkt.gpsFix); Serial.print(F(" | Flame "));
  Serial.print(pkt.flameDetected); Serial.print(F(" | PIR "));
  Serial.print(pkt.motionDetected); Serial.print(F(" | Dist "));
  Serial.println(pkt.distanceCm);

  delay(500);
}
