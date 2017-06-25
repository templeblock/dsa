/*
 * AutoWhiteBalanceFilter.cpp
 *
 * Some camera's can do this automatically, but we have to assume your's can't.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "AutoWhiteBalanceFilter.h"

/**
 * Constructor
 *
 * No parameters for the filter function to initialize.
 */
dsa::AutoWhiteBalanceFilter::AutoWhiteBalanceFilter(bool assumeGrayWorld) {
	assumeGrayWorld_ = assumeGrayWorld;
	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::AutoWhiteBalanceFilter::~AutoWhiteBalanceFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::AutoWhiteBalanceFilter::init() {
}

/**
 * When filter is ready, run the OpenCV function.
 */
void dsa::AutoWhiteBalanceFilter::apply() {
	if(isReady()) {
		dst = cv::Mat(src->rows, src->cols, src->type());
		// in order to have fast pixel access, we need to differentiate between the pixel type
		cv::Scalar mean = cv::mean(*src);
		if(src->type() == CV_8UC1) {	// RGB/BGR 8bit
			float f = (float)(128.0/mean.val[0]);
			// speed things up: compute lookup table for each channel
			UINT8 LUT[256];
			for(size_t i = 0; i < 256; i++) // we do not use saturate_cast, as our value can only exceed the interval and min() is faster than min() and max()
				LUT[i] = (UINT8)(std::min((int)(i*f), 255));

			for(size_t i = 0; i < src->rows; i++) {	// in all rows
				UINT8* srcrow = src->ptr<UINT8>(i);
				UINT8* dstrow = dst.ptr<UINT8>(i);
				for(size_t j = 0; j < src->cols; j++) // in all cols
					dstrow[j] = LUT[srcrow[j]];
			}
		} else if(src->type() == CV_8UC3) {	// RGB/BGR 8bit
			// speed things up: compute lookup table for each channel
			float f[3] { 0, 0 , 0 };
			if(assumeGrayWorld_) {
				float average = (mean.val[0] + mean.val[1] + mean.val[2])/3.0f;
				f[0] = (float)(average/mean.val[0]);
				f[1] = (float)(average/mean.val[1]);
				f[2] = (float)(average/mean.val[2]);
			} else {
				float average = std::max(std::max(mean.val[0], mean.val[1]), mean.val[2]);
				f[0] = (float)(average/mean.val[0]);
				f[1] = (float)(average/mean.val[1]);
				f[2] = (float)(average/mean.val[2]);
			}
			UINT8 C1_LUT[256];
			UINT8 C2_LUT[256];
			UINT8 C3_LUT[256];
			for(size_t i = 0; i < 256; i++) { // we do not use saturate_cast, as our value can only exceed the interval and min() is faster than min() and max()
				C1_LUT[i] = (UINT8)(std::min((int)(i*f[0]), 255));
				C2_LUT[i] = (UINT8)(std::min((int)(i*f[1]), 255));
				C3_LUT[i] = (UINT8)(std::min((int)(i*f[2]), 255));
			}

			for(size_t i = 0; i < src->rows; i++) {	// in all rows
				cv::Vec3b* srcrow = src->ptr<cv::Vec3b>(i);
				cv::Vec3b* dstrow = dst.ptr<cv::Vec3b>(i);
				for(size_t j = 0; j < src->cols; j++) {	// in all cols
					cv::Vec3b *srcPixel = &(srcrow[j]);
					cv::Vec3b *dstPixel = &(dstrow[j]);
					dstPixel->val[0] = C1_LUT[srcPixel->val[0]];
					dstPixel->val[1] = C2_LUT[srcPixel->val[1]];
					dstPixel->val[2] = C3_LUT[srcPixel->val[2]];
				}
			}
		} else if(src->type() == CV_16UC3) { // RGB/BGR 16Bit
			float f[3] { 0, 0 , 0 };
			if(assumeGrayWorld_) {
				float average = (mean.val[0] + mean.val[1] + mean.val[2])/3.0f;
				f[0] = (float)(average/mean.val[0]);
				f[1] = (float)(average/mean.val[1]);
				f[2] = (float)(average/mean.val[2]);
			} else {
				float average = std::max(std::max(mean.val[0], mean.val[1]), mean.val[2]);
				f[0] = (float)(average/mean.val[0]);
				f[1] = (float)(average/mean.val[1]);
				f[2] = (float)(average/mean.val[2]);
			}

			// speed things up: compute lookup table for each channel (only feasible for 0.25MPixel and above)
			UINT16 C1_LUT[65536];
			UINT16 C2_LUT[65536];
			UINT16 C3_LUT[65536];
			for(size_t i = 0; i < 65536; i++) {	// we do not use saturate_cast, as our value can only exceed the interval and min() is faster than min() and max()
				C1_LUT[i] = (UINT16)(std::min((int)(i*f[0]), 255));
				C2_LUT[i] = (UINT16)(std::min((int)(i*f[1]), 255));
				C3_LUT[i] = (UINT16)(std::min((int)(i*f[2]), 255));
			}
			for(size_t i = 0; i < src->rows; i++) {	// in all rows
				cv::Vec3w* srcrow = src->ptr<cv::Vec3w>(i);
				cv::Vec3w* dstrow = dst.ptr<cv::Vec3w>(i);
				for(size_t j = 0; j < src->cols; j++) {	// in all cols
					cv::Vec3w *srcPixel = &(srcrow[j]);
					cv::Vec3w *dstPixel = &(dstrow[j]);
					dstPixel->val[0] = C1_LUT[srcPixel->val[0]];
					dstPixel->val[1] = C2_LUT[srcPixel->val[1]];
					dstPixel->val[2] = C3_LUT[srcPixel->val[2]];
				}
			}
		}
	}
}
