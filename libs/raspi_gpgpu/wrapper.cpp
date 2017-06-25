/*
 * wrapper.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: gregStudent
 */

#include "wrapper.h"


raspi_gpgpu::RaspiGL *raspi_gpgpu::RaspiGL::instance = NULL;

raspi_gpgpu::RaspiGL* raspi_gpgpu::RaspiGL::GetInstance() {
	 if (!raspi_gpgpu::RaspiGL::instance) {
		 raspi_gpgpu::RaspiGL::instance = new raspi_gpgpu::RaspiGL();
	    }

	    return raspi_gpgpu::RaspiGL::instance;
}

void raspi_gpgpu::RaspiGL::init_dispmanx(EGL_DISPMANX_WINDOW_T *nativewindow) {   
    int32_t success = 0;   
    uint32_t screen_width;
    uint32_t screen_height;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    bcm_host_init();

    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, 
					&screen_width, 
					&screen_height);
    assert( success >= 0 );

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = screen_width;
    dst_rect.height = screen_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screen_width << 16;
    src_rect.height = screen_height << 16;        

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = 
	vc_dispmanx_element_add(dispman_update, dispman_display,
				0/*layer*/, &dst_rect, 0/*src*/,
				&src_rect, DISPMANX_PROTECTION_NONE, 
				0 /*alpha*/, 0/*clamp*/, 0/*transform*/);

    // Build an EGL_DISPMANX_WINDOW_T from the Dispmanx window
    nativewindow->element = dispman_element;
    nativewindow->width = screen_width;
    nativewindow->height = screen_height;
    vc_dispmanx_update_submit_sync(dispman_update);

    printf("Got a Dispmanx window\n");
}

void raspi_gpgpu::RaspiGL::egl_from_dispmanx(EGL_STATE_T *state, 
		       EGL_DISPMANX_WINDOW_T *nativewindow) {
    EGLBoolean result;

   printf("egl_from_dispmanx\n");
    _state.surface = eglCreateWindowSurface(_state.display, 
					    _state.config, 
					    nativewindow, NULL );
    assert(_state.surface != EGL_NO_SURFACE);

    // connect the context to the surface
    //result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
    //assert(EGL_FALSE != result);
	if(!eglMakeCurrent( _state.display, _state.surface, _state.surface, _state.context))
				printf("Error by making context current context");
	EGLint err2 = eglGetError();
   std::cout<< std::endl << "eglGetError() was: " << err2 << std::endl;
}

void raspi_gpgpu::RaspiGL::Init() {
	//EGL_STATE_T state;//, *_p_state = &state;
		//EGL_STATE_T *_p_state = new EGL_STATE_T();

		//EGL_STATE_T *_p_state = &this->_state;

		EGLBoolean result;
		EGLint num_configs;
		EGLint major, minor, count, n, size;
		EGLConfig *configs;
		EGLNativeWindowType eglNativeWindow;
      EGL_DISPMANX_WINDOW_T nativewindow;

		static const EGLint attribute_list[] =
		{
			EGL_RED_SIZE, 8,
		   EGL_GREEN_SIZE, 8,
		   EGL_BLUE_SIZE, 8,
		   EGL_ALPHA_SIZE, 8,
		   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	   	EGL_NONE
		};
	static const EGLint context_attributes[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, 2,
		   EGL_NONE
		};

		try
		{
			// get an EGL display connection
			//EGLDisplay disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			_state.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			assert(EGL_NO_DISPLAY != _state.display);
			// initialize the EGL display connection
			result = eglInitialize(_state.display, NULL, NULL);
			assert(EGL_FALSE != result);

			// get configuration
			eglGetConfigs(_state.display, NULL, 0, &num_configs);
				printf("EGL has %d configs\n", num_configs);

			configs = (EGLConfig*) calloc(num_configs, sizeof(EGLConfig));
			eglGetConfigs(_state.display, configs, num_configs, &num_configs);

			// get an appropriate EGL configuration - just use the first available
			result = eglChooseConfig(_state.display, attribute_list,
					&_state.config, 1, &num_configs);
			assert(EGL_FALSE != result);

         // finally tet context
         _state.context = eglCreateContext(_state.display,
					_state.config, EGL_NO_CONTEXT,
								  context_attributes);
			assert(_state.context!=EGL_NO_CONTEXT);

			//eglNativeWindow = elgCreateNativeWindow();
         init_dispmanx(&nativewindow);
         egl_from_dispmanx(&_state, &nativewindow);

			//_state.surface = eglCreateWindowSurface(_state.display, _state.config, eglNativeWindow, NULL);
			assert(EGL_NO_SURFACE != _state.surface);
			// Choose the OpenGL ES API
			result = eglBindAPI(EGL_OPENGL_ES_API);
			assert(EGL_FALSE != result);

			//if(!eglMakeCurrent( _state.display, _state.surface, _state.surface, _state.context))
			//	printf("Error by making context current context");


			// test , check if context valid
			std::cout << "getting gl api " <<  glGetString(GL_VERSION) << std::endl;

			std::cout << "getting egl api " << eglQueryString(_state.display, EGL_CLIENT_APIS) << std::endl;


			// prepare ogles objct
			_gpgpuMgr = raspi_gpgpu::Controller::GetInstance();

			raspi_gpgpu::GrayscaleProcessor* grayscale  = new raspi_gpgpu::GrayscaleProcessor;
			//raspi_gpgpu::AdaptiveThresholdProcessor* ada  = new raspi_gpgpu::AdaptiveThresholdProcessor;
			raspi_gpgpu::CannyProcessor* canny = new raspi_gpgpu::CannyProcessor;
			raspi_gpgpu::HoughTransformProcessor* hough = new raspi_gpgpu::HoughTransformProcessor;
        
         raspi_gpgpu::MaxProcessor* maxp = new raspi_gpgpu::MaxProcessor;

			_gpgpuMgr->AddProcessor(grayscale);
			//_gpgpuMgr->AddProcessor(ada);
			_gpgpuMgr->AddProcessor(canny);
         if(raspi_gpgpu::RaspiConfig::Instance()->_pipeline != 1){
			   _gpgpuMgr->AddProcessor(hough);
            //_gpgpuMgr->AddProcessor(maxp);
      
         }

			_gpgpuMgr->Init(_state.context);


         _filterPtrs.push_back(grayscale);
			//_filterPtrs.push_back(ada);
			_filterPtrs.push_back(canny);
			_filterPtrs.push_back(hough);
			_filterPtrs.push_back(maxp);
		
			//_filterPtrs.push_back(erosion);
		}
		catch(const std::exception& e)
		{
			std::cout << "erro in try catch " << e.what() << std::endl;
			eglDestroyContext(_state.display, _state.context);
		}
}

cv::Mat* raspi_gpgpu::RaspiGL::Process(const cv::Mat& input) {

	try
		{
		    //cv::Mat test;
		    //cv::cvtColor ( input, test, CV_BGR2GRAY );
			_gpgpuMgr = raspi_gpgpu::Controller::GetInstance();

			//  Make sure that cv::Mat::type() returns CV_8U*
			//int rt = (input.type() == CV_8UC3);
			//cv::Mat grey;
			//cv::cvtColor ( input,grey,CV_BGR2GRAY );
			//assert(grey.type() == CV_8UC1);

			//printf("\nInput is type %d", rt);
			//cv::imwrite("inputFile.jpg", input);
			unsigned char *pixels = (unsigned char *)input.data;
			//uchar *output = NULL;
			//cv::Mat outputMat(input);
			//unsigned char *output = (unsigned char *)outputMat.data;
         int houghHeight = raspi_gpgpu::RaspiConfig::GetHoughHeight();
         std::cout << "\nhoughHeight: " << houghHeight << std::endl;

			if(!_outputMat)
				_outputMat = new cv::Mat( cv::Mat::zeros(houghHeight, input.cols, CV_8UC3));
			//printf("\n\n\nshowing inputa\n\n");
			//imshow( "showing input", input );
			unsigned char *output = (unsigned char *)_outputMat->data;

			if(!_gpgpuMgr->IsInitialized())
			{
				_gpgpuMgr->CreateRender(input.cols, input.rows, 0);
				_gpgpuMgr->Prepare(input.cols, input.rows ,true);
			}

			//clock_t shadert = clock();
			_gpgpuMgr->SetInputData(pixels);
			_gpgpuMgr->Process();
			_gpgpuMgr->GetOutputData(output);

			//printf("\n\n\nshowing outputa %f\n\n", ogles_gpgpu::Tools::calculateTimer(shadert));
			//imshow( "showing input", *_outputMat );
			//cv::imwrite("outputFile.jpg",*_outputMat);

			return (_outputMat);

		}
		catch(const std::exception& e)
		{
			std::cout << "erro in try catch " << e.what() << std::endl;
		}
}

void raspi_gpgpu::RaspiGL::Destroy() {
	eglDestroyContext(_state.display, _state.context);
	raspi_gpgpu::Controller* ptr  = raspi_gpgpu::Controller::GetInstance();
	ptr->Dispose();
	delete _outputMat;

   for(int i = 0; i < _filterPtrs.size(); i++)
		delete _filterPtrs[i];
}

raspi_gpgpu::RaspiGL::~RaspiGL() {
}





