// spin.c
//
// Example program for bcm2835 library
// Shows how to interface with SPI to transfer a number of bytes to and from an SPI device
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o spin spin.c -l bcm2835
// sudo ./spin
//
// Or you can test it before installing with:
// gcc -o spin -I ../../src ../../src/bcm2835.c spin.c
// sudo ./spin
//
// Author: Mike McCauley
// Copyright (C) 2012 Mike McCauley
// $Id: RF22.h,v 1.21 2012/05/30 01:51:25 mikem Exp $
#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
// Use for testing
//        bcm2835_set_debug(1);
    
    if (!bcm2835_init())
        return 1;
    
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    //bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    //bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
    bcm2835_spi_setClockDivider(22);  //lowest divider for correct data
    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // User slave select
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
    
    // Send a some bytes to the slave and simultaneously read 
    // some bytes back from the slave
    // Most SPI devices expect one or 2 bytes of command, after which they will send back
    // some data. In such a case you will have the command bytes first in the buffer,
    // followed by as many 0 bytes as you expect returned data bytes. After the transfer, you 
    // Can the read the reply bytes from the buffer.
    // If you tie MISO to MOSI, you should read back what was sent.
    
    
    //char buf[] = { 0x01, 0x02, 0x11, 0x33, 0x00, 0x00}; // Data to send
    const unsigned N=640*360  *5;
    char buf[N];char buf2[N];
    int i;unsigned char j=0;
    for(i=0;i<N;i++){if(!j){j++;};buf[i]=j;buf2[i]=j;j++;}
    clock_t c0=clock();
    bcm2835_spi_transfern(buf, N);
    clock_t c1=clock();
    double delta=((double)c1-c0)/CLOCKS_PER_SEC;

    printf("SIZE:%.2fMB\n",N/1024./1024.);
    printf("TIME:%.3fs\n",delta);
    printf("SPEED:%.3fMB/s\n",(N/1024./1024.)/delta);

    int n_correct=0;
    int offset=(buf[1]==0xff)?2:1;
    for(i=0;i<N-offset;i++){n_correct+=(buf2[i]==buf[i+offset]);}
    printf("DATA %s:%i/%i [%.3f%%]\n",(n_correct==(N-offset)?"CORRECT":"INVALID"), n_correct,(N-offset), n_correct*100.0/(N-offset));
    //printf("%i,%i\n",buf2[0],buf[0+offset]);

    // buf will now be filled with the data that was read from the slave
    printf("Read from SPI: %02X  %02X  %02X  %02X %02X %02X\n", buf[0], buf[1], buf[2], buf[3],buf[4],buf[5]);
    
    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}
