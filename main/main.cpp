//============================================================================
// Name        : main.cpp
// Author      : Matthias Jung
// Version     : 1.0
// Copyright   : Lehrstuhl fml
// Description : cvbenchmark C++, Ansi-style style 1
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <net/UDPPacketSender.h>
#include <eval/EvaluationLogger.h>
#include <demonstrator/LocalizationFunction.h>
#include <demonstrator/ForkHeightFunction.h>
#include <net/LocalizationToDatagramTranslator.h>
#include <sensordata/Calibrator.h>
#include "../libs/raspi_gpgpu/raspi_config.h"


#ifdef WITH_RASPICAM
	#include <raspicam_cv.h>
	#include <unistd.h>
#endif



using namespace dsa;

/**
 * Calibrate a camera using our Calibrator class
 *
 * @param argc
 * @param argv
 * @return
 */
static struct timeval tm1;

void stoper_start()
{
    gettimeofday(&tm1, NULL);
}

void stoper_stop()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    std::cout<< t <<" ms" << std::endl;
}
int mainCalibrate(const int argc, const char* const argv[]) {
	
		//UINT32 argLength = strlen(argv[1]);
		#ifdef WITH_RASPICAM
			raspicam::RaspiCam_Cv* capture = new raspicam::RaspiCam_Cv();
			/*if(!capture->open()){
				std::cout<<"Error opening camera"<<std::endl; return 1; 
			}
			else{
				std::cout<<"opened RaspiCam_Cv OK, going sleep 3 secs"<<std::endl;
			}
			sleep(3);*/

		#else
			VideoCapture *capture = new VideoCapture(0);
		#endif
		//capture->set(CV_CAP_PROP_FRAME_WIDTH, 640);
		//capture->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
		//capture->set(CV_CAP_PROP_FPS, 25.0);
      //capture->set( CV_CAP_PROP_FORMAT, CV_8UC1); 

		Calibrator calibrator(capture);
		calibrator.run();
		calibrator.writeCalibrationToXML("camera.xml");

		delete capture;
		return(0);
	
}

int testCamera(const int argc, const char* const argv[]){

    time_t timer_begin,timer_end;
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    //IplImage *im;
    int nCount=100;
    
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing "<<nCount<<" frames ...."<<endl;
    time ( &timer_begin );
    for ( int i=0; i<nCount; i++ ) {
        Camera.grab();
        Camera.retrieve ( image);        
        if ( i%5==0 )  cout<<"\r captured "<<i<<" images"<<std::flush;
        //cv::imshow("test",image);
        
    }
    cv::imwrite("raspicam_cv_image.jpg",image);
    cout<<"Stop camera..."<<endl;
    Camera.release();
    //cvReleaseImage(&im);
   return 0;
}

// localization demonstrator
int mainTestDemos(const int argc, const char* const argv[]) {

   raspi_gpgpu::RaspiConfig *cnf = raspi_gpgpu::RaspiConfig::Instance();
	
   //cnf->_lowerCannyParam = (argc > 1) ? atof(argv[1]) : 0;
	//cnf->_upperCannyParam = (argc > 2) ? atof(argv[2]) : 0;   

   if(argc < 2)
      cnf->LoadConfig();
   else
   {
      cnf->_houghTreshold = (argc > 1) ? atoi(argv[1]) : -1;  
      cnf->_uHoughTreshold = (argc > 2) ? atoi(argv[2]) : -1;

      cnf->_l2houghTreshold = (argc > 3) ? atoi(argv[3]) : -1;
      cnf->_u2HoughTreshold = (argc > 4) ? atoi(argv[4]) : -1;

      cnf->_lSmallSquare = (argc > 5) ? atoi(argv[5]) : -1;;
      cnf->_uSmallSquare = (argc > 6) ? atoi(argv[6]) : -1;
   }
   
  // cnf->_allFrames = (argc > 7) ? (atoi(argv[7]) == 1 ? true : false) : false;
   cnf->_pipeline = (argc > 8) ? atoi(argv[8]) : -1;
   cnf->_lineFineProcessing = (argc > 9) ? atoi(argv[9]) : 1;
   cnf->_distanceFilter = (argc > 10) ? atoi(argv[10]) : 15;
   cnf->_angleFilter = (argc > 11) ? atoi(argv[11]) : 6;
   cnf->_angle = (argc > 12) ? atoi(argv[12]) : 200;
  

   double fps = 45.0; // (argc > 2) ? strtod(argv[2], 0) : 25.0;
	bool autoexposure = (argc > 22) ? atoi(argv[22]) == 1 : false;

	std::string ausgabe = (argc > 21)  ? std::string(argv[21]) : std::string("ausgabe");
	ausgabe.append(".csv");
	int width = 640; // (argc > 5) ? atoi(argv[5]) : 1280;
	int height = 480; //(argc > 6) ? atoi(argv[6]) : 720;
   cnf->Width = float(width);
	cnf->Height = float(height);
	cnf->IsColor = false;

	double angle = (argc > 7) ? atof(argv[7]) : 0;
	int exposure = argc > 8 ? atoi(argv[8]) : -1;
	double gain = argc > 9 ? atof(argv[9]) : -1;

	#ifdef WITH_RASPICAM
		raspicam::RaspiCam_Cv* capture = new raspicam::RaspiCam_Cv();
		capture->set( CV_CAP_PROP_FORMAT, CV_8UC1); 
     
      capture->set(CV_CAP_PROP_FRAME_WIDTH, width);//1280
		capture->set(CV_CAP_PROP_FRAME_HEIGHT, height); //720
		capture->set(CV_CAP_PROP_FPS, fps);
      //capture->set(CV_CAP_PROP_EXPOSURE,2);
      //capture->set(CV_CAP_PROP_GAIN,2);

		std::cout<<"Opening Camera..."<<std::endl;
		if ( !capture->open()) {std::cerr<<"Error opening camera"<<std::endl;return -1;}
		//wait a while until camera stabilizes
		std::cout<<"Sleeping for 3 secs"<<std::endl;
		sleep(3);
	#else
		VideoCapture *capture = new VideoCapture(atoi(argv[1]));
      capture->set(CV_CAP_PROP_FRAME_WIDTH, width);
   	capture->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	   capture->set(CV_CAP_PROP_FPS, fps);
	#endif

	

	LocalizationFunction *localizationFunction = new LocalizationFunction(capture, fps, exposure, width, height, angle, gain);
	WaitForInputWorker *waitForInputWorker = new WaitForInputWorker();

	//boost::thread localizationThread = boost::thread(&LocalizationFunction::run, &(*localizationFunction));
	//boost::thread listenForInputThread = boost::thread(&WaitForInputWorker::run, &(*waitForInputWorker));

	// create logger
	EvaluationLogger logger(ausgabe, waitForInputWorker);
	LocalizationToDatagramTranslator translator = LocalizationToDatagramTranslator(52);
	UDPPacketSender sender = UDPPacketSender("127.0.0.1");

	int msPerFrame = 1000/fps;
	bool stop = false;
	std::vector<char> payload = std::vector<char>();
	int last = currentTimeOfDayMilliseconds();
	
   raspi_gpgpu::RaspiGL *wrapper = 0;

	try
	{
		//namedWindow( "Display window", WINDOW_AUTOSIZE );
		localizationFunction->init();
		localizationFunction->loadConfig();
		localizationFunction->InitialiseFilters();

		//pOgles = ogles_gpgpu::OglesCU::getInstance();
		//pOgles->Init();
      wrapper = raspi_gpgpu::RaspiGL::GetInstance();
      wrapper->Init();

		//while(!stop) {
		for(int i=0;i<1;i++){
			// get results
			//stoper_start();
			localizationFunction->run();
			//stoper_stop();
			LocalizationResult *localizationResult = (LocalizationResult*)localizationFunction->getLastValid();

			//ForkHeightResult *forkHeightResult = new ForkHeightResult();
			//std::cerr << "ANGLE: " << localizationResult->r[2] << std::endl;
			// do someting with the results
			//logger.log(localizationResult, forkHeightResult);	// log it
			// send it out via UDP
			//translator.translate(localizationResult, forkHeightResult, payload);
			//sender.send(payload);

			// quit on user input
			//stop = waitForInputWorker->isFinished();
			// delete generated objects
			payload.clear();
			delete localizationResult;
			//delete forkHeightResult;

			// sleep if there is still time
			int now = currentTimeOfDayMilliseconds();
			int waitFor = msPerFrame - (now-last);
			last = now;

	//		if(waitFor > 0)
	//			boost::this_thread::sleep(boost::posix_time::milliseconds(waitFor));
		}

		//pOgles->Destroy();
      wrapper->Destroy();
	}
	catch(const std::exception & ex )
	{
		std::cout << ex.what();
		localizationFunction->stop();
		delete capture;
		delete localizationFunction;
		
      raspi_gpgpu::RaspiConfig::Instance()->Destroy();
		//delete waitForInputWorker;
	}

	// cleanup (stop threads)
   localizationFunction->stop();
   //delete capture;	
	// wait for the threads to finish on their own
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	// call join
	//localizationThread.join();
	//listenForInputThread.join();
	// delete all the stuff
	delete localizationFunction;
	delete waitForInputWorker;
   raspi_gpgpu::RaspiConfig::Instance()->Destroy();

	return(0);
}


// main method
int main(const int argc, const char* const argv[]) {
	//return(mainCalibrate(argc, argv));
	return(mainTestDemos(argc, argv)); //org
	//return(mainTestDemos2(argc, argv));
    //return (testCamera(argc,argv));
}
