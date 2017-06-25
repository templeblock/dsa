/*
 * Settings.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>
#include <aruco/cameraparameters.h>
//#include <opencv2/core/types_c.h>
#include <MultiMarkerStrategy.hpp>
#include <PoseEstimationAlgorithm.hpp>
#include <RegulationStrategy.hpp>

#include <fstream>

#include <demonstrator/localizor/utils3d.hpp>

using namespace std;

/*
 * Reads the settings (settings.yml) and the
 * camera parameters (camparams.txt) file and
 * stores their values.
 */
class Settings {
private:
	static const string defaultCamparamsFilename;
	static const string settingsFilename;
	static const string defaultConfigFolder;
	static const string defaultForkliftConfigFilename;
	String configFolder;


public:
	int camIndex; // -1 means first available
	aruco::CameraParameters camParameters;
	bool flipVertical;
	bool flipHorizontal;
	bool rotateImage;
	MultiMarkerStrategy multiMarkerStrategy;
	RegulationStrategy regulationStrategy;
	PoseEstimationAlgorithm poseEstimationAlgorithm;
	float exponentialSmoothingAlpha;
	bool invertYAxis;
	float detectionThresholdBlockSize;
	float detectionThresholdSubtraction;
	float detectionMinMarker;
	float detectionMaxMarker;
	string camParamsFilename;
	string forkliftConfigFilename;

	cv::Point2f rotationMarkerPosition;
	cv::Point2f rotationAxisPosition;
	cv::Point2f cameraPosition;
	float heightMarkerOffset;
	float forkTipDistance;
	float initialCameraRotation;
	float cameraXRotation;
	double rotationOffset;
	int heightMode;
	double heightOffset;
	double heightFormulaFactor;
	double heightFormulaExponent;

	double x_markerMin;
	double x_markerMax;
	double y_markerMin;
	double y_markerMax;
	double fullTiltAngle;
	double zeroTiltAngleOffset;

	Settings() {
		this->configFolder = defaultConfigFolder;
		flipVertical = true;
		flipHorizontal = false;
		readSettingsFile();
	}

	Settings(String configFolder) {
		this->configFolder = configFolder;
		flipVertical = true;
		flipHorizontal = false;
		readSettingsFile();
	}

	void readSettingsFile() {
		string filename = configFolder + "/" + settingsFilename;
		FileStorage fs(filename, FileStorage::READ);
		multiMarkerStrategy = (MultiMarkerStrategy)(int)fs["multiMarkerStrategy"];
		regulationStrategy = (RegulationStrategy)(int)fs["regulationStrategy"];
		poseEstimationAlgorithm = (PoseEstimationAlgorithm)(int)fs["poseEstimationAlgorithm"];
		flipVertical = (int)fs["flipVertical"]==1;
		flipHorizontal = (int)fs["flipHorizontal"]==1;
		rotateImage = (int)fs["rotateImage"]==1;
		invertYAxis = (int)fs["invertYAxis"]==1;
		exponentialSmoothingAlpha = (float)fs["exponentialSmoothingAlpha"];
		detectionThresholdBlockSize = (float)fs["detectionThresholdBlockSize"];
		detectionThresholdSubtraction = (float)fs["detectionThresholdSubtraction"];
		detectionMinMarker = (float)fs["detectionMinMarker"];
		detectionMaxMarker = (float)fs["detectionMaxMarker"];
		if(fs["camParamsFile"].empty()) {
			camParamsFilename = defaultCamparamsFilename;
		} else {
			camParamsFilename = (string)fs["camParamsFile"];
		}
		if(fs["forkliftConfigFile"].empty()) {
			forkliftConfigFilename = defaultForkliftConfigFilename;
		} else {
			forkliftConfigFilename = (string)fs["forkliftConfigFile"];
		}
		fs.release();
	}

	void readCameraParameters() {
		this->camParameters.readFromFile(configFolder + "/" + camParamsFilename);
	}

	void readCameraParameters(Size & size) {
		readCameraParameters();
		camParameters.resize(size);
		camParameters.CamSize = size;
	}

	void readForkliftConfig() {
		string filename = configFolder + "/" + forkliftConfigFilename;
		FileStorage fs(filename, FileStorage::READ);
		rotationMarkerPosition =
				Point2f((float)fs["rotationMarkerX"], (float)fs["rotationMarkerY"]);
		rotationAxisPosition =
				Point2f((float)fs["rotationAxisX"], (float)fs["rotationAxisY"]);
		cameraPosition =
				Point2f((float)fs["cameraX"], (float)fs["cameraY"]);
		heightMarkerOffset = (float)fs["heightMarkerOffset"];
		forkTipDistance = (float)fs["forkTipDistance"];
		initialCameraRotation = (float)fs["initialCameraRotation"];
		cameraXRotation = (float)fs["cameraXRotation"];
		rotationOffset = (double)fs["rotationOffset"];
		heightOffset = (double)fs["heightOffset"];
		heightMode = (int)fs["heightMode"];

		x_markerMin = (double)fs["x_markerMin"];
		x_markerMax = (double)fs["x_markerMax"];
		y_markerMin = (double)fs["y_markerMin"];
		y_markerMax = (double)fs["y_markerMax"];
		fullTiltAngle = (double)fs["fullTiltAngle"];
		zeroTiltAngleOffset = (double)fs["zeroTiltAngleOffset"];

		heightFormulaFactor = (double)fs["heightFormulaFactor"];;
		heightFormulaExponent = (double)fs["heightFormulaExponent"];;

		fs.release();
	}

	cv::Size camResoultion() {
		return camParameters.CamSize;
	}

	void intrinsics(Mat & K) {
		K = camParameters.CameraMatrix;
	}

	void load() {
		readCameraParameters();
		readForkliftConfig();
	}

	void load(Size camReso) {
		camIndex = 1; // -1 means first available
		Size s = Size(camReso.width, camReso.height);
		readCameraParameters(s);
		readForkliftConfig();
	}
};

const string Settings::defaultCamparamsFilename = "camparams.txt";
const string Settings::settingsFilename = "settings.yml";
const string Settings::defaultConfigFolder = "config/default";
const string Settings::defaultForkliftConfigFilename = "forklift.yml";


#endif /* SETTINGS_HPP_ */
