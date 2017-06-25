/*
 * raspi_config.h
 *
 *  Created on: 29 mar 2016
 *      Author: gregStudent
 */

#ifndef RASPI_CONFIG_H_
#define RASPI_CONFIG_H_

#include <stdlib.h>
#include <string>
#include <opencv2/core/persistence.hpp>

namespace raspi_gpgpu
{
	class RaspiConfig
	{
	private:
		static RaspiConfig *_instance;	


	public:

		bool IsColor;
		float Width ;
		float Height;
		float _lowerCannyParam;
		float _upperCannyParam;
      float _lowerCannyThresholdBW;
		float _lowerCannyThresholdC;
		float _upperCannyThresholdBW;
		float _upperCannyThresholdC;

      int _houghTreshold;
      int _uHoughTreshold;
      int _l2houghTreshold;
      int _u2HoughTreshold;

      int _lSmallSquare;
      int _uSmallSquare;

      int _angle;
      bool _allFrames;
      int _workMode;
      int _imageNr;
      int _pipeline;
      int _lineFineProcessing;
      int _distanceFilter;
      int _angleFilter;


      // output
      int _houghHeight;

      // canera
      float _gain;
      int _exposure;
      int _brightness;
      int _contrast;
      

      // markers specific data
      int _lastConfigRead;
      int _lastThres;
      bool _fileLoaded;
      bool _lastFound;
      int _lHoughThreshold_t1;
      int _hHoughThreshold_t1;
      int _l2HoughThreshold_t1;
      int _h2HoughThreshold_t1;
      int _lPDistance_t1;
      int _uPDistance_t1;
      float _lowerCannyParam_t1n;
		float _upperCannyParam_t1n;
      float _lowerCannyParam_t1a;
		float _upperCannyParam_t1a;

      int _lHoughThreshold_t2;
      int _hHoughThreshold_t2;
      int _l2HoughThreshold_t2;
      int _h2HoughThreshold_t2;
      int _lPDistance_t2;
      int _uPDistance_t2;
      float _lowerCannyParam_t2n;
		float _upperCannyParam_t2n;
      float _lowerCannyParam_t2a;
		float _upperCannyParam_t2a;

      int _lHoughThreshold_t3;
      int _hHoughThreshold_t3;
      int _l2HoughThreshold_t3;
      int _h2HoughThreshold_t3;
      int _lPDistance_t3;
      int _uPDistance_t3;
      float _lowerCannyParam_t3n;
		float _upperCannyParam_t3n;
      float _lowerCannyParam_t3a;
		float _upperCannyParam_t3a;
      // end of markers specific data

		RaspiConfig(){}
		~RaspiConfig(){}

		static RaspiConfig *Instance();
		static void Destroy();

		float GetLowerCannyThreshold();
		float GetUpperCannyThreshold();

      static void LoadConfig(int type = 0);
      static void SetFound(bool found);
      static int GetHoughHeight();


	};
}


#endif /* RASPI_CONFIG_H_ */
