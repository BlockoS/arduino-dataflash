#include <SPI.h>
#include "DataFlash.h"

DataFlash dataflash;

void printSectorProtectionStatus(const DataFlash::SectorProtectionStatus &status, uint8_t count)
{
  Serial.println("Sector protection: ");
  for(int8_t i=AT45_SECTOR_0A; i<count; i++)
  {
    Serial.print("Sector #");
    if(i == AT45_SECTOR_0A) Serial.print("0a");
    else if(i == AT45_SECTOR_0B) Serial.print("0b");
    else Serial.print(i, DEC);
    Serial.print(' ');
    Serial.print(status.get(i), DEC);
    Serial.print('\n');
  }
}

const char *message = "Writing something!";

void setup()
{   
  uint8_t status;
  DataFlash::ID id;
  
  /* Initialize SPI */
  SPI.begin();
  
  /* Let's wait 1 second, allowing use to press the serial monitor button :p */
  delay(1000);
  
  /* Initialize dataflash */
  dataflash.setup(53, 49);
    
  delay(10);
  
  dataflash.begin();
  
  /* Read status register */
  status = dataflash.status();
  
  /* Read manufacturer and device ID */
  dataflash.readID(id);

  /* Set baud rate for serial communication */
  Serial.begin(115200);

  /* Display status register */
  Serial.print("Status register :");
  Serial.print(status, BIN);
  Serial.print('\n');

  /* Display manufacturer and device ID */
  Serial.print("Manufacturer ID :\n");  // Should be 00011111
  Serial.print(id.manufacturer, HEX);
  Serial.print('\n');

  Serial.print("Device ID (part 1) :\n"); // Should be 00011111
  Serial.print(id.device[0], HEX);
  Serial.print('\n');

  Serial.print("Device ID (part 2)  :\n"); // Should be 00000000
  Serial.print(id.device[1], HEX);
  Serial.print('\n');

  Serial.print("Extended Device Information String Length  :\n"); // 00000000
  Serial.print(id.extendedInfoLength, HEX);
  Serial.print('\n');
  
  DataFlash::SectorProtectionStatus protection;
  uint8_t count;

  count = dataflash.readSectorProtectionRegister(protection);
  printSectorProtectionStatus(protection, count);

  protection.clear();
  protection.set(2, true);
  printSectorProtectionStatus(protection, count);
 
  dataflash.enableSectorProtection();
  dataflash.programSectorProtectionRegister(protection);

  count = dataflash.readSectorProtectionRegister(protection);
  printSectorProtectionStatus(protection, count);

  Serial.println("Write");
  dataflash.bufferWrite(0, 0);
  for(int i=0; message[i] != '\0'; i++)
  {
     SPI.transfer(message[i]);
  }
  dataflash.bufferToPage(0, 256); 
  
  Serial.println("Read");
  dataflash.pageToBuffer(256, 1);    
  dataflash.bufferRead(1, 0);
  for(int i=0; message[i] != '\0'; i++)
  {
     char c = SPI.transfer(0xff);
     Serial.print(c); 
  }
  Serial.print('\n');

  dataflash.disableSectorProtection();

}

void loop()
{
}
