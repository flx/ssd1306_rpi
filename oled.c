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

// Software SPI
#define MOSI 12 // You can change
#define SCLK 14 // You can change
#define CS   10  // You can change

//#define BITBANG    1
//#define SHIFTOUT   2
#define _BV(bit) (1<<(bit))

#define I2C_ADDRESS        0x3C

#define SSD1306_DEBUG 0
#define OLED_DEBUG    0

FontxFile fx[2];

typedef struct {
  uint8_t ank;
  uint8_t utf;
  uint8_t colum;
  uint8_t reverse;
  uint8_t enhance;
  uint8_t size;
  uint8_t ascii[16];
  uint16_t sjis[16];
} SaveData;

typedef struct {
  SaveData save[4];
} SaveFrame;

unsigned char frame[1024]; // frame buffer

void init_hardware_spi(void);
void show_hardware_spi(void);

int i2cd;

// 128x64
unsigned char init_command[] = {
    0xAE, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0xA1, 0xC8,
    //0xA6, 0xD5, 0x80, 0xDA, 0x12, 0x81, 0x00, 0xB0,
    0xA6, 0xD5, 0x80, 0xDA, 0x12, 0x81, 0xFF,
    0xA4, 0xDB, 0x40, 0x20, 0x00, 0x00, 0x10, 0x8D,
    0x14, 0x2E, 0xA6, 0xAF
};


int main(int argc, char **argv){
  int i;
  char cpath[128];
  FILE *fp;
  SaveFrame sv;
  
  strcpy(cpath, argv[0]);
  for(i=strlen(cpath);i>0;i--) {
    if (cpath[i-1] == '/') {
      cpath[i] = 0;
      break;
    }
  }
  strcat(cpath,"oled.conf");

  struct stat stat_buf;
  if (stat(cpath,&stat_buf) == 0) {
    fp = fopen(cpath,"rb");
    fread(&sv,sizeof(sv),1,fp);
    fclose(fp);
  } else {
    memset(&sv,0,sizeof(sv));
  }

  if (argc == 1) {
    usage(basename(argv[0]));
    return 0;
  }

  // You can change font file
  Fontx_init(fx,"./fontx/ILGH16XB.FNT","./fontx/ILGZ16XB.FNT"); // 16Dot Gothic
//  Fontx_init(fx,"./fontx/ILMH16XB.FNT","./fontx/ILMZ16XB.FNT"); // 16Dot Mincyo

  int spos;
  char numc[5];
  int num;


  if (strcmp(argv[1],"r") == 0) {
    memset(&sv,0,sizeof(sv));
    fp = fopen(cpath,"wb");
    fwrite(&sv,sizeof(sv),1,fp);
    fclose(fp);
    return 1;
  }

  if (strcmp(argv[1],"s") == 0) {

    init_hardware_spi();
    // int y;
//     for (num=0;num<4;num++) {
//       if (sv.save[num].size == 0) continue;
// //      y = 1;
//       y = sv.save[num].colum+1;
//       for (i=0;i<sv.save[num].size;i++) {
//
//         if (sv.save[num].ank)
//           y = drawChar(num+1,y,sv.save[num].ascii[i],sv.save[num].reverse,
//                        sv.save[num].enhance);
//         if (sv.save[num].utf)
//           y = drawSJISChar(fx,num+1,y,sv.save[num].sjis[i],sv.save[num].reverse,
//                            sv.save[num].enhance);
//       }
//     }
	
	for (int i = 0; i<1024; i++) {
		printf("%d -> %d\n",i,i%2);
		if (i%32 < 16) frame[i] = 0B11111111;
	}
	
    show_hardware_spi();	
  }

  if (strcmp(argv[1],"D") == 0) {
    DumpSaveFrame(sv);
  }
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


