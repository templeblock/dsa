/*
 * BarcodeFunction.cpp
 *
 *  Created on: 01.12.2013
 *      Author: jung
 */

#include "BarcodeFunction.h"

dsa::BarcodeFunction::BarcodeFunction(cv::VideoCapture *capture, double fps, int exposure, bool awb, double gain) : dsa::Sensorfunction(SensorType::CodeReader) {
	capture_ = capture;
	fps_ = fps;
	exposure_ = exposure;
	awb_ = awb;
	gain_ = gain;

	stopped_ = false;

	result_ = BarcodeResult();
}

dsa::BarcodeFunction::~BarcodeFunction() {
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
void dsa::BarcodeFunction::run() {
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

	// load settings
	// diese Einstellungen müssen noch zentralisiert werden
	std::string config = std::string("eval_smart_5M");
	Settings settings = Settings("config/"+config);
	settings.load();
	aruco::CameraParameters camparams = settings.camParameters;

	// where to put the video file
	std::string ausgabe_vid = std::string("ausgabe_bc.avi");

	// settings for zxing (only use CODE128 for now, target is NVE)
	//zxing::DecodeHints hints(zxing::DecodeHints::DEFAULT_HINT);
	zxing::DecodeHints hints = zxing::DecodeHints();
	hints.addFormat(zxing::BarcodeFormat(zxing::BarcodeFormat::CODE_128));
	//hints.addFormat(zxing::BarcodeFormat(zxing::BarcodeFormat::EAN_13));
	//hints.addFormat(zxing::BarcodeFormat(zxing::BarcodeFormat::DATA_MATRIX));
	//hints.addFormat(zxing::BarcodeFormat(zxing::BarcodeFormat::PDF_417));

	// filter chain
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// filter chain to execute
	GrabImageFilter		*filter0 = new GrabImageFilter(hub);
	UndistortFilter		*filter1 = new UndistortFilter(camparams.CameraMatrix, camparams.Distorsion, cv::Mat(), INTER_NEAREST, BORDER_CONSTANT, cv::Scalar(255));
	filter1->setActive(false);
	AutoWhiteBalanceFilter *filter2 = new AutoWhiteBalanceFilter();
	filter2->setActive(awb_);
	BarcodeSegmentationFilter	*filter3 = new BarcodeSegmentationFilter(8);	// create barcode segmentation
	ZxingBarcodeAnalysisFilter	*filter4 = new ZxingBarcodeAnalysisFilter(hints);
	CvtColorFilter		*filter5 = new CvtColorFilter(CV_GRAY2RGB);	// convert to rgb (do this later in chain)
	VisualizeBarcodeFilter *filter6 = new VisualizeBarcodeFilter(5);
	ResizeFilter		*filter7 = new ResizeFilter(0.25);
	VideoWriterFilter	*filter8 = new VideoWriterFilter(ausgabe_vid, CV_FOURCC('X', 'V', 'I', 'D'), fps_, true);
	ImageOutputFilter	*filter9 = new ImageOutputFilter("BarcodeReader");	// show video
	AutoExposureAlgorithm* AE = new MeanAutoExposureAlgorithm(capture_, 1000.0, 30000.0, CV_CAP_PROP_EXPOSURE, 5, 5);
	AutoSensorFilter    *filter10 = new AutoSensorFilter(AE);
	filter5->setActive(autoexposure);
	WaitFilter			*filter11 = new WaitFilter((float)fps_);	// wait for 15fps

	filter1->src = &(filter0->dst);	// filter1 is successor of hub and obtains the videostream
	filter2->src = &(filter1->dst);
	filter3->src = &(filter2->dst);
	filter3->src = &(filter2->dst);
	filter4->src = &(filter3->dst);
	filter5->src = &(filter4->dst);
	filter6->src = &(filter5->dst);
	filter7->src = &(filter6->dst);
	filter8->src = &(filter7->dst);
	filter9->src = &(filter8->dst);
	filter10->src = &(filter9->dst);
	filter11->src = &(filter10->dst);

	filter4->candidates = &(filter3->candidates);
	filter6->codes = &(filter4->codes);

	filterList->push_back(filter0);
	filterList->push_back(filter1);
	filterList->push_back(filter2);
	filterList->push_back(filter3);
	filterList->push_back(filter4);
	filterList->push_back(filter5);
	filterList->push_back(filter6);
	filterList->push_back(filter7);
	filterList->push_back(filter8);
	filterList->push_back(filter9);
	filterList->push_back(filter10);
	filterList->push_back(filter11);

	FilterChain chain(filterList, true);

	while(!stopped_) {	// loop forever
		chain.apply();	// apply all filters in chain
		cv::waitKey(1);
		if(filter4->codes.size() > 0) {	// only lock mutex when necessary
			mtx_.lock();
			for(size_t i = 0; i < filter4->codes.size(); i++) {
				result_.isValid = true;
				result_.barcodeEvents.push_back(BarcodeEvent(filter0->dataTimestamp, filter4->codes[i]));
			}
			mtx_.unlock();
		}
	}
}

/**
 * Stop this sensor
 */
void dsa::BarcodeFunction::stop() {
	stopped_ = true;
}

bool dsa::BarcodeFunction::calibrate() {
	return(true);
}

dsa::SensorfunctionResult* dsa::BarcodeFunction::getResult() {
	mtx_.lock();
	result_.timestamp = currentTimeOfDayMilliseconds();
	BarcodeResult *result = new BarcodeResult(result_);
	result_.isValid = false;
	result_.barcodeEvents.clear();
	mtx_.unlock();
	return(result);
}

dsa::SensorfunctionResult* dsa::BarcodeFunction::getLastValid() {
	return(getResult());
}
