/**
 * VideoHub.cpp
 *
 *	Derived from the "interface" DataHub. Can be used to capture frames via any VideoCapture source and hand out as many copies of or references to its data as
 *	required. This hub can only be used to access the capture device directly. If you want to distribute a modified frame to multiple recipients, use an ImageHub
 *	instead.
 *
 *  Created on: 13.02.2012
 *      Author: jung
 */

#include "VideoHub.h"

/**
 * Constructor. Initializes the VideoCapture object to capture from a camera.
 *
 * @param capture: any type of VideoCapture object
 */
#ifdef WITH_RASPICAM
dsa::VideoHub::VideoHub(raspicam::RaspiCam_Cv* const capture) : DataHub() {
#else
dsa::VideoHub::VideoHub(cv::VideoCapture* const capture) : DataHub() {
#endif
	haveStrobe_ = false;
	activateLamp_ = false;
	activateLamp = &activateLamp_;
	capture_= capture;
	//grabData();
}

/**
 * Destructor.
 */
dsa::VideoHub::~VideoHub() {
	delete capture_;
}

/**
 * Grabs the next frame + meta info into data_ from the VideoCapture object.
 */
void dsa::VideoHub::grabData() {
	DataHub::grabData();
	clock_t grabTimestamp = 0, dataTimestamp = 0;
	for(int i = 0; i < 10; i++) {
		grabTimestamp = currentTimeOfDayMilliseconds();
		if(haveStrobe_)
			{(*activateLamp) ? strobe_.startContinuous() : strobe_.stopContinuous();}
		#ifdef WITH_RASPICAM
		if(capture_->grab())
		{
			capture_->retrieve(dst_);
		#else
		if(capture_->read(dst_)) 
		{
		#endif

        std::stringstream stmp;
        if(raspi_gpgpu::RaspiConfig::Instance()->_imageNr > -1)
            stmp << raspi_gpgpu::RaspiConfig::Instance()->_imageNr;
        else
   	     stmp << _lastIdx;
	     //std::string path = "/home/pi/raspi_record/testImages/640/image" + stmp.str() + "_";
        std::string path = "../testImages/640/image" + stmp.str() + "_";
	     stmp.str("");
	     int sz = 640;
	       stmp << sz;
	     path += stmp.str() + "_B.bmp";
	     std::cout<< "Loading File:" << std::endl << path << std::endl;

	    if(_lastIdx < 69)
	    	_lastIdx++;
	    else
	    	_lastIdx = 0;


	       //dst_ =  cv::imread("d:\\dsa_dev\\dsa_dev\\workspace\\dsaImages\\images\\test1.bmp", CV_LOAD_IMAGE_COLOR);
         if(raspi_gpgpu::RaspiConfig::Instance()->_allFrames || raspi_gpgpu::RaspiConfig::Instance()->_imageNr > -1)
             dst_ =  cv::imread(path.c_str(), CV_LOAD_IMAGE_UNCHANGED);
         else
         {            
   	         //dst_ =  cv::imread("/home/pi/raspi_record/images/image23_640_B.bmp", CV_LOAD_IMAGE_UNCHANGED); // 34  
               dst_ =  cv::imread("/home/pi/raspi_record/oldies/image7_1280_B.bmp", CV_LOAD_IMAGE_UNCHANGED); // 34          
         }
		  

         //dst_ =  cv::imread("/home/pi/raspi_record/oldies/images/image7_1280_B.bmp", CV_LOAD_IMAGE_UNCHANGED);


			dataTimestamp = (clock_t)capture_->get(CV_CAP_PROP_POS_MSEC);
			if(dataTimestamp == 0)
				dataTimestamp = grabTimestamp;
			break;
		} else if(i >= 9) {
			std::cout << "Video capture object seems dead, exiting" << std::endl;
			exit(1);
		} else {
			std::cout << "Failed to grab image" << std::endl;
		}
	}
	//*capture_ >> frame_; // alternativ könnte auch der von VideoCapture überladene operator >> genutzt werden.
	original_ = ImageSensorData(dst_, grabTimestamp, dataTimestamp);
}

/**
 * Grabs a new frame and returns the captured data

 * @param copy return a copy of the data (true) or only a reference to the data (false)
 * @return the captured ImageSensorData
 */
dsa::AbstractSensorData* dsa::VideoHub::getData(const bool copy) {
	grabData();
	ImageSensorData* const data = (copy) ? new ImageSensorData(ImageSensorData(original_)) : new ImageSensorData(original_);
	return(data);
}

/**
 * Grabs a channel from a new frame and returns the captured data
 *
 * @param channel return only a specific channel of the image
 * @param copy return a copy of the data (true) or only a reference to the data (false)
 * @return the captured ImageSensorData
 */
dsa::AbstractSensorData* dsa::VideoHub::getData(const int channel, const bool copy) {
	grabData();
	ImageSensorData* const data = (copy) ? new ImageSensorData(ImageSensorData(original_, channel)) : new ImageSensorData(original_, channel);

	return(data);
}

/**
 * Grabs and returns a new frame if the currently held frame is older than clock_t, otherwise returns the currently
 * held frame without doing any grabbing. This is mostly to reduce I/O from the camera by supplying a buffered copy of
 * the last shot taken if it isn't too old.
 *
 * @param clock_t how an old frame will we accept (in ms) without insisting a new frame being fetched
 * @param copy return a copy of the data (true) or only a reference to the data (false)
 * @return the ImageSensorData requested (either old or new)
 */
dsa::AbstractSensorData* dsa::VideoHub::getData(const clock_t clock_t, const bool copy) {
	if(needsGrabbing(clock_t))
		grabData();
	ImageSensorData* const data = (copy) ? new ImageSensorData(ImageSensorData(original_)) : new ImageSensorData(original_);

	return(data);
}

/**
 * Grabs and returns a new frame if the currently held frame is older than clock_t, otherwise returns the currently
 * held frame without doing any grabbing. This is mostly to reduce I/O from the camera by supplying a buffered copy of
 * the last shot taken if it isn't too old.
 *
 * @param clock_t how an old frame will we accept (in ms) without insisting a new frame being fetched
 * @param copy return a copy of the data (true) or only a reference to the data (false)
 * @return the ImageSensorData requested (either old or new)
 */
dsa::AbstractSensorData* dsa::VideoHub::getData(const clock_t clock_t, const int channel, const bool copy) {
	if(needsGrabbing(clock_t))
		grabData();
	ImageSensorData* const data = (copy) ? new ImageSensorData(ImageSensorData(original_, channel)) : new ImageSensorData(original_, channel);

	return(data);
}

/**
 * Return pointer to the VideoCapture object used by the hub. This is useful, if you want to make changes to
 * the capture attributes. This is currently only for test purposes, later we will have to remove this and have the
 * hub do all the work, since we do not want to (and really shouldn't) surrender control over the camera.
 *
 * @return const pointer to the VideoCapture object
 */
#ifdef WITH_RASPICAM
raspicam::RaspiCam_Cv* dsa::VideoHub::getCapture() const {
#else
cv::VideoCapture* dsa::VideoHub::getCapture() const {
#endif
	return(capture_);
}

/**
 * Attach a strobe controller to this video hub
 *
 * @param strobeController
 */
void dsa::VideoHub::setStrobeController(StrobeController &strobeController) {
	strobe_ = strobeController;
	haveStrobe_ = true;
}
