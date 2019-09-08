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
 software spi interface
 cc -o oled oled.c fontx.c -lwiringPi -DSOFT_SPI
 SSD1606   RPI
 --------------
 GND    --- Gnd
 VCC    --- 3.3V
 DO     --- SCLK(Pin#23) (You can use Any Pin)
 DI     --- MOSI(Pin#19) (You can use Any Pin)
 RES    --- GPIO2(Pin#3) (You can use Any Pin)
 DC     --- GPIO4(Pin#7) (You can use Any Pin)
 CS     --- GPIO8(Pin#24) (You can use Any Pin)
 i2c interface
 cc -o oled oled.c fontx.c -lwiringPi -DI2C
 SSD1606   RPI
 --------------
 GND    --- Gnd
 VCC    --- 3.3V
 SCK    --- SCL(Pin#5)
 SDA    --- SDA(Pin#3)
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
#include <wiringPiI2C.h>
#include <wiringShift.h>
#include "fontx.h"

// Hardware/Software SPI
#define RST  8  // You can change
#define DC   7  // You can change


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


int main(int argc, char **argv){
    init_hardware_spi();
	
	for (int i = 0; i<8; i++) {
		if (i%2 == 0) frame[i] = 0B01011111;
	}
	
    show_hardware_spi();	
  }

}


/*
Initialize SSD1306 (hardware spi)
*/

void init_hardware_spi(void){


}

/*
Show frame buffer to SSD1306 (spi)
*/

void show_hardware_spi(void){
	// init
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

	// show frame
  digitalWrite(DC,  HIGH);
  digitalWrite(CS, LOW);
  wiringPiSPIDataRW(0, frame, 1024);
  digitalWrite(CS, HIGH);
}


