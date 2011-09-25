#include <SPI.h>
#include "DataFlash.h"

#define NUM_PAGES 8

DataFlash dataflash;
uint8_t   loop_cnt;
uint16_t  page;

void setup()
{   
  uint8_t status;
  DataFlash::ID id;
  
  /* Initialize SPI */
  SPI.begin();
  
  /* Let's wait 1 second, allowing use to press the serial monitor button :p */
  delay(1000);
  
  /* Initialize dataflash */
  dataflash.setup(5,6,7);
    
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
  
  loop_cnt = 0;
  page     = 0;
}

void loop()
{
  unsigned int i,j;
  char message[] = "@ write test ";
  char overflow[] = "\nOVERFLOW!\n";
  char buffer[64];
  uint8_t data;

  /* We are going to write a simple text into buffer 1 */
  itoa(loop_cnt, buffer, 10);
  
  /* Set dataflash so that any call to spi_tranfer will write the byte
   * given as argument to the Buffer 1 */
  dataflash.bufferWrite(1, 0);
  /* Transfer the message */
  for(i=0; message[i] != '\0'; ++i)
  {
    SPI.transfer(message[i]);
  }
  /* Transfer the loop counter (as string) */  
  for(i=0; buffer[i] != '\0'; ++i)
  {
    SPI.transfer(buffer[i]);
  }
  
  SPI.transfer('\n');
  
  ++loop_cnt;
  
  if(loop_cnt == 0)
  {
    /* loop_cnt overflow */
    /* To celebrate this we write the string "\nOVERFLOW!\n" to Buffer 1 */
    for(i=0; overflow[i] != '\0'; ++i)
    {
      SPI.transfer(overflow[i]);
    }
  }

  /* Write '\0' to buffer 1. This will help us know that we must stop reading from it. */
  SPI.transfer('\0');

  /* Transfer buffer 1 to 'page' page (with builtin erase) */
  dataflash.bufferToPage(1, page);	

  ++page;
  /* When we wrote the number of pages we wanted (NUM_PAGES), we display their contents by
   * using 2 methods alternatively ;
   *     - PageToBuffer + BufferRead
   *     - PageRead
   */
  if(page >= NUM_PAGES)
  {
    for(i=0; i<NUM_PAGES; ++i)
    {
      if(i & 1)
      {
        Serial.println("Page to buffer");
        dataflash.pageToBuffer(i, 1);    
        dataflash.bufferRead(1, 0);
      }
      else {   
        Serial.println("Page read");
        dataflash.pageRead(i, 0); 
      }
      
      j = 0;
      do
      {
        data = SPI.transfer(0xff);
        if(data != '\0')
          Serial.print(data);
        ++j;
      }while((data != '\0') && (j < 64));
   
      /* Add a little delay otherwise the display will be too fast */   
      delay(500); 
    }
    
    page = 0;
  }
}
