/*
  RC522读卡
*/

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_pin(4); // 未使用的io

SPIClass &spiClass = SPI; // Alternative SPI e.g. SPI2 or from library e.g. softwarespi.

const SPISettings spiSettings = SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0); // May have to be set if hardware is not fully compatible to Arduino specifications.

MFRC522DriverSPI driver{ss_pin, spiClass, spiSettings}; // Create SPI driver.

MFRC522 mfrc522{driver}; // Create MFRC522 instance.

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC for debugging.
  while (!Serial);
  mfrc522.PCD_Init();   // 初始化 MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);  // 固件版本
  Serial.println(F("卡片读取"));
}

void loop() {
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    // 未发现新卡
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    // 未能选中其中一张卡
    return;
  }
  MFRC522Debug::PICC_DumpDetailsToSerial(mfrc522, Serial, &(mfrc522.uid));
  
  // 未发现新卡前停止 PICC
  mfrc522.PICC_HaltA();
  // 未发现新卡前停止加密PCD
  mfrc522.PCD_StopCrypto1();
  // 结束循环
  return;
}
