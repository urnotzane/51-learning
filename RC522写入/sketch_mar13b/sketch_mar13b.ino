/*
 写入
 */

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <MFRC522Hack.h>

MFRC522DriverPinSimple ss_pin(4); // Configurable, see typical pin layout above.

MFRC522DriverSPI driver(ss_pin);
// MFRC522DriverI2C driver();
MFRC522 mfrc522(driver);                // Create MFRC522 instance.
MFRC522Hack mfrc522Hack(mfrc522, true); // Create MFRC522Hack instance.

/* Set your new UID here! */
constexpr uint8_t newUid[] = {0xDE, 0xAD, 0xBE, 0xEF};

MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(115200); // Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                 // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  mfrc522.PCD_Init(); // Init MFRC522 board.
  Serial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));
  Serial.println(F("This example only works with MIFARE Classic cards."));

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
}

void loop()
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    delay(50);
    return;
  }

  // Now a card is selected. The UID and SAK is in mfrc522.uid.

  // Dump UID.
  Serial.print(F("Card UID:"));
  MFRC522Debug::PrintSelectedUID(mfrc522, Serial);
  Serial.println();

  // Set new UID.
  if (mfrc522Hack.MIFARE_SetUid(newUid, (byte)4, key, true))
  {
    Serial.println(F("Wrote new UID to card."));
  }

  // Halt PICC and re-select it so DumpToSerial doesn't get confused.
  mfrc522.PICC_HaltA();
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // Dump the new memory contents.
  Serial.println(F("New UID and contents:"));
  MFRC522Debug::PICC_DumpToSerial(mfrc522, Serial, &(mfrc522.uid));

  delay(2000);
}
