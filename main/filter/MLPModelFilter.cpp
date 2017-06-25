/*
 * MLPModelFilter.cpp
 *
 * 	Multi-Layer Perceptron model
 *
 *  Created on: 28.09.2012
 *      Author: jung
 */

#include "MLPModelFilter.h"

/**
 * Constructor
 *
 */
dsa::MLPModelFilter::MLPModelFilter(const char* filename, const char* name) : StatModelFilter(filename, name) {
	type_ = MLType::MLP;
	mlp_.load(filename, name);

	init();
}

/**
 * Destructor
 *
 * Save the settings.
 */
dsa::MLPModelFilter::~MLPModelFilter() {
	// TODO: add newly learned examples to training data when finished
	//if(learnedNew_) {
	// update weights: not yet implemented
	//cv::Mat sampleWeights(1, newInputs.cols, CV_32FC1, cv::Scalar::all(1));
	//mlp_.train(newInputs, newOutputs, sampleWeights, cv::Mat(), CvANN_MLP_TrainParams(), UPDATE_WEIGHTS);
	// save new mlp
	//mlp_.save(filename_, name_);
	//}
}

/**
 * Run predict() on the MLP
 */
void dsa::MLPModelFilter::predict() {
	cv::Mat output;
	mlp_.predict(*descriptorValues, output);
	for(size_t i = 0; i < descriptorValues->rows; i++) {
		results.push_back(GenericMLResult(type_, output.row(i)));
	}

	// alternative: one at a time
	/*for(size_t i = 0; i < descriptorValues->rows; i++) {
		cv::Mat output;
		mlp_.predict(descriptorValues->row(i), output);
		results.push_back(GenericMLResult(type_, output));
	}*/
}
