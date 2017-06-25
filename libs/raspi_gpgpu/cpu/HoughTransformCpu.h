/*
 * HoughTransformCpu.h
 *
 *  Created on: 27 sty 2016
 *      Author: gregStudent
 */

#ifndef HOUGHTRANSFORMCPU_H_
#define HOUGHTRANSFORMCPU_H_

#include <opencv2/opencv.hpp>
#include <tr1/unordered_map>
//#include <ctime>

namespace raspi_gpgpu
{
  class HoughCpu
  {
  public :
	  HoughCpu();
	 ~HoughCpu();
	 // int Transform(unsigned char* img_data, int w, int h);	
     //std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > GetLines(int threshold);

	  void GetLinesGPU(int threshold, unsigned char* img_data, std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> &squareCandidates);
	  
	  const unsigned int* GetAccu(int *w, int *h);
	  cv::Mat DoTransform(cv::Mat *source, cv::Mat &output);
	  void DoTransform(cv::Mat *source, std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > &lines);

     void DoTransform(cv::Mat *source, std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> &squareCandidates);

	  void GetCorners(unsigned char*source, std::vector<std::pair<int, int>>  &points, int w, int h);
	  bool Comparator(const cv::Point2f &a, const cv::Point2f &b);
	  void SortCorners(std::vector< std::pair< std::pair<int, int>, std::pair<int, int> >> &lines);
	  bool Intersection(cv::Point2f p0, cv::Point2f p1, cv::Point2f p2, cv::Point2f p3, cv::Point2f& r);
     std::vector<std::pair<int,int>> GetSimilarLines(std::vector<std::pair<int,int>> &candidates, std::pair<int,int> &reff);
     std::vector<std::pair<int,int>> GetSimilarLine(std::vector<std::pair<int,int>> &candidates, std::pair<int,int> &reff);
     void ComputeCoord(int &x1, int &y1, int &x2, int &y2, std::pair<int,int> &l1);

  private:
  		unsigned int* _accu;
  		int _accu_w;
  		int _accu_h;
  		int _img_w;
  		int _img_h;
  		int _upperThreshold;
  };
}

#endif /* HOUGHTRANSFORMCPU_H_ */
