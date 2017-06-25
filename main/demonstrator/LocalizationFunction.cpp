/*
 * LocalizationFunction.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "LocalizationFunction.h"


dsa::LocalizationFunction::LocalizationFunction(raspicam::RaspiCam_Cv* capture, double fps, int exposure, int width, int height, double rotation, double gain) :
dsa::Sensorfunction(SensorType::Localization)
{

	capture_ = capture;
	fps_ = fps;
	exposure_ = exposure;
	width_ = width;
	height_ = height;
	rotation_ = rotation;
	gain_ = gain;

	stopped_ = false;

	result_ = LocalizationResult();
	//VideoHub hub_=NULL;
	//LocalizationResult lastValid_=NULL;
	filterChain_=NULL;
	pfilter5_=NULL;
	pfilter0_=NULL;
	pfilter3_=NULL;
	pfilter6_=NULL;
	//cv::Mat trans_=NULL;
	//cv::Mat rot_=NULL;
}
void dsa::LocalizationFunction::init(){
	// configure camera
	bool autoexposure = false;
	//capture_->set(CV_CAP_PROP_FPS, fps_);
	if(exposure_ > 0)
		capture_->set(CV_CAP_PROP_EXPOSURE, exposure_);
	else
		autoexposure = true;
	if(gain_ >= 0)
		capture_->set(CV_CAP_PROP_GAIN, gain_);
}
dsa::LocalizationFunction::~LocalizationFunction() {
}

void dsa::LocalizationFunction::loadConfig(){
	width_ = capture_->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    height_ = capture_->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    // load settings
    // diese Einstellungen müssen noch zentralisiert werden
    config_ = std::string("raspicam");
    settings_= Settings("config/"+config_);
    settings_.load();
    cv::Size imageSize = cv::Size(width_, height_);
    settings_.camParameters.resize(imageSize);
    settings_.camParameters.CamSize = imageSize;
    camparams_ = settings_.camParameters;
}
void dsa::LocalizationFunction::InitialiseFilters()
{
	// configure camera
	    //VideoHub hub(capture_);
		this->phub_ = new VideoHub(capture_);
		// where to put the video file
		trans_ = cv::Mat::zeros(3, 1, CV_32FC1);
	    rot_ = cv::Mat::zeros(3, 1, CV_32FC1);
		trans_.at<float>(0) = 136.6f;
		trans_.at<float>(1) = 30.3f;
		//rot.at<float>(2) = PI;


		// filter chain
		std::list<Filter*> *filterList = new std::list<Filter*>();
		// filter chain to execute
		GrabImageFilter		*filter0 = new GrabImageFilter(*phub_);
		//UndistortFilter		*filter1 = new UndistortFilter(camparams_.CameraMatrix, camparams_.Distorsion, cv::Mat(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255));
		//AutoWhiteBalanceFilter *filter2 = new AutoWhiteBalanceFilter();
      //ResizeFilter		*filter0_1 = new ResizeFilter(resizedSize, cv::INTER_NEAREST);
		FindMarkersFilter	*filter3 = new FindMarkersFilter(settings_);
		SplitMarkersByIdFilter	*filter4 = new SplitMarkersByIdFilter(config_);
		EstimatePoseFilter	*filter5 = new EstimatePoseFilter(settings_, 3, 4);
		CoordinateTransformFilter *filter6 = new CoordinateTransformFilter(trans_, rot_, false, true);
		//VideoWriterFilter	*filter7 = new VideoWriterFilter(ausgabe_vid, CV_FOURCC('X', 'V', 'I', 'D'), fps_, false);
		//ImageOutputFilter	*filter7 = new ImageOutputFilter("Ausgabe Lokalisierung");
		WaitFilter			*filter8 = new WaitFilter((float)fps_);	// wait for desired framerate

		
      //filter1->src=&(filter0->dst);	
      //filter3->src = &(filter1->dst);

   	filter3->src = &(filter0->dst);	     	
	   filter4->src = &(filter3->dst);
   	filter5->src = &(filter4->dst);
   	filter6->src = &(filter5->dst);
   	filter8->src = &(filter6->dst);
   	

		filter4->markers = &(filter3->markers);
		filter5->markers = &(filter4->location_markers);
		filter5->poses = &(filter4->location_poses);
		filter6->position = &(filter5->camPosition);
		filter6->rotation = &(filter5->camRotation);
		filter6->rotationTransformer = &(filter5->camRotation);

		filterList->push_back(filter0);
		//filterList->push_back(filter1);
		
		filterList->push_back(filter3);
		filterList->push_back(filter4);
		filterList->push_back(filter5);
		filterList->push_back(filter6);
		//filterList->push_back(filter7);
		filterList->push_back(filter8);

		pfilter0_ = filter0;
		pfilter3_ = filter3;
		pfilter5_ = filter5;
		pfilter6_ = filter6;
		FilterChain *fltr = new FilterChain(filterList, true);
		this->filterChain_ = fltr;
}
/**
 * die relevanten Stellschrauben für mehr Performance in diesem Demonstrator sind:
 * - geringere Auflösung (Skalierung)
 * - UndistortFilter mit interpolation=INTER_NEAREST verwenden und blur() auf den Bildkandidaten ausführen (ob das funktioniert)
 * - UndistortFilter weiter verbessern (unwahrscheinlich) oder ganz eliminieren (siehe Wiki)
 * - FindMarkerFilter weiter verbessern (unwahrscheinlich)
 */
void dsa::LocalizationFunction::run() {
	// configure camera
/*
	bool autoexposure = false;
	capture_->set(CV_CAP_PROP_FPS, fps_);
	if(exposure_ > 0)
		capture_->set(CV_CAP_PROP_EXPOSURE, exposure_);
	else
		autoexposure = true;
	if(gain_ >= 0)
		capture_->set(CV_CAP_PROP_GAIN, gain_);
	VideoHub hub(capture_);

	cv::Size imageSize = cv::Size(width_, height_);
	cv::Size resizedSize = cv::Size(std::min(640, width_), std::min(360, height_));

	// load settings
	// diese Einstellungen müssen noch zentralisiert werden
	std::string config = std::string("webcam_logitechc310");
	Settings settings = Settings("config/"+config);
	settings.load();
	settings.camParameters.resize(imageSize);
	settings.camParameters.CamSize = imageSize;
	aruco::CameraParameters camparams = settings.camParameters;

	// where to put the video file
	cv::Mat trans = cv::Mat::zeros(3, 1, CV_32FC1);
	cv::Mat rot = cv::Mat::zeros(3, 1, CV_32FC1);
	trans.at<float>(0) = 136.6f;
	trans.at<float>(1) = 30.3f;
	//rot.at<float>(2) = PI;

	// filter chain
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// filter chain to execute
	GrabImageFilter		*filter0 = new GrabImageFilter(hub);
	ResizeFilter		*filter0_1 = new ResizeFilter(resizedSize, cv::INTER_NEAREST);
	#ifdef WITH_FPGA
	FPGAFilter          *filter1=new FPGAFilter();
	#endif
	FindMarkersFilter	*filter3 = new FindMarkersFilter(settings);
	SplitMarkersByIdFilter	*filter4 = new SplitMarkersByIdFilter(config);
	EstimatePoseFilter	*filter5 = new EstimatePoseFilter(settings, 3, 4);
	CoordinateTransformFilter *filter6 = new CoordinateTransformFilter(trans, rot, false, true);
	//VideoWriterFilter	*filter7 = new VideoWriterFilter(ausgabe_vid, CV_FOURCC('X', 'V', 'I', 'D'), fps_, false);
	#ifdef WITH_RASPICAM
	ImageServerFilter* filter8=new ImageServerFilter();
	#else
	ImageOutputFilter	*filter8 = new ImageOutputFilter("Ausgabe Lokalisierung");
	#endif
	WaitFilter			*filter9 = new WaitFilter((float)fps_);	// wait for desired framerate

	filter0_1->src=&(filter0->dst);
	#ifdef WITH_FPGA
	filter1->src = &(filter0_1->dst);
	filter3->src = &(filter1->dst);
	#else
	filter3->src = &(filter0_1->dst);
	#endif
	filter4->src = &(filter3->dst);
	filter5->src = &(filter4->dst);
	filter6->src = &(filter5->dst);
	filter8->src = &(filter6->dst);
	filter9->src = &(filter6->dst);

	filter4->markers = &(filter3->markers);
	filter5->markers = &(filter4->location_markers);
	filter5->poses = &(filter4->location_poses);
	filter6->position = &(filter5->camPosition);
	filter6->rotation = &(filter5->camRotation);
	filter6->rotationTransformer = &(filter5->camRotation);

	filterList->push_back(filter0);
	filterList->push_back(filter0_1);
	#ifdef WITH_FPGA
	filterList->push_back(filter1);
	#endif
	filterList->push_back(filter3);
	filterList->push_back(filter4);
	filterList->push_back(filter5);
	filterList->push_back(filter6);
	filterList->push_back(filter8);

	FilterChain chain(filterList, true);
*/
   int index = 0;
   int key = 0;
   int end = 1;
   if(raspi_gpgpu::RaspiConfig::Instance()->_allFrames)
      end = 52;

   std::cout<<"Execute frames "<< end << std::endl; 
	while(!stopped_ && index < end){	// loop forever

      clock_t c0=clock();  
		filterChain_->apply();	// apply all filters in chain
      std::cout<<"CHAIN FRAMES PER SECOND "<<std::fixed<< 1.0f / (((float)clock() - c0)/CLOCKS_PER_SEC) << std::endl; 

		//key = cv::waitKey(1000);
      if(key == 27)
         break;
       // mtx_.lock();
		if(pfilter5_->poses->size() > 0) {
			result_.isValid = true;
			result_.timestamp = pfilter0_->dataTimestamp;
			result_.t[0] = pfilter6_->transPosition.at<float>(0);
			result_.t[1] = pfilter6_->transPosition.at<float>(1);
			result_.t[2] = pfilter6_->transPosition.at<float>(2);
			result_.r[0] = pfilter6_->transRotation.at<float>(0);
			result_.r[1] = pfilter6_->transRotation.at<float>(1);
			result_.r[2] = pfilter6_->transRotation.at<float>(2);
			// update lastValid_
			lastValid_ = LocalizationResult(result_);
		} else {
			result_.isValid = false;
		}

      std::cout << "=========== POSE ===========" << std::endl;
      std::cout << "The result position" << std::endl;
      std::cout << "T:" << result_.t[0]<< " " << result_.t[1] << " " << result_.t[2] << std::endl;
      std::cout << "R:" << result_.r[0]<< " " << result_.r[1] << " " << result_.r[2] << std::endl;
      std::cout << "=========== END POSE ===========" << std::endl;

      if(raspi_gpgpu::RaspiConfig::Instance()->_allFrames)
      {
         cv::imshow("Display filter ID", pfilter3_->dst);
         key = cv::waitKey(0);
         if(key == 27)
           break;
      }
      

      

      
         
       // mtx_.unlock();
      index++;
	}
}

/**
 * Stop this sensor
 */
void dsa::LocalizationFunction::stop() {
	stopped_ = true;
	delete filterChain_;
   if(phub_ != NULL)
    delete (phub_);
}

bool dsa::LocalizationFunction::calibrate() {
	return(true);
}

dsa::SensorfunctionResult* dsa::LocalizationFunction::getResult() {
    //mtx_.lock();
	LocalizationResult *result = new LocalizationResult(result_);
   // mtx_.unlock();
	return(result);
}

dsa::SensorfunctionResult* dsa::LocalizationFunction::getLastValid() {
	//mtx_.lock();
	LocalizationResult *result = result_.isValid ? new LocalizationResult(result_) : new LocalizationResult(lastValid_);
	//mtx_.unlock();
	return(result);
}
