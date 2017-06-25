/*
 * ForkHeightFunction.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "ForkHeightFunction.h"

#ifdef WITH_RASPICAM
dsa::ForkHeightFunction::ForkHeightFunction(raspicam::RaspiCam_Cv* capture, double fps, int exposure, int width, int height, double rotation, double gain) : 
#else
dsa::ForkHeightFunction::ForkHeightFunction(cv::VideoCapture *capture, double fps, int exposure, int width, int height, double rotation, double gain) : 
#endif
	dsa::Sensorfunction(SensorType::Localization) {
	capture_ = capture;
	fps_ = fps;
	exposure_ = exposure;
	width_ = width;
	height_ = height;
	rotation_ = rotation;
	gain_ = gain;

	stopped_ = false;

	result_ = ForkHeightResult();
}

dsa::ForkHeightFunction::~ForkHeightFunction() {
}

/**
 * die relevanten Stellschrauben für mehr Performance in diesem Demonstrator sind:
 * - geringere Auflösung (Skalierung)
 * - UnsharpMaskFilter deaktivieren (bringt relativ wenig)
 * - UndistortFilter mit interpolation=INTER_NEAREST verwenden und blur() auf den Bildkandidaten ausführen (ob das funktioniert)
 * - UndistortFilter weiter verbessern (unwahrscheinlich) oder ganz eliminieren (siehe Wiki)
 * - FindMarkerFilter weiter verbessern (unwahrscheinlich)
 * - Ausgabe deaktivieren (Filter 8-11), falls gerade nicht benötigt
 */
void dsa::ForkHeightFunction::run() {
	// configure camera
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
	std::string ausgabe_vid = std::string("ausgabe_fh.avi");

	// filter chain
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// filter chain to execute
	GrabImageFilter		*filter0 = new GrabImageFilter(hub);
	RotationFilter		*filter1 = new RotationFilter(rotation_, imageSize);	// entfernen, stattdessen das Ergebnis um 180° rotieren (Rotationswinkel ist nur eine Zahl, hier müssen wir sonst jeden Pixel rotieren)
	EmptyMaskFilter		*filter2 = new EmptyMaskFilter();
	UndistortFilter		*filter3 = new UndistortFilter(camparams.CameraMatrix, camparams.Distorsion, cv::Mat(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(255));
	AutoWhiteBalanceFilter *filter4 = new AutoWhiteBalanceFilter();
	FindMarkersFilter	*filter5 = new FindMarkersFilter(settings);
	SplitMarkersByIdFilter	*filter6 = new SplitMarkersByIdFilter(config);
	EstimatePoseFilter	*filter7 = new EstimatePoseFilter(settings, 4, 0, 1, 2);
	EmjaysFancyForkPoseFilter *filter7_1 = new EmjaysFancyForkPoseFilter(settings);
	MarkerAOIFilter			*filter8 = new MarkerAOIFilter(1.2f);
	std::vector<Circle> circles;
	DrawCircleFilter		*filter9 = new DrawCircleFilter(circles, cv::Scalar(1));	// important: only draw 1, so AutoExposure can perform much faster
	RoiFilter				*filter10_1 = new RoiFilter(cv::Rect(0, 0, 0, 0));
	RoiFilter				*filter10_2 = new RoiFilter(cv::Rect(0, 0, 0, 0));
	AutoExposureAlgorithm* AE = new HistAutoExposureAlgorithm(capture_, 1000.0, 30000.0, CV_CAP_PROP_EXPOSURE, 5, 5);
	AutoSensorFilter    *filter11 = new AutoSensorFilter(AE);
	filter11->setActive(autoexposure);
	ResizeFilter		*filter12 = new ResizeFilter(resizedSize, cv::INTER_NEAREST);
	//VideoWriterFilter	*filter13 = new VideoWriterFilter(ausgabe_vid, CV_FOURCC('X', 'V', 'I', 'D'), fps_, false);
	ImageOutputFilter	*filter13 = new ImageOutputFilter("Ausgabe Hubhöhe");
	WaitFilter			*filter14 = new WaitFilter((float)fps_);	// wait for desired framerate

	filter1->src = &(filter0->dst);
	filter2->src = &(filter1->dst);
	filter3->src = &(filter1->dst);
	filter4->src = &(filter3->dst);
	filter5->src = &(filter4->dst);
	filter6->src = &(filter5->dst);
	filter7->src = &(filter6->dst);
	filter7_1->src = &(filter7->dst);
	filter8->src = &(filter7_1->dst);
	filter9->src = &(filter2->dst);
	filter10_1->src = &(filter3->dst);
	filter10_2->src = &(filter9->dst);
	filter11->src = &(filter10_1->dst);
	filter11->mask = &(filter10_2->dst);
	filter12->src = &(filter4->dst);
	filter13->src = &(filter12->dst);
	filter14->src = &(filter13->dst);

	filter6->markers = &(filter5->markers);
	filter7->markers = &(filter6->height_markers);
	filter7->poses = &(filter6->height_poses);
	filter7_1->measuredHeightMarkerPosition = &(filter7->camPosition);
	filter7_1->markers = &(filter6->rotation_markers);
	filter8->markers = &(filter6->height_markers);
	filter9->circles = &(filter8->aois);
	filter10_1->roi = &(filter8->cutoff);
	filter10_2->roi = &(filter8->cutoff);
	filter11->usemask = &(filter8->haveMarker);

	filterList->push_back(filter0);
	filterList->push_back(filter1);
	filterList->push_back(filter2);
	filterList->push_back(filter3);
	filterList->push_back(filter4);
	filterList->push_back(filter5);
	filterList->push_back(filter6);
	filterList->push_back(filter7);
	filterList->push_back(filter7_1);
	filterList->push_back(filter8);
	filterList->push_back(filter9);
	filterList->push_back(filter10_1);
	filterList->push_back(filter10_2);
	filterList->push_back(filter11);
	filterList->push_back(filter12);
	filterList->push_back(filter13);
	filterList->push_back(filter14);

	FilterChain chain(filterList, true);

	while(!stopped_) {	// loop forever
		chain.apply();	// apply all filters in chain
		cv::waitKey(1);
        mtx_.lock();
		if(filter7->poses->size() > 0) {
			result_.isValid = true;
			result_.timestamp = filter0->dataTimestamp;
			result_.h = filter7_1->forkHeight;
			result_.r = filter7_1->forkRotation;
			result_.endX = filter7_1->forkEndPosition.x;
			result_.endZ = filter7_1->forkEndPosition.y;
			// update lastValid_
			lastValid_ = ForkHeightResult(result_);
		} else {
			result_.isValid = false;
		}
        mtx_.unlock();
	}
}

/**
 * Stop this sensor
 */
void dsa::ForkHeightFunction::stop() {
	stopped_ = true;
}

bool dsa::ForkHeightFunction::calibrate() {
	return(true);
}

dsa::SensorfunctionResult* dsa::ForkHeightFunction::getResult() {
    mtx_.lock();
	ForkHeightResult *result = new ForkHeightResult(result_);
    mtx_.unlock();
	return(result);
}

dsa::SensorfunctionResult* dsa::ForkHeightFunction::getLastValid() {
	mtx_.lock();
	ForkHeightResult *result = result_.isValid ? new ForkHeightResult(result_) : new ForkHeightResult(lastValid_);
	mtx_.unlock();
	return(result);
}
