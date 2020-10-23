#include <Adafruit_NeoPixel.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include "SPI.h"

const int      delayval  =  50; // Time each pixel pauses to do it's duty.
const int      PIN       =  2; // Data output to Adafruit NeoPixel
const int      NUMPIXELS =  24; // Number of pixels in the ring ^
const int      SS_PIN    =  10; // Data PIN for MFRC522
const int      RST_PIN   =  9; // Reset PIN for MFRC522
const int      TIME      =  150; // Time between each NeoPixel blink.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint32_t red       =  pixels.Color(255, 0, 0);
const uint32_t green     =  pixels.Color(0, 255, 0);
const uint32_t yellow    =  pixels.Color(255, 255, 0);
const uint32_t blank     =  pixels.Color(0, 0, 0);

const uint32_t storm1    =  pixels.Color(14, 30, 49);
const uint32_t storm2    =  pixels.Color(47, 69, 93);
const uint32_t storm3    =  pixels.Color(113, 122, 125);
const uint32_t storm4    =  pixels.Color(187, 187, 179);
const uint32_t storm5    =  pixels.Color(255, 255, 0);

const uint32_t rain1    =  pixels.Color(135, 187, 173);
const uint32_t rain2    =  pixels.Color(172, 206, 186);
const uint32_t rain3    =  pixels.Color(197, 231, 202);
const uint32_t rain4    =  pixels.Color(207, 233, 255);
const uint32_t rain5    =  pixels.Color(151, 192, 255);

const uint32_t sunRise1    =  pixels.Color(189, 152, 108);
const uint32_t sunRise2    =  pixels.Color(249, 151, 64);
const uint32_t sunRise3    =  pixels.Color(245, 181, 108);
const uint32_t sunRise4    =  pixels.Color(207, 95, 49);
const uint32_t sunRise5    =  pixels.Color(164, 140, 100);

const uint32_t sun1    =  pixels.Color(227, 103, 0);
const uint32_t sun2    =  pixels.Color(242, 116, 11);
const uint32_t sun3    =  pixels.Color(229, 154, 39);
const uint32_t sun4    =  pixels.Color(214, 148, 21);
const uint32_t sun5    =  pixels.Color(242, 197, 89);


MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting all...\n\n");
  pixels.begin();

  SPI.begin(); // Make the NeoPixel and microcontroller communicate
  rfid.PCD_Init();
}

void loop() {


  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    setAll(blank);
    return;
  }
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();

  while (validateCard(strID) != "invalid") {
    Serial.println("Working card");
    Serial.print("Card ID is: ");
    Serial.println(strID);
    Serial.print("");
    if (validateCard(strID) == "storm") {
      paintStorm();
    }
    else if (validateCard(strID) == "rain") {
      paintRain();
    }
    else if (validateCard(strID) == "sunrise") {
      paintSunrise();
    }
    else if (validateCard(strID) == "sun") {
      paintSun();
    }
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
      break;
    }
  }
  if (validateCard(strID) == "invalid") {
    Serial.println("Invalid card");
    Serial.print("Card ID is: ");
    Serial.println(strID);
    Serial.print("");
  }
}

void paintStorm() {
  setAll(storm1);
  delay(750);
  setAll(storm2);
  delay(750);
  setAll(storm3);
  delay(750);
  setAll(storm4);
  delay(750);
  setAll(storm5);
  delay(750);
}

void paintRain() {
  setAll(rain1);
  delay(750);
  setAll(rain2);
  delay(750);
  setAll(rain3);
  delay(750);
  setAll(rain4);
  delay(750);
  setAll(rain5);
  delay(750);
}

void paintSunrise() {
  setAll(sunRise1);
  delay(750);
  setAll(sunRise2);
  delay(750);
  setAll(sunRise3);
  delay(750);
  setAll(sunRise4);
  delay(750);
  setAll(sunRise5);
  delay(750);
}

void paintSun() {
  setAll(sun1);
  delay(750);
  setAll(sun2);
  delay(750);
  setAll(sun3);
  delay(750);
  setAll(sun4);
  delay(750);
  setAll(sun5);
  delay(750);
}


void setAll(uint32_t color) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
  }
}

String validateCard(String usercard) {
  // Insert your own card ID's here, insert more with else if under
  if (usercard.indexOf("69:F0:1C:C9") >= 0) {
    return "storm";
  }
  else if (usercard.indexOf("D4:BB:19:2A") >= 0) {
    return "rain";
  }
  else if (usercard.indexOf("2B:FC:78:13") >= 0) {
    return "sunrise";
  }
  else if (usercard.indexOf("4E:96:98:C2") >= 0) {
    return "sun";
  }
  else return "invalid";
}
