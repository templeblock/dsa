/*
 * CamshiftFilter.cpp
 *
 * 	Base class for custom image filters, that perform actions defined by the user
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "CamshiftFilter.h"

/**
 * Constructor
 *
 */
dsa::CamshiftFilter::CamshiftFilter(std::vector<cv::Mat> &histograms, std::vector<cv::Rect> &rois) {
	histograms_ = histograms;
	rois_ = rois;

	init();
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::CamshiftFilter::~CamshiftFilter() {
}

/**
 * We use public pointers to the real arguments when running the OpenCV function, which may look stupid at first. But this allows
 * a user to direct the pointer to any preceding or even succeeding filters output and thus adjust the input parameters to this
 * filter automatically without having to write any further code for runtime behavior. In order for the filter to work with the
 * default values, we have to map the pointers to those default values, which happen to be private. We could of course make those
 * values also public, but then again, we only want to allow changes the way they are supposed to be changed to avoid confusion
 * and mistakes, just as we would do in case of only exporting the values via getter and setter methods.
 */
void dsa::CamshiftFilter::init() {
	EvaluationFilter::init();
}

/**
 * Add to training data
 *
 */
void dsa::CamshiftFilter::evaluate() {
	const float histRange[] = {0.0f, 255.0f};
    const float* phistRange = histRange;

	cv::Mat bp1, bp2;
	cv::Rect trackWindow1 = rois_[0];
	cv::Rect trackWindow2 = rois_[1];

	//std::cerr << "Begin Backprojection" << std::endl;
	calcBackProject(src, 1, 0, histograms_[0], bp1, &phistRange);
	calcBackProject(src, 1, 0, histograms_[1], bp2, &phistRange);

	/*cv::RotatedRect trackBox1 = CamShift(bp1, trackWindow1, cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));
	cv::RotatedRect trackBox2 = CamShift(bp2, trackWindow2, cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));

    if(trackWindow1.area() <= 1) {
         int cols = bp1.cols;
         int rows = bp1.rows;
         int r = (MIN(cols, rows) + 5)/6;
         trackWindow1 = cv::Rect(trackWindow1.x - r, trackWindow1.y - r, trackWindow1.x + r, trackWindow1.y + r) & cv::Rect(0, 0, cols, rows);
     }

    if(trackWindow2.area() <= 1) {
         int cols = bp2.cols;
         int rows = bp2.rows;
         int r = (MIN(cols, rows) + 5)/6;
         trackWindow2 = cv::Rect(trackWindow2.x - r, trackWindow2.y - r, trackWindow2.x + r, trackWindow2.y + r) & cv::Rect(0, 0, cols, rows);
     }

    cv::Mat output;
    cv::cvtColor(*src, output, CV_GRAY2RGB);
    cv::ellipse(output, trackBox1, cv::Scalar(0, 255, 0), 3, CV_AA );
    cv::ellipse(output, trackBox2, cv::Scalar(0, 0, 255), 3, CV_AA );

    imshow("output", output);*/

    imshow("Backprojection1", bp1);
    imshow("Backprojection2", bp2);
    //std::cerr << "End Backprojection" << std::endl;
}
