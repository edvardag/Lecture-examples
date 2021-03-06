
// July/2014 Omer Siar Baysal
#include <SPI.h>
#include <MFRC522.h>

int successRead; // Variable integer to keep if we have Successful Read from Reader

byte readCard[4];           // Stores scanned ID read from RFID Module

/* We need to define MFRC522's pins and create instance
 * Pin layout should be as follows (on Arduino Uno):
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK : Pin 13 / ICSP-3
 * SS : Pin 10 (Configurable)
 * RST : Pin 9 (Configurable)
 * look MFRC522 Library for
 * pin configuration for other Arduinos.
 */

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

// My cards
byte myCard1[4] = {0x7B,0xA4,0x27,0x49}; // Card #1
byte myCard2[4] = {0xA1,0x17,0xE8,0x2B}; // Card #2

int const redLEDPin = 2;
int const greenLEDPin = 3;
boolean redLEDon = false;
boolean greenLEDon = false;

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {

  //Protocol Configuration
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
}


///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop() {
  successRead = getID(); // sets successRead to 1 when we get read from reader otherwise 0
  if (successRead) ; //the program will not go further while you not get a successful read
  {
    if (areEqual(readCard,myCard1)) {
      if (redLEDon) {
        digitalWrite(redLEDPin, LOW);
        redLEDon = false; } 
       else {
        digitalWrite(redLEDPin, HIGH);
        redLEDon = true;}
    };
    if (areEqual(readCard,myCard2)) {
      if (greenLEDon) {
        digitalWrite(greenLEDPin, LOW);
        greenLEDon = false; } 
       else {
        digitalWrite(greenLEDPin, HIGH);
        greenLEDon = true;}
    };
  }
}


boolean getID() {
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = 0;
  };
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return false;
  }
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}


boolean areEqual( byte a[], byte b[] ) {
  boolean match = true; // Assume they match at first
  for ( int k = 0; k < 4; k++ ) { // Loop 4 times
    if ( a[k] != b[k] ) // IF a != b then set match = false, one fails, all fail
      match = false;
  }
  return match;
}


