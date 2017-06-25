/*
 * wrapper.h
 *
 *  Created on: 12 sty 2016
 *      Author: gregStudent
 */

#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "egl_include.h"
#include "Processor/basic_processor.h"
#include "Processor/basic_filter.h"
#include "Processor/grayscale_filter.h"
#include "Processor/adaptive_threshold_processor.h"
#include "Processor/CannyProcessor.h"
#include "Processor/Controller.h"
//#include "Processor/adaptive_threshold.h"
#include "Processor/HoughTransformProcessor.h"
#include "Processor/HarrisProcessor.h"
#include "Processor/max_filter.h"
#include "raspi_config.h"

namespace raspi_gpgpu
{
	typedef struct
	{
		uint32_t screen_width;
		uint32_t screen_height;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		EGLConfig config;
	} EGL_STATE_T;

	class RaspiGL
	{

	private:

		static RaspiGL *instance;
		raspi_gpgpu::Controller *_gpgpuMgr;
		EGL_STATE_T _state;
		cv::Mat * _outputMat;
      std::vector<BasicProcessor*> _filterPtrs;

	public:

		static RaspiGL *GetInstance();

		void Init();

      void init_dispmanx(EGL_DISPMANX_WINDOW_T *nativewindow);

      void egl_from_dispmanx(EGL_STATE_T *state, 
		       EGL_DISPMANX_WINDOW_T *nativewindow);

		cv::Mat * Process(const  cv::Mat &input);

		void Destroy();

		~RaspiGL();
	};
}

#endif /* WRAPPER_H_ */
