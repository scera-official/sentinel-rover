/*
  SentinelRover - RECEIVER (Arduino Uno at the operator station)

  Hardware:
    nRF24L01  -> CE D9, CSN D10 (SPI: D11 MOSI, D12 MISO, D13 SCK)
    16x2 I2C LCD -> SDA A4, SCL A5 (address 0x27, change if needed)
    Push button -> D2 (to GND, uses INPUT_PULLUP)
    Buzzer      -> D3

  Libraries needed (install via Library Manager):
    RF24, LiquidCrystal_I2C

  Display pages (button cycles through them):
    Page 0: Latitude / Longitude
    Page 1: Speed / Altitude
    Page 2: Satellites / GPS Fix
    Alerts (flame, or object closer than ALERT_DISTANCE_CM) override
    the buzzer and show a warning on line 2 of the LCD.
*/

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pin definitions ----------
const uint8_t NRF_CE    = 9;
const uint8_t NRF_CSN   = 10;
const uint8_t BUTTON_PIN = 2;
const uint8_t BUZZER_PIN = 3;

// ---------- Settings ----------
const uint16_t ALERT_DISTANCE_CM = 20;     // buzz when object is closer than this
const unsigned long DEBOUNCE_MS  = 200;
const unsigned long SIGNAL_TIMEOUT_MS = 3000;

// ---------- Objects ----------
RF24 radio(NRF_CE, NRF_CSN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte address[6] = "SNTNL";

// ---------- Data packet (must match transmitter exactly) ----------
struct SensorPacket {
  float    latitude;
  float    longitude;
  float    speedKmph;
  float    altitudeM;
  uint8_t  satellites;
  uint8_t  gpsFix;
  uint8_t  flameDetected;
  uint8_t  motionDetected;
  uint16_t distanceCm;
};

SensorPacket pkt;
bool hasData = false;
unsigned long lastPacketTime = 0;

uint8_t page = 0;
const uint8_t PAGE_COUNT = 3;

bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;

// ---------- Helpers ----------
void beepAlert() {
  tone(BUZZER_PIN, 2000, 200);
}

void drawPage() {
  lcd.clear();
  if (!hasData) {
    lcd.setCursor(0, 0);
    lcd.print(F("Waiting for"));
    lcd.setCursor(0, 1);
    lcd.print(F("rover signal..."));
    return;
  }

  switch (page) {
    case 0:  // Lat / Lon
      lcd.setCursor(0, 0);
      lcd.print(F("Lat:"));
      lcd.print(pkt.gpsFix ? pkt.latitude : 0.0, 4);
      lcd.setCursor(0, 1);
      lcd.print(F("Lon:"));
      lcd.print(pkt.gpsFix ? pkt.longitude : 0.0, 4);
      break;

    case 1:  // Speed / Altitude
      lcd.setCursor(0, 0);
      lcd.print(F("Spd:"));
      lcd.print(pkt.speedKmph, 1);
      lcd.print(F(" km/h"));
      lcd.setCursor(0, 1);
      lcd.print(F("Alt:"));
      lcd.print(pkt.altitudeM, 1);
      lcd.print(F(" m"));
      break;

    case 2:  // Satellites / Fix
      lcd.setCursor(0, 0);
      lcd.print(F("Satellites: "));
      lcd.print(pkt.satellites);
      lcd.setCursor(0, 1);
      lcd.print(F("GPS Fix: "));
      lcd.print(pkt.gpsFix ? F("YES") : F("NO"));
      break;
  }
}

void checkAlerts() {
  bool alarm = false;
  if (pkt.flameDetected) {
    alarm = true;
    lcd.setCursor(0, 1);
    lcd.print(F("!! FLAME !!     "));
  } else if (pkt.distanceCm < ALERT_DISTANCE_CM) {
    alarm = true;
    lcd.setCursor(0, 1);
    lcd.print(F("!! OBJECT !!    "));
  }
  if (alarm) beepAlert();
}

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print(F("SentinelRover"));
  lcd.setCursor(0, 1);
  lcd.print(F("Receiver start"));

  if (!radio.begin()) {
    lcd.clear();
    lcd.print(F("NRF24 not found"));
    while (1);
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, address);
  radio.startListening();

  delay(1000);
  drawPage();
}

void loop() {
  // Receive packets
  if (radio.available()) {
    radio.read(&pkt, sizeof(pkt));
    hasData = true;
    lastPacketTime = millis();

    Serial.print(F("RX: "));
    Serial.print(pkt.latitude, 6); Serial.print(F(", "));
    Serial.println(pkt.longitude, 6);

    drawPage();
    checkAlerts();
  }

  // Signal lost check
  if (hasData && millis() - lastPacketTime > SIGNAL_TIMEOUT_MS) {
    hasData = false;
    lcd.clear();
    lcd.print(F("Signal lost"));
  }

  // Button: change page on press (debounced)
  bool reading = digitalRead(BUTTON_PIN);
  if (reading == LOW && lastButtonState == HIGH &&
      millis() - lastButtonTime > DEBOUNCE_MS) {
    page = (page + 1) % PAGE_COUNT;
    lastButtonTime = millis();
    drawPage();
  }
  lastButtonState = reading;
}
