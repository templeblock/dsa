/*
 * SmartekStrobeController.cpp
 *
 *  Created on: 21.10.2013
 *      Author: jung
 */

#include "StrobeController.h"

/**
 * Just a datastructure to hold relevant information on output current
 *
 * @param amperage in A
 * @param duration in ms
 * @param voltage in V
 */
dsa::LampEnergy::LampEnergy(double amperage, std::size_t duration, double voltage) {
	this->duration = duration;
	this->amperage = amperage;
	this->voltage = voltage;
}

/**
 * Current is potentially dangerous if one of the values exceeds the designated maximum
 * @param rhs
 * @return
 */
bool dsa::LampEnergy::operator>(LampEnergy const &rhs) const {
	return(this->amperage > rhs.amperage || this->duration > rhs.duration || this->voltage > rhs.voltage );
}

/**
 * Current is potentially dangerous if one of the values exceeds the designated maximum
 * @param rhs
 * @return
 */
bool dsa::LampEnergy::operator>=(LampEnergy const &rhs) const {
	return(this->amperage >= rhs.amperage || this->duration >= rhs.duration || this->voltage >= rhs.voltage);
}

/**
 * Current is only harmless if ALL of the values are below specification (the specified values correctness is assumed)
 *
 * @param rhs
 * @return
 */
bool dsa::LampEnergy::operator<(LampEnergy const &rhs) const {
	return(this->amperage < rhs.amperage && this->duration < rhs.duration && this->voltage < rhs.voltage);
}

/**
 * Current is only harmless if ALL of the values are below or equal specification (the specified values correctness is assumed)
 *
 * @param rhs
 * @return
 */
bool dsa::LampEnergy::operator<=(LampEnergy const &rhs) const {
	return(this->amperage <= rhs.amperage && this->duration <= rhs.duration && this->voltage <= rhs.voltage);
}

dsa::StrobeController::StrobeController() {
	maxNorm_ = std::vector<LampEnergy>();
	maxFlash_ = std::vector<LampEnergy>();
}

/**
 * Define maximum current details for all the outputs
 *
 * @param maxNorm for continuous use
 * @param maxFlash for flash use
 */
dsa::StrobeController::StrobeController(std::vector<LampEnergy> &maxNorm, std::vector<LampEnergy> &maxFlash) {
	// make sure all values are defined
	assert(maxNorm.size() == maxFlash.size());
	on_ = std::vector<bool>(maxNorm.size(), false);
	maxNorm_ = maxNorm;
	maxFlash_ = maxFlash;
}

dsa::StrobeController::~StrobeController() {
}

/**
 * Make sure the current does not exceed user specifications continuous mode for each output
 *
 * @param output port
 * @param energy current details
 */
void dsa::StrobeController::assertNormOverload(std::size_t output, LampEnergy &energy) {
	assert(output < maxNorm_.size() && energy <= maxNorm_[output]);
}

/**
 * Make sure the current does not exceed user specifications flash mode for each output
 *
 * @param output port
 * @param energy current details
 */
void dsa::StrobeController::assertFlashOverload(std::size_t output, LampEnergy &energy) {
	assert(output < maxFlash_.size() && energy <= maxFlash_[output]);
}

/**
 * Tell whether the port was activated. This doesn't necessarily mean it is actually on, but shows whether we tried to turn it on
 *
 * @param output port
 *
 * @return true if it was activated.
 */
bool dsa::StrobeController::isOn(std::size_t output) {
	return((output < on_.size()) ? on_[output] : false);
}

/**
 * Start continuous mode (=supplying current on port output)
 *
 * @param output port
 * @param energy current details
 *
 * @return true if successful
 */
bool dsa::StrobeController::startContinuous(std::size_t output, LampEnergy &energy) {
	assertNormOverload(output, energy);
	on_[output] = true;
	return(true);
}

/**
 * Start continuous mode with max current values;
 *
 * @param output port
 * @return true if successful
 */
bool dsa::StrobeController::startContinuous(std::size_t output) {
	LampEnergy energy = LampEnergy(0, 0, 0);
	return(startContinuous(output, energy));
}

/**
 * Start continuous mode (=supplying current on port output)
 *
 * @param output port
 * @param amperage current details
 * @param voltage current details
 *
 * @return true if successful
 */
bool dsa::StrobeController::startContinuous(std::size_t output, double amperage, double voltage) {
	LampEnergy energy = LampEnergy(amperage, 0, voltage);
	return(startContinuous(output, energy));
}

/**
 * Stop continuous mode on port output
 *
 * @param output port
 *
 * @return true if successful
 */
bool dsa::StrobeController::stopContinuous(std::size_t output) {
	assert(output < maxNorm_.size());
	on_[output] = false;
	return(true);
}

/**
 * Perform one flash on port output with max current values
 * @param output port
 *
 * @return true if successful
 */
bool dsa::StrobeController::flash(std::size_t output) {
	LampEnergy energy = LampEnergy(0, 0, 0);
	return(flash(output, energy));
}

/**
 * Perform one flash on port output with given current details
 * @param output port
 * @param energy current details
 *
 * @return true if successful
 */
bool dsa::StrobeController::flash(std::size_t output, LampEnergy &energy) {
	assertFlashOverload(output, energy);
	return(true);
}

/**
 * Perform one flash on port output with given current details
 *
 * @param output port
 * @param amperage current details
 * @param duration current details
 * @param voltage current details
 *
 * @return true if successful
 */
bool dsa::StrobeController::flash(std::size_t output, double amperage, std::size_t duration, double voltage) {
	LampEnergy energy = LampEnergy(amperage, duration, voltage);
	return(flash(output, energy));
}
