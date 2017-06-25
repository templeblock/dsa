/*
 * ForkObserver.cpp
 *
 *  Created on: 31.07.2012
 *      Author: jung
 */

#include "ForkObserver.h"

dsa::ForkObserver::ForkObserver(VideoHub* dataHub, ForkAllocationSensorInitializer* fasi) : dsa::Sensorfunction(SensorType::ForkOccupied) {
	fasi_ = fasi;
	hub_ = dataHub;
	isReady_ = false;

	init();
}

dsa::ForkObserver::~ForkObserver() {
	delete filterChain_;
	delete finfo_;
}

void dsa::ForkObserver::init() {
	std::vector<cv::Rect> rois = fasi_->getROIs();
	std::vector<std::vector<cv::Point> > forks = fasi_->getForks();
	std::vector<cv::Mat> forkMasks = fasi_->getMasks();
	std::vector<cv::Mat> forkReferences = fasi_->getReferences();
	std::vector<cv::Mat> forkHistograms = fasi_->getHistograms();
	std::vector<std::vector<size_t> *> maskedPixelsPerLine = fasi_->getMaskedPixelsPerLine();

	GrabImageFilter		*filter0 = new GrabImageFilter(*hub_);	// convert to grayscale
	RotationFilter		*filter1 = new RotationFilter(0, cv::Size(816, 686));
	//filter1->setActive(false);
	//SubtractMeanFilter	*filter2 = new SubtractMeanFilter();
	//filter2->setActive(false);
	CamshiftFilter *filter2 = new CamshiftFilter(forkHistograms, rois);
	filter2->setActive(false);

	RoiFilter *filter21 = new RoiFilter(rois[0]);
	RoiFilter *filter22 = new RoiFilter(rois[1]);
	MaskFilter *filter31 = new MaskFilter(forkMasks[0], cv::Scalar(0));
	MaskFilter *filter32 = new MaskFilter(forkMasks[1], cv::Scalar(0));
	EqualizeHistFilter *filter41 = new EqualizeHistFilter();
	filter41->setActive(false);
	EqualizeHistFilter *filter42 = new EqualizeHistFilter();
	filter42->setActive(false);
	SubtractReferenceFilter *filter51 = new SubtractReferenceFilter(forkReferences[0], forkMasks[0]);
	SubtractReferenceFilter *filter52 = new SubtractReferenceFilter(forkReferences[1], forkMasks[1]);

	CustomFilter *filter61 = new CustomFilter("changes1", contourArea(forks[0]), maskedPixelsPerLine[0], forkMasks[0]);
	CustomFilter *filter62 = new CustomFilter("changes2", contourArea(forks[1]), maskedPixelsPerLine[1], forkMasks[1]);
	finfo_ = new ForkInfo(filter61->state, filter62->state); // finfo_->getState(); returns the current status of the forks

	CvtColorFilter		*filter71 = new CvtColorFilter(CV_GRAY2RGB);	// convert to color
	filter71->setActive(false);
	CvtColorFilter		*filter72 = new CvtColorFilter(CV_GRAY2RGB);	// convert to color
	filter71->setActive(false);

	cv::Mat fork0Mat(forks[0]);
	cv::Mat fork1Mat(forks[1]);
	std::vector<cv::Mat> fork0Polyline = std::vector<cv::Mat>();
	std::vector<cv::Mat> fork1Polyline = std::vector<cv::Mat>();
	fork0Polyline.push_back(fork0Mat);
	fork1Polyline.push_back(fork1Mat);
	DrawPolylineFilter *filter81 = new DrawPolylineFilter(fork0Polyline, cv::Scalar(0,255,0));
	filter81->setActive(false);
	DrawPolylineFilter *filter82 = new DrawPolylineFilter(fork1Polyline, cv::Scalar(0,255,0));
	filter82->setActive(false);

	ImageOutputFilter	*filter91 = new ImageOutputFilter("Fork1");	// show video
	//filter91->setActive(false);
	ImageOutputFilter	*filter92 = new ImageOutputFilter("Fork2");	// show video
	//filter92->setActive(false);
	CvtColorFilter		*filter10 = new CvtColorFilter(CV_GRAY2RGB); //überflüssig
	//filter10->setActive(false);
	PutTextFilter		*filter11 = new PutTextFilter(finfo_->text, cv::Point(10, 60), cv::FONT_HERSHEY_COMPLEX, 2.0, cv::Scalar(0,255,0));
	ImageOutputFilter	*filter13 = new ImageOutputFilter("Gabelansicht");	// show video
	//filter12->setActive(false);
	WaitFilter			*filter14 = new WaitFilter(15.0f);	// wait assuming 15fps

	// show video again, this time with the mask and only inside the roi
	filter1->src = &(filter0->dst);
	filter2->src = &(filter1->dst);
	filter21->src = &(filter2->dst);
	filter31->src = &(filter21->dst);
	filter41->src = &(filter31->dst);
	filter51->src = &(filter41->dst);
	filter61->src = &(filter51->dst);
	filter71->src = &(filter61->dst);
	filter81->src = &(filter71->dst);
	//filter811->src = &(filter81->dst);
	filter91->src = &(filter81->dst);

	filter22->src = &(filter2->dst);
	filter32->src = &(filter22->dst);
	filter42->src = &(filter32->dst);
	filter52->src = &(filter42->dst);
	filter62->src = &(filter52->dst);
	filter72->src = &(filter62->dst);
	filter82->src = &(filter72->dst);
	//filter821->src = &(filter82->dst);
	filter92->src = &(filter82->dst);

	filter10->src = &(filter2->dst);
	filter11->src = &(filter10->dst);
	filter11->text = &(finfo_->text);
	//filter12->src = &(filter11->dst);
	filter13->src = &(filter11->dst);
	filter14->src = &(filter13->dst);

	// list of the filters for phase 3
	std::list<Filter*> *filterList = new std::list<Filter*>();
	// add filters to list
	filterList->push_back(filter0);
	filterList->push_back(filter1);
	filterList->push_back(filter2);
	filterList->push_back(filter21);
	filterList->push_back(filter22);
	filterList->push_back(filter31);
	filterList->push_back(filter32);
	filterList->push_back(filter41);
	filterList->push_back(filter42);
	filterList->push_back(filter51);
	filterList->push_back(filter52);
	filterList->push_back(filter61);
	filterList->push_back(filter62);
	filterList->push_back(filter71);
	filterList->push_back(filter72);
	filterList->push_back(filter81);
	filterList->push_back(filter82);
	filterList->push_back(filter91);
	filterList->push_back(filter92);
	filterList->push_back(filter10);
	filterList->push_back(filter11);
	filterList->push_back(filter13);
	filterList->push_back(filter14);

	// create filterchain for phase 3
	filterChain_ = new FilterChain(filterList, true);

	isReady_ = true;
}

bool dsa::ForkObserver::calibrate() {
	return(true);
}

void dsa::ForkObserver::run() {
	filterChain_->apply();	// apply all filters in chain
	finfo_->getState();
}

bool dsa::ForkObserver::isReady() {
	return(isReady_);
}

void dsa::ForkObserver::reset() {
}

void dsa::ForkObserver::setInitializer(ForkAllocationSensorInitializer* fasi) {
	fasi_ = fasi;	// add new

	std::vector<cv::Rect> rois = fasi_->getROIs();
	std::vector<std::vector<cv::Point> > forks = fasi_->getForks();
	std::vector<cv::Mat> forkMasks = fasi_->getMasks();
	std::vector<cv::Mat> forkReferences = fasi_->getReferences();
	std::vector<cv::Mat> forkHistograms = fasi_->getHistograms();
	std::vector<std::vector<size_t> *> maskedPixelsPerLine = fasi_->getMaskedPixelsPerLine();

	/**
	 * propagate info above to the filters:
	 *
	 * 3. Filter: RoiFilter->(*rois_)[0]
	 * 4. Filter: RoiFilter->(*rois_)[1]
	 * 5. Filter: MaskFilter->(*forkMasks_)[0]
	 * 6. Filter: MaskFilter->(*forkMasks_)[1]
	 * 9. Filter: SubtractReferenceFilter->(*forkReferences_)[0]
	 * 10. Filter: SubtractReferenceFilter->(*forkReferences_)[1]
	 * 11. Filter: CustomFilter->contourArea((*forks_)[0]), (*maskedPixelsPerLine_)[0]
	 * 12. Filter: CustomFilter->contourArea((*forks_)[1]), (*maskedPixelsPerLine_)[1]

	 * 15. Filter: DrawPolylineFilter->fork0Polyline
	 * 16. Filter: DrawPolylineFilter->fork1Polyline
	 */

	std::list<Filter*>::iterator it = filterChain_->getChain()->begin(); // ->0
	it++; // ->1
	it++; // ->2
	it++; // ->3
	((RoiFilter*)(*it))->setRoi(rois[0]);
	it++; // ->4
	((RoiFilter*)(*it))->setRoi(rois[1]);
	it++; // ->5
	((MaskFilter*)(*it))->setMask(forkMasks[0]);
	it++; // ->6
	((MaskFilter*)(*it))->setMask(forkMasks[1]);
	it++; // ->7
	it++; // ->8
	it++; // ->9
	((SubtractReferenceFilter*)(*it))->setReference(forkReferences[0]);
	it++; // ->10
	((SubtractReferenceFilter*)(*it))->setReference(forkReferences[1]);
	it++; // ->11
	((CustomFilter*)(*it))->update(contourArea(forks[0]), maskedPixelsPerLine[0], forkMasks[0]);
	it++; // ->12
	((CustomFilter*)(*it))->update(contourArea(forks[0]), maskedPixelsPerLine[1], forkMasks[1]);
	it++; // ->13
	it++; // ->14
	it++; // ->15

	// change format for polyline
	cv::Mat fork0Mat(forks[0]);
	cv::Mat fork1Mat(forks[1]);
	std::vector<cv::Mat> fork0Polyline = std::vector<cv::Mat>();
	std::vector<cv::Mat> fork1Polyline = std::vector<cv::Mat>();
	fork0Polyline.push_back(fork0Mat);
	fork1Polyline.push_back(fork1Mat);
	// and update them in the drawlinesFilter
	((DrawPolylineFilter*)(*it))->setPolylines(fork0Polyline);
	it++;
	((DrawPolylineFilter*)(*it))->setPolylines(fork1Polyline);
}

dsa::ForkInfo* dsa::ForkObserver::getForkInfo() {
	return(finfo_);
}
