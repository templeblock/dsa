#ifndef FPGAFilter_H_
#define FPGAFilter_H_


//#include "fpga/ImageFilter.h"
#include "ImageFilter.h"
namespace dsa
{



class FPGAFilter:public ImageFilter
{	private:
	char* buffer;
	size_t buffer_size;
	void init();

	public:
	FPGAFilter();
	virtual ~FPGAFilter();
	
	void apply();
};

}

#endif
