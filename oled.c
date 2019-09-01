/*
 SSD1606 example

 hardware spi interface
 cc -o oled oled.c fontx.c -lwiringPi -DSPI
 SSD1606   RPI
 --------------
 GND    --- Gnd
 VCC    --- 3.3V
 DO     --- SCLK(Pin#23)
 DI     --- MOSI(Pin#19)
 RES    --- GPIO2(Pin#3) (You can use Any Pin)
 DC     --- GPIO4(Pin#7) (You can use Any Pin)
 CS     --- CS0(Pin#24)

*/

#include <stdio.h>  
#include <stdint.h>
#include <stdbool.h>  
#include <stdlib.h>  
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringShift.h>
#include "fontx.h"

// Hardware/Software SPI
#define RST  8  // You can change
#define DC   7  // You can change

// Software SPI
#define MOSI 12 // You can change
#define SCLK 14 // You can change
#define CS   10  // You can change

//#define BITBANG    1
//#define SHIFTOUT   2
#define _BV(bit) (1<<(bit))

unsigned char frame[1024]; // frame buffer

void init_hardware_spi(void);
void show_hardware_spi(void);

// 128x64
unsigned char init_command[] = {
    0xAE, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xA1, 0xC8,
    //0xA6, 0xD5, 0x80, 0xDA, 0x12, 0x81, 0x00, 0xB0,
    0xA6, 0xD5, 0x80, 0xDA, 0x12, 0x81, 0xFF,
    0xA4, 0xDB, 0x40, 0x20, 0x00, 0x00, 0x10, 0x8D,
    0x14, 0x2E, 0xA6, 0xAF
};

// 128x32
// unsigned char init_command[] = {
//     0xAE, 0xA8, 0x1F, 0xD3, 0x00, 0x40, 0xA1, 0xC8,
//     //0xA6, 0xD5, 0x80, 0xDA, 0x02, 0x81, 0x00, 0xB0,
//     0xA6, 0xD5, 0x80, 0xDA, 0x02, 0x81, 0xFF,
//     0xA4, 0xDB, 0x40, 0x20, 0x00, 0x00, 0x10, 0x8D,
//     0x14, 0x2E, 0xA6, 0xAF
// };

int main(int argc, char **argv){

  int byte;

  for(byte=0;byte<1024;byte++){
    frame[byte] = 0x00;
  }

  init_hardware_spi();
  show_hardware_spi();	
}


/*
Initialize SSD1306 (hardware spi)
*/

void init_hardware_spi(void){
  int byte;

  for(byte=0;byte<1024;byte++){
    frame[byte] = 0x00;
  }

  wiringPiSetup();
  pinMode (DC, OUTPUT) ;
  pinMode (RST, OUTPUT) ;
  wiringPiSPISetup(0, 32*1000*1000);
  digitalWrite(RST,  HIGH) ;
  delay(50);
  digitalWrite(RST,  LOW) ;
  delay(50);
  digitalWrite(RST,  HIGH) ;
  digitalWrite(DC, LOW);
  digitalWrite(CS, LOW);
  wiringPiSPIDataRW(0, init_command, sizeof(init_command));
  digitalWrite(CS, HIGH);

}

/*
Show frame buffer to SSD1306 (spi)
*/

void show_hardware_spi(void){
  digitalWrite(DC,  HIGH);
  digitalWrite(CS, LOW);
  wiringPiSPIDataRW(0, frame, 1024);
  digitalWrite(CS, HIGH);
}
	