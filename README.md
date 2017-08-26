Arduino DataFlash library
================================

This is library adds support for the AT45DB SPI flash memory from Adesto Technology (which bought it from Atmel in 2012).
At the moment only the D version is supported.

Hardware setup
-------------------------------
DataFlash is 5V tolerant but you must power it with 3.3V.

| Signal | Arduino pin  | Dataflash |
|--------|--------------|-----------|
|  MISO  |     11       |     1     |
|  MOSI  |     12       |     8     |
|  SCK   |     13       |     2     |
|  SS    |     10       |     4     |
|  RESET | user defined |     3     |
|  WP    | user defined |     5     |

Software setup
-------------------------------
Copy the following filesto your library or sketch folder.
* DataFlash.cpp
* DataFlash.h
* DataFlashCommands.h
* DataFlashInlines.h
* DataFlashSizes.h

DataFlash_test.cpp is a simple unit test program. It is built upon the [arduino-tests library](https://github.com/BlockoS/arduino-tests).
The /examples/ directory contains some sample sketches.

Please refer to the [doxygen documentation](http://blockos.github.io/arduino-dataflash/doxygen/html/) for a more detailed API description.

Example
-------------------------------
The following example shows how to write and read on a AT45DB161D DataFlash.
```cpp
#include <SPI.h>
#include "DataFlash.h"

static const int csPin    = 10;
static const int resetPin = 8;
static const int wpPin    = 7;

DataFlash dataflash;

void setup()
{
  uint8_t status;
  DataFlash::ID id;

  const char* dummyMessage = "Hello world";

  SPI.begin();

  dataflash.setup(csPin, resetPin, wpPin);
  dataflash.begin();

  status = dataflash.status();
  dataflash.readID(id);
  // For a brand new AT45DB161D dataflash
  //  status = BIN(00101100)
  //  id.manufacturer       = 0x1F;
  //  id.device[0]          = 0x26;
  //  id.device[1]          = 0x00;
  //  id.extendedInfoLength = 0x00;

  // Write "Hello world" to buffer 1.
  dataflash.bufferWrite(1, 0);
  for(int i=0; dummyMessage[i] != '\0'; i++)
  {
    SPI.transfer(dummyMessage[i]);
  }

  // Transfer buffer 1 to page 7.
  dataflash.bufferToPage(1, 7);

  // Read page 5.
  dataflash.pageRead(5, 0);
  for(int i=0; i<DF_45DB161_PAGESIZE; i++)
  {
    uint8_t data = SPI.transfer(0xff);
  }
}

void loop()
{
  // nothing
}
```
