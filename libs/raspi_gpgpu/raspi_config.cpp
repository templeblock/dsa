/*
 * raspi_config.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: gregStudent
 */

#ifndef RASPI_CONFIG_CPP_
#define RASPI_CONFIG_CPP_

#include "raspi_config.h"

raspi_gpgpu::RaspiConfig* raspi_gpgpu::RaspiConfig::_instance = NULL;

raspi_gpgpu::RaspiConfig* raspi_gpgpu::RaspiConfig::Instance() {
	 if (!raspi_gpgpu::RaspiConfig::_instance) {
		 raspi_gpgpu::RaspiConfig::_instance = new raspi_gpgpu::RaspiConfig();
       raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead = 0;
       raspi_gpgpu::RaspiConfig::_instance->_lastThres = 0;
       raspi_gpgpu::RaspiConfig::_instance->_fileLoaded = false;
       raspi_gpgpu::RaspiConfig::_instance->_houghHeight = -1;
       

       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = 0.1; // 0.15  0.05;
		 raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdC = 0.09;
		 raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = 0.26; // 0.195;
		 raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdC = 0.19;
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam = 0.0;
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam = 0.0;

       raspi_gpgpu::RaspiConfig::_instance->_houghTreshold = 60;
       raspi_gpgpu::RaspiConfig::_instance->_uHoughTreshold = 100;

       raspi_gpgpu::RaspiConfig::_instance->_l2houghTreshold = 60;
       raspi_gpgpu::RaspiConfig::_instance->_u2HoughTreshold = 100;

       raspi_gpgpu::RaspiConfig::_instance->_angle = 200;
       raspi_gpgpu::RaspiConfig::_instance->_allFrames = false;
       raspi_gpgpu::RaspiConfig::_instance->_pipeline = -1;
       raspi_gpgpu::RaspiConfig::_instance->_lineFineProcessing = 1;
       raspi_gpgpu::RaspiConfig::_instance->_distanceFilter = 15;
       raspi_gpgpu::RaspiConfig::_instance->_angleFilter = 6;

       std::string file = "../libs/raspi_gpgpu/config.txt";
       cv::FileStorage fs = cv::FileStorage(file, cv::FileStorage::READ);
       raspi_gpgpu::RaspiConfig::_instance->_imageNr = (int)fs["imageNr"];

       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = (float)fs["lowerCannyThreshold"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = (float)fs["upperCannyThreshold"];

       raspi_gpgpu::RaspiConfig::_instance->_workMode = (int)fs["workMode"];       

         //camera
       raspi_gpgpu::RaspiConfig::_instance->_gain = (float)fs["gain"];
       raspi_gpgpu::RaspiConfig::_instance->_exposure = (int)fs["exposure"];
       raspi_gpgpu::RaspiConfig::_instance->_brightness = (int)fs["brightness"];
       raspi_gpgpu::RaspiConfig::_instance->_contrast = (int)fs["contrast"];

       fs.release();
	 }

	return raspi_gpgpu::RaspiConfig::_instance;
}

void raspi_gpgpu::RaspiConfig::Destroy()
{
	 if (!raspi_gpgpu::RaspiConfig::_instance) {
			 delete raspi_gpgpu::RaspiConfig::_instance;
			 raspi_gpgpu::RaspiConfig::_instance = NULL;
		 }
}

float raspi_gpgpu::RaspiConfig::GetLowerCannyThreshold() {
	float ret;
	if(IsColor)
		ret =  _lowerCannyThresholdC;
	else
		ret =  _lowerCannyThresholdBW;

	if(_lowerCannyParam != 0)
		ret = _lowerCannyParam;

	return ret;
}

float raspi_gpgpu::RaspiConfig::GetUpperCannyThreshold() {
	float ret;
		if(IsColor)
			ret =  _upperCannyThresholdC;
		else
			ret =  _upperCannyThresholdBW;

		if(_upperCannyParam != 0)
			ret = _upperCannyParam;

		return ret;
}

void raspi_gpgpu::RaspiConfig::LoadConfig(int type = 0)
{
   int input = type;

   if(!raspi_gpgpu::RaspiConfig::_instance->_fileLoaded)
   {
       std::string file = "../libs/raspi_gpgpu/config.txt";
       cv::FileStorage fs = cv::FileStorage(file, cv::FileStorage::READ);

       raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t1 = (int)fs["lHoughThreshold_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t1 = (int)fs["hHoughThreshold_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t1 = (int)fs["l2HoughThreshold_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t1 = (int)fs["h2HoughThreshold_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t1 = (int)fs["lPDistance_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t1 = (int)fs["uPDistance_t1"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t1n = (float)fs["lowerCannyParam_t1n"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t1n = (float)fs["upperCannyParam_t1n"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t1a = (float)fs["lowerCannyParam_t1a"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t1a = (float)fs["upperCannyParam_t1a"];


       raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t2 = (int)fs["lHoughThreshold_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t2 = (int)fs["hHoughThreshold_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t2 = (int)fs["l2HoughThreshold_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t2 = (int)fs["h2HoughThreshold_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t2 = (int)fs["lPDistance_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t2 = (int)fs["uPDistance_t2"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t2n = (float)fs["lowerCannyParam_t2n"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t2n = (float)fs["upperCannyParam_t2n"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t2a = (float)fs["lowerCannyParam_t2a"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t2a = (float)fs["upperCannyParam_t2a"];

      
       raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t3 = (int)fs["lHoughThreshold_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t3 = (int)fs["hHoughThreshold_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t3 = (int)fs["l2HoughThreshold_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t3 = (int)fs["h2HoughThreshold_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t3 = (int)fs["lPDistance_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t3 = (int)fs["uPDistance_t3"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t3n = (float)fs["lowerCannyParam_t3n"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t3n = (float)fs["upperCannyParam_t3n"];
       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t3a = (float)fs["lowerCannyParam_t3a"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t3a = (float)fs["upperCannyParam_t3a"];



       raspi_gpgpu::RaspiConfig::_instance->_fileLoaded = true;

       raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = (float)fs["lowerCannyThreshold"];
       raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = (float)fs["upperCannyThreshold"];

       int frames = (int)fs["allFrames"];
       raspi_gpgpu::RaspiConfig::_instance->_allFrames = frames == 1 ? true : false;

       fs.release();
   }
    

   if(input == 0 && raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead == 0)
   {
      if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 0)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 1;
         input = 1;
      }

      if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 1)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 0;
         input = 3;
      }
   }
   else if(input == 0 && raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead  == 1)
   {
      if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 0)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 1;
         input = 1;
      }

      if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 1)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 0;
         input = 3;
      }
      
   }
   else if(input == 0 && raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead  == 3)
   {
     if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 0)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 1;
         input = 3;
      }

      if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 1)
      {
         raspi_gpgpu::RaspiConfig::_instance->_lastThres = 0;
         input = 1;
      }      
   }
   else
   {
      input = 1;
      raspi_gpgpu::RaspiConfig::_instance->_lastThres = 0;
   }


   raspi_gpgpu::RaspiConfig::_instance->_lastThres = 1;
   input = 1;

   if(input == 1)
   {
       raspi_gpgpu::RaspiConfig::_instance->_houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t1;
       raspi_gpgpu::RaspiConfig::_instance->_uHoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t1;
       raspi_gpgpu::RaspiConfig::_instance->_l2houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t1;
       raspi_gpgpu::RaspiConfig::_instance->_u2HoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t1;
       raspi_gpgpu::RaspiConfig::_instance->_lSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t1;
       raspi_gpgpu::RaspiConfig::_instance->_uSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t1;

       raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead = 1;

       /*if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 0)
       {
         raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t1a;
         raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t1a;
       }
       if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 1)
       {
         raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t1n;
         raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t1n;
       }*/
   }

   if(input == 2)
   {
       raspi_gpgpu::RaspiConfig::_instance->_houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t2;
       raspi_gpgpu::RaspiConfig::_instance->_uHoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t2;
       raspi_gpgpu::RaspiConfig::_instance->_l2houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t2;
       raspi_gpgpu::RaspiConfig::_instance->_u2HoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t2;
       raspi_gpgpu::RaspiConfig::_instance->_lSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t2;
       raspi_gpgpu::RaspiConfig::_instance->_uSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t2;

      raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead = 2;
   }

    if(input == 3)
   {
       raspi_gpgpu::RaspiConfig::_instance->_houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_lHoughThreshold_t3;
       raspi_gpgpu::RaspiConfig::_instance->_uHoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_hHoughThreshold_t3;
       raspi_gpgpu::RaspiConfig::_instance->_l2houghTreshold = raspi_gpgpu::RaspiConfig::_instance->_l2HoughThreshold_t3;
       raspi_gpgpu::RaspiConfig::_instance->_u2HoughTreshold = raspi_gpgpu::RaspiConfig::_instance->_h2HoughThreshold_t3;
       raspi_gpgpu::RaspiConfig::_instance->_lSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_lPDistance_t3;
       raspi_gpgpu::RaspiConfig::_instance->_uSmallSquare = raspi_gpgpu::RaspiConfig::_instance->_uPDistance_t3;

      raspi_gpgpu::RaspiConfig::_instance->_lastConfigRead = 3;

      /*if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 0)
       {
         raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t3a;
         raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t3a;
       }
       if(raspi_gpgpu::RaspiConfig::_instance->_lastThres == 1)
       {
         raspi_gpgpu::RaspiConfig::_instance->_lowerCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_lowerCannyParam_t3n;
         raspi_gpgpu::RaspiConfig::_instance->_upperCannyThresholdBW = raspi_gpgpu::RaspiConfig::_instance->_upperCannyParam_t3n;
       }*/
   }

  
}

void raspi_gpgpu::RaspiConfig::SetFound(bool found)
{
     raspi_gpgpu::RaspiConfig::_instance->_lastFound = found;
}

int raspi_gpgpu::RaspiConfig::GetHoughHeight()
{
   if(raspi_gpgpu::RaspiConfig::_instance->_houghHeight == -1)
   {
      int hough = (int)(sqrt(2.0) * raspi_gpgpu::RaspiConfig::_instance->Width) + 1;
      int siz = hough * 180;
      int out = (int)(siz / raspi_gpgpu::RaspiConfig::_instance->Width) + 1;
      raspi_gpgpu::RaspiConfig::_instance->_houghHeight = out;

      //raspi_gpgpu::RaspiConfig::_instance->_houghHeight = raspi_gpgpu::RaspiConfig::_instance->Height; 
   }

   return raspi_gpgpu::RaspiConfig::_instance->_houghHeight;
}



#endif /* RASPI_CONFIG_CPP_ */
