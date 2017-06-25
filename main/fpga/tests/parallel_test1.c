#include "bcm2835.h"
#include <stdio.h>
#include <time.h>

#define BIT0 RPI_V2_GPIO_P1_12 //gpio18
#define BIT1 RPI_V2_GPIO_P1_11 //gpio17
#define BIT2 RPI_V2_GPIO_P1_08 //gpio14
#define BIT3 RPI_V2_GPIO_P1_10 //gpio15
#define BIT4 RPI_V2_GPIO_P1_15 //gpio22
#define BIT5 RPI_V2_GPIO_P1_16 //gpio23
#define BIT6 RPI_V2_GPIO_P1_18 //gpio24
#define BIT7 RPI_V2_GPIO_P1_22 //gpio25
#define CLK  RPI_V2_GPIO_P1_13 //gpio27
#define RST  RPI_V2_GPIO_P1_03 //i2c sda (i2c not used)

const unsigned GPIOMASK=(1<<BIT0)|(1<<BIT1)|(1<<BIT2)|(1<<BIT3)|(1<<BIT4)|
                        (1<<BIT5)|(1<<BIT6)|(1<<BIT7)|(1<<CLK);

void reset()
{
	bcm2835_gpio_write(RST,HIGH);
	bcm2835_gpio_write(RST,LOW);
}

inline unsigned gpio_send(unsigned char val)
{
	bcm2835_gpio_write_multi(GPIOMASK,LOW);
	register unsigned gpioval=((val&0x01)<<BIT0) //gpio18 not working=>LSB
	                         |((val&0x02)<<BIT1-1)
	                         |((val&0x0c)<<BIT2-2)
	                         |((val&0xf0)<<BIT4-4);
	bcm2835_gpio_write_multi(gpioval, HIGH);
	bcm2835_gpio_write(CLK,HIGH); //putting clk into gpioval results in nondeterminism
}

int main (int argc, char* argv[])
{
	if (!bcm2835_init()){return(1);}
	bcm2835_gpio_fsel(BIT0, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT4, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT5, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT6, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BIT7, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(CLK,  BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(RST,  BCM2835_GPIO_FSEL_OUTP);
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(22);  //lowest divider for correct data
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // User slave select
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
	
	
	
	
	
	
	const unsigned N=640*360*18;
	unsigned char bigdata[N]={0x00};
	int i;
	for(i=0;i<20;i++){bigdata[i]=i;}
	
	while(1)
	{
		clock_t c0=clock();
		reset();
		for(i=0;i<sizeof(bigdata);i++)
		{
			gpio_send(bigdata[i]);
		}
		clock_t c1=clock();
		double delta=((double)c1-c0)/CLOCKS_PER_SEC;
		printf("SIZE:%.2fMB\n",N/1024./1024.);
		printf("TIME:%.3fs\n",delta);
		printf("SPEED:%.3fMB/s\n",(N/1024./1024.)/delta);
		
		char buf[4]={0x00}; //spi receive buffer
		bcm2835_spi_transfern(buf, sizeof(buf));
		int offset= (buf[1]==0xff) +1; //offset is sometimes 1 or 2 (vhdl spi bug?)
		unsigned short result=*(unsigned short*)(buf+offset);
		printf("Read from SPI: %02X %02X %02X %02X\n", buf[0],buf[1],buf[2],buf[3]);
		printf("checksum: %i\n",result);
		bcm2835_delay(500);
	}
	
	
	bcm2835_spi_end();
	bcm2835_close();
	return (0);
}

