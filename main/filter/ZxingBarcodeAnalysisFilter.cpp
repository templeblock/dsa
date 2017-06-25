/*
 * ZXingBarcodeAnalysisFilter.h
 *
 *	This filter determines the contents of a extracted barcode image
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "ZxingBarcodeAnalysisFilter.h"

/**
 * Constructor
 */
dsa::ZxingBarcodeAnalysisFilter::ZxingBarcodeAnalysisFilter(zxing::DecodeHints &hints) {
	hints_ = hints;
	reader_ = zxing::MultiFormatReader();
	codes = std::vector<Barcode>();

	init();
}

/**
 * Destructor
 */
dsa::ZxingBarcodeAnalysisFilter::~ZxingBarcodeAnalysisFilter() {
}

void dsa::ZxingBarcodeAnalysisFilter::init() {
}

void dsa::ZxingBarcodeAnalysisFilter::preprocessing() {
	EvaluationFilter::preprocessing();
	codes.clear();
}

dsa::Barcode dsa::ZxingBarcodeAnalysisFilter::decode(cv::Mat &image) {
	try	{
		zxing::Ref<Mat2LuminanceSource> source(new Mat2LuminanceSource(image));
		zxing::Ref<zxing::Binarizer> binarizer(new zxing::GlobalHistogramBinarizer(source));
		zxing::Ref<zxing::BinaryBitmap> bitmap(new zxing::BinaryBitmap(binarizer));

		/*cv::Mat newImage = cv::Mat::zeros(image.rows, image.cols, image.type());
		zxing::Ref<zxing::BitMatrix> matrix = bitmap->getBlackMatrix();
		for(size_t i = 0, count = 0; i < image.rows; i++)
			for(size_t j = 0; j < image.cols; j++, count++)
				newImage.data[count] = ((UINT8)(matrix->get(j,i)))*255;
		imshow("newImage", newImage);
		cv::waitKey(0);*/

		zxing::Ref<zxing::Result> result(reader_.decode(bitmap, hints_));
		return(result.empty() ? Barcode() : Barcode(cv::RotatedRect(), result->getBarcodeFormat().value, result->getText()->getText()));
	}
	catch (zxing::Exception& e) {
		//std::cerr << "Error: " << e.what() << std::endl;
		return(Barcode());
	}
}

/**
 * Fill internal data structure with markers
 */
void dsa::ZxingBarcodeAnalysisFilter::evaluate() {
	if(candidates != NULL) {
		for(size_t i = 0; i < candidates->size(); i++) {
			Barcode bc = decode((*candidates)[i].image);
			cv::RotatedRect rrect = (*candidates)[i].rotatedRect;
			if(bc.format != zxing::BarcodeFormat::Value::NONE) {
				bc.rrect = rrect;
				codes.push_back(bc);
				//std::cerr << "Found Barcode (fmt:" << bc.format << ") " << bc.contents << std::endl;
			} else {
				// no barcode found, just for debugging purposes, TODO: remove when final
				ellipse(dst, rrect, cv::Scalar(128), 5);
			}
		}
	}
}
