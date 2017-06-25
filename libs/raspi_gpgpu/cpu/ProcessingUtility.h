/*
 * ProcessingUtility.h
 *
 *  Created on: 11 lut 2016
 *      Author: gregStudent
 */

#ifndef PROCESSINGUTILITY_H_
#define PROCESSINGUTILITY_H_


#include <opencv2/opencv.hpp>

namespace raspi_gpgpu
{
  class ProcessingUtility
  {
  public :
	  static cv::Mat SortCorners(std::vector<std::pair<int, int>> &points,  std::vector< std::pair< std::pair<int, int>, std::pair<int, int> >> &lines,  cv::Mat *source)
	  {
		  struct {
		 		   bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b)
		 		   {
		 			   return a.second<b.second;
		 		   }
		 	   } LessY;

		 	   struct {
		 		   bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b)
		 		   {
		 			   return a.first<b.first;
		 		   }
		 	   } LessX;

		 	   // get max and min value in y
		 	   // vector<int>::iterator a = max_element(...);
		        std::pair<int,int> maxY = *std::max_element(points.begin(),points.end(), LessY);
		        std::pair<int,int> minY = *std::min_element(points.begin(),points.end(), LessY);
		        std::pair<int,int> maxX = *std::max_element(points.begin(),points.end(), LessX);
		        std::pair<int,int> minX = *std::min_element(points.begin(),points.end(), LessX);


		 	   std::pair<int, int> tl = minX;
		 	   std::pair<int, int> tr = maxY;
		 	   std::pair<int, int> bl = minY;
		 	   std::pair<int, int> br = maxX;

		 	   // line 1
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tl.first,tl.second), std::pair<int, int>(tr.first,tr.second)));
		 	   // line 4
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tr.first,tr.second), std::pair<int, int>(br.first,br.second)));
		 	   // line 2
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(br.first,br.second), std::pair<int, int>(bl.first,bl.second)));
		 	   // line 3
		 	    lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(bl.first,bl.second), std::pair<int, int>(tl.first,tl.second)));



		 	   cv::Mat copy = source->clone();

		 	   std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > >::iterator it;
		 	   int index = 0;
		 	   	for(it=lines.begin();it!=lines.end();it++)
		 	   	{
		 	   	    cv::line(copy, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar( 50+ (index * 40), 50+ (index * 40), 50+ (index * 40)), 2, 5);

		 	   		index++;
		 	   	}

		 	   	return copy;
	  }

 static void SortCornersLight(std::vector<std::pair<int, int>> &points,  std::vector< std::pair< std::pair<int, int>, std::pair<int, int> >> &lines,  cv::Mat *source)
	  {
		  struct {
		 		   bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b)
		 		   {
		 			   return a.second<b.second;
		 		   }
		 	   } LessY;

		 	   struct {
		 		   bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b)
		 		   {
		 			   return a.first<b.first;
		 		   }
		 	   } LessX;

		 	   // get max and min value in y
		 	   // vector<int>::iterator a = max_element(...);
		        std::pair<int,int> maxY = *std::max_element(points.begin(),points.end(), LessY);
		        std::pair<int,int> minY = *std::min_element(points.begin(),points.end(), LessY);
		        std::pair<int,int> maxX = *std::max_element(points.begin(),points.end(), LessX);
		        std::pair<int,int> minX = *std::min_element(points.begin(),points.end(), LessX);


		 	   std::pair<int, int> tl = minX;
		 	   std::pair<int, int> tr = maxY;
		 	   std::pair<int, int> bl = minY;
		 	   std::pair<int, int> br = maxX;

		 	   // line 1
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tl.first,tl.second), std::pair<int, int>(tr.first,tr.second)));
		 	   // line 4
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tr.first,tr.second), std::pair<int, int>(br.first,br.second)));
		 	   // line 2
		 	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(br.first,br.second), std::pair<int, int>(bl.first,bl.second)));
		 	   // line 3
		 	    lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(bl.first,bl.second), std::pair<int, int>(tl.first,tl.second)));


	  }

	  static void GetCorners(unsigned char*source, std::vector<std::pair<int, int>>  &points, int w, int h)
	  {
		  for(int y=0;y<h;y++)
		  	{
		  		for(int x=0;x<w;x++)
		  		{
		  			if( source[ (y*w*3) + x*3] > 250 )
		  			{
		  				std::pair<int, int> p(x,y);
		  				points.push_back(p);
		  			}
		  		}
		  	}
	  }
  };
}
#endif /* PROCESSINGUTILITY_H_ */
