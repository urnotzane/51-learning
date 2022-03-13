

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_1_pin(4); // 未使用的io

MFRC522DriverSPI driver_1{ss_1_pin};

MFRC522 readers[]{driver_1}; // Create MFRC522 instance.

/**
 * Initialize.
 */
void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC for debugging.
  while (!Serial)
    ; // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).

  for (MFRC522 reader : readers)
  {
    reader.PCD_Init(); // Init each MFRC522 card.
    Serial.print(F("读卡实例 "));
    static uint8_t i = 0;
    i++;
    Serial.print(i);
    Serial.print(F(": "));
    MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
  }
}

/**
 * Main loop.
 */
void loop()
{
  // Look for new cards.
  for (MFRC522 reader : readers)
  {
    if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial())
    {
      Serial.print(F("第"));
      static uint8_t i = 0;
      i++;
      Serial.print(i);
      Serial.print(F("次读取"));

      // Show some details of the PICC (that is: the tag/card).
      Serial.print(F(": 卡片UID:"));
      MFRC522Debug::PrintUID(Serial, reader.uid);
      Serial.println();

      Serial.print(F("卡片类型: "));
      MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);
      Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));

      // Halt PICC.
      reader.PICC_HaltA();
      // Stop encryption on PCD.
      reader.PCD_StopCrypto1();
    }
  }
}
