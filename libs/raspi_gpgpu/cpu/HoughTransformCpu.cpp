
#include "HoughTransformCpu.h"
#include <opencv/highgui.h>
#include <algorithm>
#include <tuple>
#include "../raspi_config.h"

#define DEG2RAD 0.017453293f

raspi_gpgpu::HoughCpu::HoughCpu() :_accu(0), _accu_w(0), _accu_h(0), _img_w(0), _img_h(0), _upperThreshold(100) {
// _upperThreshold(80) for 1280

}

raspi_gpgpu::HoughCpu::~HoughCpu() {
	if(_accu)
		free(_accu);
}


const unsigned int* raspi_gpgpu::HoughCpu::GetAccu(int* w, int* h) {
	*w = _accu_w;
	*h = _accu_h;

	return _accu;
}

// do not use this anymore
void raspi_gpgpu::HoughCpu::DoTransform(
		cv::Mat* source, std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > &lines) {

	int w = source->cols;
	int h = raspi_gpgpu::RaspiConfig::Instance()->Height; //source->rows;
	_img_w = w;
	_img_h = h;
	double hough_h = ((sqrt(2.0) * (double)(h>w?h:w)) / 2.0);
	_accu_h = hough_h * 2.0; // -r -> +r
	_accu_w = 180;

	int threshold = 42; //(10/18)45; //(6stopni)62 ;
   std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> squareCandidates;
	GetLinesGPU(threshold, source->data, squareCandidates);

   //for(int i = 0 ; i< squareCandidates.size(); i++)
   //{
   //   for(int j = 0; j< squareCandidates[i].size(); j++)
   //   SortCorners(squareCandidates[i][j]);
   //}
   
}

// use in produciton
void raspi_gpgpu::HoughCpu::DoTransform(cv::Mat *source, std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> &squareCandidates)
{
   int w = source->cols;
	int h = raspi_gpgpu::RaspiConfig::Instance()->Height; //source->rows;
	_img_w = w;
	_img_h = h;
	double hough_h = ((sqrt(2.0) * (double)(h>w?h:w)) / 2.0);
	_accu_h = hough_h * 2.0; // -r -> +r
	_accu_w = 180;

   //std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> squareCandidates;
	//clock_t c0 = clock();
	GetLinesGPU(0, source->data, squareCandidates);

   clock_t start = clock();
   for(int i = 0 ; i< squareCandidates.size(); i++)
   {
      SortCorners(squareCandidates[i]);
   }
   std::cout<<"\nSorting corners:  "<< ((float)clock() - start)/CLOCKS_PER_SEC << std::endl;
}

// use for preview
cv::Mat raspi_gpgpu::HoughCpu::DoTransform(cv::Mat *source, cv::Mat &output) {
	// init
	int w = source->cols;
	int h =  raspi_gpgpu::RaspiConfig::Instance()->Height; //source->rows;
	_img_w = w;
	_img_h = h;
	double hough_h = ((sqrt(2.0) * (double)(h>w?h:w)) / 2.0);
	_accu_h = hough_h * 2.0; // -r -> +r
	_accu_w = 180;

	cv::Mat copy = output.clone();


	int threshold = raspi_gpgpu::RaspiConfig::Instance()->_houghTreshold;
   _upperThreshold = raspi_gpgpu::RaspiConfig::Instance()->_uHoughTreshold;
   printf("\nHough treshold %d", threshold);
	// CPU
	//Transform(source->data, w, h);
	//std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > lines = GetLines(threshold);

	// 42 for 1280
	//(10/18)45; //(6stopni)62 ; //w>h?w/4:h/4;



	// GPU
	std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> squareCandidates;
	//clock_t c0 = clock();
	GetLinesGPU(threshold, source->data, squareCandidates);
    //clock_t c1 = clock();
    //std::cout << c1 - c0 << " GetLinesGPU time " << std::endl;

	//Draw the results
   for(int i = 0 ; i < squareCandidates.size(); i++)
   {
	   std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > >::iterator it;
	   for(it=squareCandidates[i].begin();it!=squareCandidates[i].end();it++)
	   {
	   	cv::line(copy, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar( 100, 100, 0), 2, 5);
	   }
	   //printf("\nThere are %d lines detected by HOUGH\n\n", lines.size());
   	//cv::imshow( "showing hough", copy );
   }
	

	return copy;
}

void raspi_gpgpu::HoughCpu::GetCorners(unsigned char* source, std::vector<std::pair<int, int>>  &points ,int w, int h) {

	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
		{
			if( source[ (y*w*3) + x*3] > 250 )
			{
				std::pair<int, int> p(x,y);
				//cv::Point2f p(x, y);
				points.push_back(p);
			}
		}
	}

}

bool raspi_gpgpu::HoughCpu::Comparator(const cv::Point2f &a, const cv::Point2f &b) {
	  return a.x<b.x;
}




void raspi_gpgpu::HoughCpu::SortCorners( std::vector< std::pair< std::pair<int, int>, std::pair<int, int> >> &lines) {
	   std::vector<std::pair<int, int>> top, bot;

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


	   //cv::Point2f inter;
	   std::vector<std::pair<int, int>> points;
      bool verbose = !raspi_gpgpu::RaspiConfig::Instance()->_allFrames;
	   
     cv::Point2f inter, inter2, inter3, inter4;
      int x1, y1, x2, y2;
      int x1_2, y1_2, x2_2, y2_2;

      bool intersect = false;
      // p1
      //ComputeCoord(x1, y1, x2, y2, lines[0]);
      //ComputeCoord(x1_2, y1_2, x2_2, y2_2, lines[2]);
  
      intersect = Intersection(cv::Point2f(lines[0].first.first, lines[0].first.second),
						    cv::Point2f(lines[0].second.first, lines[0].second.second),
         				 cv::Point2f(lines[2].first.first, lines[2].first.second),
						    cv::Point2f(lines[2].second.first, lines[2].second.second),
						    inter);
      
      if(verbose)
       std::cout<< "\np1 intersect: " << intersect << std::endl;
      if(!intersect)
         return;
   

      // p2
      //ComputeCoord(x1, y1, x2, y2, lines[1]);
      //ComputeCoord(x1_2, y1_2, x2_2, y2_2, lines[2]);
      intersect = Intersection(cv::Point2f(lines[1].first.first, lines[1].first.second),
						    cv::Point2f(lines[1].second.first, lines[1].second.second),
         				 cv::Point2f(lines[2].first.first, lines[2].first.second),
						    cv::Point2f(lines[2].second.first, lines[2].second.second),
						    inter2);

      if(verbose)
         std::cout<< "p2 intersect: " << intersect << std::endl;
      if(!intersect)
         return;

      // p3
      //ComputeCoord(x1, y1, x2, y2, lines[1]);
      //ComputeCoord(x1_2, y1_2, x2_2, y2_2, lines[3]);
      intersect = Intersection(cv::Point2f(lines[1].first.first, lines[1].first.second),
						    cv::Point2f(lines[1].second.first, lines[1].second.second),
         				 cv::Point2f(lines[3].first.first, lines[3].first.second),
						    cv::Point2f(lines[3].second.first, lines[3].second.second),
						    inter3);

      if(verbose)
         std::cout<< "p3 intersect: " << intersect << std::endl;
      if(!intersect)
         return;

      // p4
      //ComputeCoord(x1, y1, x2, y2, lines[0]);
      //ComputeCoord(x1_2, y1_2, x2_2, y2_2, lines[3]);
      intersect = Intersection(cv::Point2f(lines[0].first.first, lines[0].first.second),
						    cv::Point2f(lines[0].second.first, lines[0].second.second),
         				 cv::Point2f(lines[3].first.first, lines[3].first.second),
						    cv::Point2f(lines[3].second.first, lines[3].second.second),
						    inter4);
       if(verbose)
         std::cout<< "p4 intersect: " << intersect << std::endl;
       if(!intersect)
         return;

       points.push_back(std::pair<int, int>(inter.x, inter.y));      

       points.push_back(std::pair<int, int>(inter2.x, inter2.y));        

       points.push_back(std::pair<int, int>(inter3.x, inter3.y));        

       points.push_back(std::pair<int, int>(inter4.x, inter4.y));      


       if(verbose)
       {
             std::cout<< "intersect1 x: " << inter.x << " y: " << inter.y <<  std::endl; 
             std::cout<< "intersect2 x: " << inter2.x << " y: " << inter2.y <<  std::endl; 
             std::cout<< "intersect3 x: " << inter3.x << " y: " << inter3.y <<  std::endl; 
             std::cout<< "intersect4 x: " << inter4.x << " y: " << inter4.y <<  std::endl  <<  std::endl; 
       }
/*
       std::pair<int,int> maxY = *std::max_element(points.begin(),points.end(), LessY);
       std::pair<int,int> minY = *std::min_element(points.begin(),points.end(), LessY);
       std::pair<int,int> maxX = *std::max_element(points.begin(),points.end(), LessX);
       std::pair<int,int> minX = *std::min_element(points.begin(),points.end(), LessX);



	   std::pair<int, int> tl = minX;
	   std::pair<int, int> tr = maxY;
	   std::pair<int, int> bl = minY;
	   std::pair<int, int> br = maxX;


	   lines.clear();

	   // line 1
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tl.first,tl.second), std::pair<int, int>(tr.first,tr.second)));
      std::cout<< "line1 x1: " << tl.first << " y1: " << tl.second << " x2: " << tr.first << " y2: " << tr.second << std::endl;

	   // line 4
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(tr.first,tr.second), std::pair<int, int>(br.first,br.second)));
      std::cout<< "line2 x1: " << tr.first << " y1: " << tr.second << " x2: " << br.first << " y2: " << br.second << std::endl;

	   // line 2
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(br.first,br.second), std::pair<int, int>(bl.first,bl.second)));
     std::cout<< "line3 x1: " << br.first << " y1: " << br.second << " x2: " << bl.first << " y2: " << bl.second << std::endl;

	   // line 3
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(bl.first,bl.second), std::pair<int, int>(tl.first,tl.second)));
      std::cout<< "line4 x1: " << bl.first << " y1: " << bl.second << " x2: " << tl.first << " y2: " << tl.second << std::endl;*/

lines.clear();

      lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(inter.x, inter.y), std::pair<int, int>(inter4.x, inter4.y)));
	   
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(inter4.x, inter4.y), std::pair<int, int>(inter3.x, inter3.y)));
	  
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(inter3.x, inter3.y), std::pair<int, int>(inter2.x, inter2.y)));
	 
	   lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int>>(std::pair<int, int>(inter2.x, inter2.y), std::pair<int, int>(inter.x, inter.y)));

}

 void raspi_gpgpu::HoughCpu::GetLinesGPU(
		int threshold, unsigned char* img_data, 
      std::vector< std::vector< std::pair< std::pair<int, int>, std::pair<int, int>>>> &squareCandidates) {

   bool verbose = !raspi_gpgpu::RaspiConfig::Instance()->_allFrames;

   int l1Thres = raspi_gpgpu::RaspiConfig::Instance()->_houghTreshold;
   int u1Thres = raspi_gpgpu::RaspiConfig::Instance()->_uHoughTreshold;
   int l2Thres = raspi_gpgpu::RaspiConfig::Instance()->_l2houghTreshold;
   int u2Thres = raspi_gpgpu::RaspiConfig::Instance()->_u2HoughTreshold;

   if(verbose)
   {  
    std::cout<<"lt:  "<< l1Thres  << std::endl;  
      std::cout<<"ut:  "<< u1Thres  << std::endl;  
      std::cout<<"l2t:  "<< l2Thres  << std::endl;  
      std::cout<<"u2t:  "<< u2Thres  << std::endl;   
      std::cout<<"smallSquaret  "<< raspi_gpgpu::RaspiConfig::Instance()->_lSmallSquare << std::endl;
   } 

   /*std::cout<<"lSmallSquare: "<< raspi_gpgpu::RaspiConfig::Instance()->_lSmallSquare << std::endl;
   std::cout<<"uSmallSquare: "<< raspi_gpgpu::RaspiConfig::Instance()->_uSmallSquare  << std::endl;
   std::cout<<"pdist: "<< raspi_gpgpu::RaspiConfig::Instance()->_distanceFilter  << std::endl;
   std::cout<<"adist: "<< raspi_gpgpu::RaspiConfig::Instance()->_angleFilter  << std::endl;*/

	//std::vector<std::pair<int,int>> lineCoordsBAK;
	//std::vector<std::pair<int,int>> lineCoords;
   std::vector<std::pair<int,int>> candidates;
   std::vector<std::pair<int,int>> t1candidates;
   std::vector<std::pair<int,int>> t2candidates;

   // ./main 12 35 300 300 1 43 0 0 0 4 6
   std::vector<std::pair<int,int>> t1acandidates;
   std::vector<std::pair<int,int>> t2acandidates;

   std::tr1::unordered_map<int, std::vector<std::pair<int,int>>> lineMap;
   std::tr1::unordered_map<int, std::vector<std::pair<int,int>>> tmplineMap;

   int range = raspi_gpgpu::RaspiConfig::Instance()->_distanceFilter; // 14, 15
   int angleRange = raspi_gpgpu::RaspiConfig::Instance()->_angleFilter;
   
   if(verbose)
   {
   std::cout<<"distance:  "<< range  << std::endl;
   std::cout<<"angle:  "<< angleRange  << std::endl;
   }


   int rw = _accu_w*3;

	clock_t start = clock();   

	for(int r=0;r<_accu_h;r++) //for(int r=0;r<_accu_h;r++)
	{
		// compute pointer here
		for(int t=0;t<_accu_w;t+=6) //for(int t=0;t<_accu_w;t+=6)
		{
			
			int toTest = (int)img_data[(r*rw) + t*3];        
			if((l1Thres <= toTest && toTest <= u1Thres))
			{           
				//Is this point a local maxima (9x9)
				int th = 0;
				//int max = th = img_data[(r*rw) + t*3];
            int max = th = toTest;


            for(int ly=1;ly<=4;ly++)
				{
					for(int lx=-angleRange;lx<=angleRange;lx+=6)
					{
						if( (ly+r>=0 && ly+r<_accu_h) && (lx+t>=0 && lx+t<_accu_w)  )
						{
							if(lx == 0 && ly == 0)
								continue;


							int val = (int)img_data[( (r+ly)*rw) + (t+lx)*3];                  
							if( val > max )
							{
								max = val;
								ly = lx = range + 1;
							}
						}
					}
				}

				if(max > th)
					continue;
            else            
               t1acandidates.push_back(std::pair<int,int>(r,t));

             //std::cout<<"Added to t1acandidates r: "<< r << " t: " << t << std::endl;


				for(int ly=5;ly<=range;ly++)
				{
					for(int lx=-angleRange;lx<=angleRange;lx+=6)
					{
						if( (ly+r>=0 && ly+r<_accu_h) && (lx+t>=0 && lx+t<_accu_w)  )
						{


							int val = (int)img_data[( (r+ly)*rw) + (t+lx)*3];                  
							if( val > max )
							{
								max = val;
								ly = lx = range + 1;
							}
						}
					}
				}

				if(max > th)
					continue;
            else            
               t1candidates.push_back(std::pair<int,int>(r,t));
           
			}
        

		}
	}
   std::cout<<"Scanning Hough Image:  "<< ((float)clock() - start)/CLOCKS_PER_SEC << std::endl;
   
  if(verbose)
  {  
    std::cout<<"size of t1candidates:  "<< t1candidates.size()  << std::endl; 
  }

for(int c= 0; c < t1candidates.size(); c++)
	{
		for(int j= 0; j < t1candidates.size(); j++)
			{
				if(t1candidates[c].second == t1candidates[j].second)
				{
					if(abs(t1candidates[c].first - t1candidates[j].first) > 22 && abs(t1candidates[c].first - t1candidates[j].first) < 45) // 18 50
					{
						t2candidates.push_back(t1candidates[c]);
						t2candidates.push_back(t1candidates[j]);
                  j = t1candidates.size();
					}
				}
			}
	}


   std::vector<std::vector<std::pair<int,int>>> sCandidates;

	for(int c= 0; c < t2candidates.size(); c+=2)
	{
		for(int j= 0; j < t2candidates.size(); j+=2)
		{
			if(abs(t2candidates[c].second - t2candidates[j].second) == 90)
			{
            int dif1 = abs(t2candidates[c].first - t2candidates[c+1].first);
            int dif2 = abs(t2candidates[j].first - t2candidates[j+1].first);
            int totdif =  abs(dif1 - dif2);
            if(totdif < raspi_gpgpu::RaspiConfig::Instance()->_lSmallSquare) // 10
            {
               std::vector<std::pair<int,int>> tmp;
				   tmp.push_back(t2candidates[c]);
   				tmp.push_back(t2candidates[c+1]);
	   			tmp.push_back(t2candidates[j]);
	   			tmp.push_back(t2candidates[j+1]);
               sCandidates.push_back(tmp);
            }
            else
               if(verbose)
                  std::cout << "OUT, lines dist diff: " << totdif << std::endl;

				j = t2candidates.size();
            
            //std::cout << "lines dist diff: " << abs(dif1 - dif2) << std::endl;
			}

		}
	}

   std::cout << "After first level squares: " << sCandidates.size() << std::endl;


   if(sCandidates.size() == 0)
   {
      if(verbose)
         std::cout << "Applying second config " << std::endl;
      
      for(int c= 0; c < t1acandidates.size(); c++)
	   {
		   for(int j= 0; j < t1acandidates.size(); j++)
			   {
			   	if(t1acandidates[c].second == t1acandidates[j].second)
			   	{
			   		if(abs(t1acandidates[c].first - t1acandidates[j].first) > 22 && abs(t1acandidates[c].first - t1acandidates[j].first) < 45) // 18 50
			   		{
			   			t2acandidates.push_back(t1acandidates[c]);
			   			t2acandidates.push_back(t1acandidates[j]);
                     j = t1acandidates.size();
			   		}
			   	}
			   }
	   }

      //std::cout << "size of t2acandidates is: " << t2acandidates.size() << std::endl;
      //return;
      for(int c= 0; c < t2acandidates.size(); c+=2)
   	{
	   	for(int j= 0; j < t2acandidates.size(); j+=2)
	   	{
	   		if(abs(t2acandidates[c].second - t2acandidates[j].second) == 90)
	   		{
               int dif1 = abs(t2acandidates[c].first - t2acandidates[c+1].first);
               int dif2 = abs(t2acandidates[j].first - t2acandidates[j+1].first);
               int totdif =  abs(dif1 - dif2);
               if(totdif < 1) // 10
               {
                  std::vector<std::pair<int,int>> tmp;
	   			   tmp.push_back(t2acandidates[c]);
      				tmp.push_back(t2acandidates[c+1]);
	      			tmp.push_back(t2acandidates[j]);
	      			tmp.push_back(t2acandidates[j+1]);
                  sCandidates.push_back(tmp);
               }
               else
                  if(verbose)
                     std::cout << "OUT, lines dist diff: " << totdif << std::endl;

	   			j = t2acandidates.size();
   			}

		   }
   	}

   } // end if(sCandidates.size() == 0)

   // post processing
   int sqrs = 0;
   for(int k = 0; k< sCandidates.size(); k++)
   { 
      std::vector<std::pair<int,int>> tt = sCandidates[k];

      candidates.push_back(tt[0]);
      candidates.push_back(tt[1]);
      candidates.push_back(tt[2]);
      candidates.push_back(tt[3]);
      sqrs++;
   }
   std::cout << "After last level squares: " << sqrs << std::endl;


   
   std::vector<std::pair<int,int>> finals2;
   std::vector<int> rightVals;
  

   finals2 = candidates; //finals;

 
	// create lines
	
   int k = finals2.size();
   std::vector<std::pair< std::pair<int, int>, std::pair<int, int> >> lines;

	for(int i = 0; i < k; i++)
	{		
		int x1, y1, x2, y2, r, t;
		x1 = y1 = x2 = y2 = 0;		
      r = finals2[i].first;
		t = finals2[i].second;

		if(t >= 45 && t <= 135)
		{
			//y = (r - x cos(t)) / sin(t)
			x1 = 0;
			y1 = ((double)(r-(_accu_h/2)) - ((x1 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);
			x2 = _img_w - 0;
			y2 = ((double)(r-(_accu_h/2)) - ((x2 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);
		}
		else
		{
			//x = (r - y sin(t)) / cos(t);
			y1 = 0;
			x1 = ((double)(r-(_accu_h/2)) - ((y1 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
			y2 = _img_h - 0;
			x2 = ((double)(r-(_accu_h/2)) - ((y2 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
		}
		


		if(lines.size() >= 150)
		{
			r = _accu_h;
			t = _accu_w;
		}
		else
		{
			lines.push_back(std::pair< std::pair<int, int>, std::pair<int, int> >(std::pair<int, int>(x1,y1), std::pair<int, int>(x2,y2)));

         if(lines.size() == 4)
         {
            squareCandidates.push_back(lines);
            lines.clear();
         }

         /*if(lines.size() < 25){
			   //printf("\nLine %d:%d,  %d:%d",x1,y1, x2,y2);  
            if(i % 2 == 0)         
			      printf("\n\ndistance %d, angle %d", r, t);     
            else        
               printf("\ndistance %d, angle %d ", r, t);
         } */
		}
	}



}


void raspi_gpgpu::HoughCpu::ComputeCoord(int &x1, int &y1, int &x2, int &y2, std::pair<int,int> &l1)
{
   int r,t;
   r = l1.first;
	t = l1.second;

   if(t >= 45 && t <= 135)
	{
			//y = (r - x cos(t)) / sin(t)
			x1 = 0;
			y1 = ((double)(r-(_accu_h/2)) - ((x1 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);

			x2 = _img_w - 0;
			y2 = ((double)(r-(_accu_h/2)) - ((x2 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);
	}
	else
	{
			//x = (r - y sin(t)) / cos(t);
			y1 = 0;
			x1 = ((double)(r-(_accu_h/2)) - ((y1 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
			y2 = _img_h - 0;
			x2 = ((double)(r-(_accu_h/2)) - ((y2 - (_img_h/2) ) * sin(t * DEG2RAD))) / cos(t * DEG2RAD) + (_img_w / 2);
	}

}


std::vector<std::pair<int,int>> raspi_gpgpu::HoughCpu::GetSimilarLine(std::vector<std::pair<int,int>> &candidates, std::pair<int,int> &reff)
{
   std::vector<std::pair<int,int>> result;
   int t = candidates.size();
   for(int i = 0; i < t; i++)
   {
      if(abs(candidates[i].second - reff.second) < 3) // must be same angle
      {
         int dst = abs(candidates[i].first - reff.first);

         if(dst >= raspi_gpgpu::RaspiConfig::Instance()->_lSmallSquare &&
                  dst <= raspi_gpgpu::RaspiConfig::Instance()->_uSmallSquare)
   		{	
            result.push_back(candidates[i]);
            break;
   		}
      }
   }

   return result;

}



std::vector<std::pair<int,int>> raspi_gpgpu::HoughCpu::GetSimilarLines(std::vector<std::pair<int,int>> &candidates, std::pair<int,int> &reff)
{
    std::vector<std::pair<int,int>> result;
    int t = candidates.size();
    
   for(int i = 0; i < t; i++)
   {
      if(candidates[i].first != 0)
      {
         // big square 25  20
         if(abs(candidates[i].first - reff.first) < 11 && abs(candidates[i].second - reff.second) < 6)
         {
            result.push_back(std::pair<int, int>(candidates[i].first, candidates[i].second));
            candidates[i].first = 0;
         }
      }
   }

   return result;
}

bool raspi_gpgpu::HoughCpu::Intersection(cv::Point2f p0, cv::Point2f p1,
		cv::Point2f p2, cv::Point2f p3, cv::Point2f& r) {

    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1.x - p0.x;     s1_y = p1.y - p0.y;
    s2_x = p3.x - p2.x;     s2_y = p3.y - p2.y;

	float s, t;
	s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = ( s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		
		r.x = p0.x + (t * s1_x);
		r.y = p0.y + (t * s1_y);
		return true;
	}

	return false;



}
