#include "FPGAFilter.h"
#include"fpga/bcm2835.h"
#include<stdexcept>


dsa::FPGAFilter::FPGAFilter():
	buffer(NULL),buffer_size(-1)
{
	if (!bcm2835_init())
	{throw(std::runtime_error("Could not initialize BMC2835"));}
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(22);          //lowest divider for correct data
	bcm2835_spi_chipSelect(BCM2835_SPI_CS1);                      // User slave select
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);      // the default
}


dsa::FPGAFilter::~FPGAFilter()
{
	if(this->buffer!=NULL){delete[] this->buffer;}
	bcm2835_spi_end();
	bcm2835_close();
}

void dsa::FPGAFilter::init(){};


void dsa::FPGAFilter::apply()
{
	size_t src_size=this->src->total()*this->src->elemSize();
	if(src_size!=this->buffer_size)
	{
		if(this->buffer!=NULL){delete[] this->buffer;}
		this->buffer=new char[src_size+2]; //+2 spi offset
		this->buffer_size=src_size;
	}
	
	bcm2835_spi_transfernb((char*)this->src->data, this->buffer, src_size);
	//2 more bytes because of spi offset
	bcm2835_spi_transfernb((char*)this->src->data, this->buffer+src_size, 2);
	
	//actual offset currently depends on whether fpga already transferred data before
	size_t offset=(this->buffer[1]==0xff) ? 2 : 1;
	
	this->dst=cv::Mat(src->rows, src->cols, src->type(), (void*) (buffer+offset));
	
}









