This is a fork of the original to get to minimal c / c++ code to draw on the ssd1306 display. This is not usable for much else ...

# ssd1306_rpi

__Install for Hardware SPI__  
```
git clone https://github.com/nopnop2002/ssd1306_rpi.git  
cd ssd1306_rpi/  
cc -o oled oled.c fontx.c -lwiringPi -lpthread -DSPI  
bash ./test.sh  
```

