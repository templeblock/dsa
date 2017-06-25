/*
 * helper.h
 *
 *  Created on: 27.03.2012
 *      Author: jung
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "definitions.h"
#include "types.h"
#include <cstdlib>
#include <ctime>
#include <boost/array.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <opencv2/highgui/highgui_c.h>
namespace dsa {

/**
 * Compare if two points are equal
 *
 * @param p1
 * @param p2
 *
 * @return true if so, false otherwise
 */
inline bool equal(const cv::Point &p1, const cv::Point &p2) {
	return(p1.x == p2.x && p1.y == p2.y);
}

/**
 * Determine distance between points p1 and p2
 * @param p1
 * @param p2
 * @return the distance
 */
inline int idistance(const cv::Point &p1, const cv::Point &p2) {
	int a = p2.y-p1.y;
	int b = p2.x-p1.x;

	return(sqrtf(a*a + b*b));
}

/**
 * Determine distance between points p1 and p2
 * @param p1
 * @param p2
 * @return the distance
 */
inline double distance(const cv::Point2f &p1, const cv::Point2f &p2) {
	double a = p2.y-p1.y;
	double b = p2.x-p1.x;

	return(sqrt(a*a + b*b));
}

/**
 * Determine longest distance between the points in a polygon
 *
 * @param polygon
 * @return
 */
inline int polygonLength(const std::vector<cv::Point> &polygon) {
	int retval = 1;
	for(size_t i = 0; i < polygon.size(); i++) {
		for(size_t j = i+1; j < polygon.size(); j++) {
			int dist = distance(polygon[i], polygon[j]);
			if(dist > retval)
				retval = dist;
		}
	}
	return(retval);
}

/**
 * Computes the cosine between two vectors (point2-point1 and point3-point2)
 * general formula: a-> * b-> = |a|*|b|*cos(alpha)
 *
 * @param point1: the first point
 * @param point2: the second point
 * @param point3: the third point
 *
 * @return the cosine of the angle
 */
inline float cosine(const cv::Point &point1, const cv::Point &point2, const cv::Point &point3) {
	int dx1 = point1.x - point2.x;
	int dy1 = point1.y - point2.y;
	int dx2 = point3.x - point2.x;
	int dy2 = point3.y - point2.y;

	return((dx1*dx2 + dy1*dy2) /(sqrtf((float)((dx1*dx1 + dy1*dy1) * (dx2*dx2 + dy2*dy2))) + FLT_MIN));
}

/**
 * Computes the gradient of a line described by two points.
 *
 *	@param x1: x value of point 1
 *	@param x2: y value of point 1
 *	@param x3: x value of point 2
 *	@param x4: y value of point 2
 *
 *	return the gradient of the line
 */
inline float computeGradient(const int x1, const int y1, const int x2, const int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	return((dx == 0) ? FLT_MAX : ((float)dy)/dx);
}

/**
 * Computes the gradient of the line described by two points.
 * To avoid later handling of infinity we use FLT_MAX in such cases.
 *
 * @param point1: a Point
 * @param point2: a Point
 *
 * @return the gradient of the line
 */
inline float computeGradient(const cv::Point &point1, const cv::Point &point2) {
	return(computeGradient(point1.x, point1.y, point2.x, point2.y));
}

/**
 * Computes the length of a line described by two Points.
 *
 * @param point1: a Point
 * @param point2: a Point
 *
 * @return the length of the line
 */
inline float computeLength(const cv::Point &point1, const cv::Point &point2) {
    int xdist = point2.x - point1.x;
    int ydist = point2.y - point1.y;

    return(sqrtf((float)(xdist*xdist + ydist*ydist)));
}

/**
 * Computes the perimeter of a polygon/polygonal line with p[i]->p[i+1]
 * @param points
 * @return
 */
inline float computeSquarePerimeter(const cv::Point (&p)[4]) {
	return(computeLength(p[1], p[0]) + computeLength(p[2], p[1]) + computeLength(p[3], p[2]) + computeLength(p[0], p[3]));
}

/**
 * Computes the perimeter of a polygon/polygonal line with p[i]->p[i+1]
 * @param points
 * @return
 */
inline float computeSquarePerimeter(const cv::Point2f (&p)[4]) {
	return(computeLength(p[1], p[0]) + computeLength(p[2], p[1]) + computeLength(p[3], p[2]) + computeLength(p[0], p[3]));
}

/**
 * Draws the square described by the polygonal line Point1 -> Point2 -> Point3 -> Point4 -> Point1 to mat
 *
 * @param mat: the image the square is drawn to
 * @param point1: first point in polygonal line
 * @param point2: second point in polygonal line
 * @param point3: third point in polygonal line
 * @param point4: fourth point in polygonal line
 */
inline void drawSquare(cv::Mat& mat, const cv::Point &point1, const cv::Point &point2, const cv::Point &point3, const cv::Point &point4) {
	cv::line(mat, point1, point2, cv::Scalar(0, 255, 0), 2);
	cv::line(mat, point2, point3, cv::Scalar(0, 255, 0), 2);
	cv::line(mat, point3, point4, cv::Scalar(0, 255, 0), 2);
	cv::line(mat, point4, point1, cv::Scalar(0, 255, 0), 2);
}

/**
 * Draws the square described by the polygonal line Point1 -> Point2 -> Point3 -> Point4 -> Point1 to mat
 *
 * @param mat: the image the square is drawn to
 * @param point1: first point in polygonal line
 * @param point2: second point in polygonal line
 * @param point3: third point in polygonal line
 * @param point4: fourth point in polygonal line
 * @param scalar: the color of the square border
 */
inline void drawSquare(cv::Mat& mat, const cv::Point &point1, const cv::Point &point2, const cv::Point &point3, const cv::Point &point4, cv::Scalar scalar) {
	cv::line(mat, point1, point2, scalar, 2);
	cv::line(mat, point2, point3, scalar, 2);
	cv::line(mat, point3, point4, scalar, 2);
	cv::line(mat, point4, point1, scalar, 2);
}


/**
 * Returns a bounding box for any rectangle(width, height, center), no matter how it may be oriented.
 * Because this box will include the rectangle no matter its orientation, the resulting area is in several
 * cases somewhat bigger than it would have to be, since we assume the worst case.
 *
 * @param center: center of the rectangle
 * @param width: width of the rectangle
 * @param height: height of the rectangle
 * @param xMax: max x coordinate in image
 * @param yMax: max y coordinate in image
 *
 * @return the bounding box (will always be a square)
 */
inline const cv::Rect getBoundingBoxFromCenter(const cv::Point center, const UINT32 width, const UINT32 height, const int xMax, const int yMax) {
	int a = 0.5f * sqrtf(float(height*height + width*width)); // half the length of the bounding box
	cv::Point Pmin = cv::Point(std::max(center.x - a, 0), std::max(center.y - a, 0));
	cv::Point Pmax = cv::Point(std::min(center.x + a, xMax), std::min(center.x + a, yMax));
	return(cv::Rect(Pmin, Pmax));
}

/**
 * Rotate point p around another point c by angle alpha (positive alpha value means a counter-clockwise rotation). Remember: our coordinate origin is in the top left corner, so we need to multiply angle by -1.
 *
 * @param point: the point to rotate
 * @param center: the point to rotate around
 * @param alpha: the angle to rotate for
 *
 * @return rotated Point
 */
inline const cv::Point rotatePoint(const cv::Point &point, const cv::Point2f &center, const float alpha) {
	// move center to (0,0)
	float x = point.x - center.x;
	float y = point.y - center.y;
	// rotate around (0,0)
	float sina = sinf(-alpha);
	float cosa = cosf(-alpha);
	float temp = x;	// save x value temporarily for y calculation
	x = x*cosa - y*sina;
	y = y*cosa + temp*sina;

	// move center back to original position
	return (cv::Point((int)(x + center.x), (int)(y + center.y)));
}

/**
 * Returns the min value of uchar1 and uchar2. This function is necessary because min is not defined for uchar
 *
 * @param uchar1
 * @param uchar2
 * @return min value of uchar1 and uchar2
 */
inline UINT8 min(const UINT8 uchar1, const UINT8 uchar2) {
	return((uchar1 < uchar2) ? uchar1 : uchar2);
}

/**
 * Computes the shannon entropy for images with one channel. If you have multiple, compute them separately and add em
 *
 * @param mat one channel image
 * @param histSize number of bins in histogram
 * @param rangeMin range of the histogram (min value)
 * @param rangeMax range of the histogram (max value)
 * @return the shannon entropy value
 */
inline float computeShannonEntropy(cv::Mat &mat, const int histSize, const float rangeMin, const float rangeMax) {
	const float range[] = {rangeMin, rangeMax} ;
	const float *histRange = {range};

	// compute histogram
	cv::Mat hist;
	cv::calcHist(&mat, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
	delete histRange;

	float frequency = 0.0f;	// how many pixels to consider
	for(int i = 1; i < histSize; i++)
		frequency += fabsf(hist.at<float>(i));

	// entropy = -sum((p_xi)*log(p_xi))
	float entropy = 0.0f;
	for(int i = 1; i < histSize; i++) {	// skip total black and total white
		float p_xi = fabsf(hist.at<float>(i))/frequency;	// probability of a pixel being of color value i
		if(p_xi > 0) // log is only defined for p_xi > 0
			entropy += p_xi * log10f(p_xi);
	}

	return(-entropy);
}

/**
 * Show Mat image in a namedWindow and wait for keyboard input until returning
 *
 * @param mat: the image to be shown
 */
inline void showImage(const cv::Mat& mat) {
	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE);
	cv::imshow("Image", mat);
	cv::waitKey(0);
	cv::destroyWindow("Image");
}

/**
 * Converts an ipv4 address in binary form into a human readable string of the octets
 *
 * @param ipAddress: the ip address in binary form
 *
 * @return a string of the form 1.2.3.4
 */
const inline std::string ipV4AddressToString(const UINT32 ipAddress) {
	UINT8 ip[4];
	// shift relevant octet to the first byte and cut it out by casting to unsigned char
	ip[0] = (UINT8)(ipAddress >> 24);
	ip[1] = (UINT8)(ipAddress >> 16);
	ip[2] = (UINT8)(ipAddress >> 8);
	ip[3] = (UINT8)ipAddress;

	std::ostringstream oss;
	oss << std::dec;
	for (UINT8 i = 0; i < 3; i++)
	   oss << (UINT16)ip[i] << ".";
	oss << (UINT16)ip[3];
	return(oss.str());
}

/**
 * Converts an ipv6 address in binary form into a human readable string of the 2byte blocks each.
 * Likely this method won't be necessary for quite a while...
 *
 * @param firstHalf front 64bit of the address
 * @param secondHalf last 64bit of the address
 *
 * @return a string of the form 1000:2000:3000:4000:5000:6000:7000:8000
 * 								|firstHalf			|secondHalf			|
 */
const inline std::string ipV6AddressToString(const UINT64 firstHalf, const UINT64 secondHalf) {
	UINT16 ip[8];
	// shift relevant block to the first short and cut it out by casting to unsigned short
	ip[0] = (UINT16)(firstHalf >> 48);
	ip[1] = (UINT16)(firstHalf >> 32);
	ip[2] = (UINT16)(firstHalf >> 16);
	ip[3] = (UINT16)firstHalf;
	ip[4] = (UINT16)(secondHalf >> 48);
	ip[5] = (UINT16)(secondHalf >> 32);
	ip[6] = (UINT16)(secondHalf >> 16);
	ip[7] = (UINT16)secondHalf;

	std::ostringstream oss;
	oss << std::hex;
	for (UINT8 i = 0; i < 7; i++)
	   oss << ip[i] << ":";
	oss << (UINT16)ip[7];
	return(oss.str());
}

/**
 * Converts a string representing an ip address of form a.b.c.d to a 32-bit integer
 *
 * @param ipAddressAsString
 *
 * @return ip address as 32-bit unsigned integer
 */
inline UINT32 stringToIpAddress(const std::string &ipAddressAsString) {
	UINT32 ipInt = 0x0;
	size_t begin = 0;
	for(UINT8 i = 0; i < 4; i++) {
		size_t end = std::min((UINT32)ipAddressAsString.find('.', begin), (UINT32)ipAddressAsString.size());
		ipInt |= ((UINT32) atoi(ipAddressAsString.substr(begin, end-begin).c_str())) << ((3-i)*8);
		begin = end + 1;
	}
	return(ipInt);
}

/**
 * Converts a mac address in binary form into a human readable string of the octets
 *
 * @param macAddress: the mac address in binary form
 *
 * @return a string of the form 00:01:02:03:04:05
 */
inline std::string macAddressToString(const UINT64 macAddress) {
	// a mac address is 48 bytes, so split into 16 bits (front) and 32 bits (rear)
	const UINT16 half1 = (UINT16)(macAddress >> 32);
	const UINT32 half2 = (UINT32)(macAddress);

	UINT8 mac[6];
	// shift relevant octet to the first byte and cut it out by casting to unsigned char
	mac[0] = (UINT8)(half1 >> 8);
	mac[1] = (UINT8)half1;
	mac[2] = (UINT8)(half2 >> 24);
	mac[3] = (UINT8)(half2 >> 16);
	mac[4] = (UINT8)(half2 >> 8);
	mac[5] = (UINT8)half2;

	std::ostringstream oss;
	oss << std::hex;
	for (UINT8 i = 0; i < 5; i++) {
		if(mac[i] < 16)
			oss << "0";	// leading 0 for small numbers
		oss << (UINT16)mac[i] << ":";
	}
	if(mac[5] < 16)
		oss << "0";	// leading 0 for small numbers
	oss << (UINT16)mac[5];
	return(oss.str());
}

inline void intToString(const int i, std::string& res) {
	std::ostringstream temp;
    temp << i;
    res = temp.str();
}

inline std::string intToString(const int i) {
	std::ostringstream temp;
    temp << i;
    return(temp.str());
}

inline void doubleToString(const double i, std::string& res) {
	std::ostringstream temp;
    temp << i;
    res = temp.str();
}

inline std::string doubleToString(const double i) {
	std::ostringstream temp;
    temp << i;
    return(temp.str());
}

std::string doubleToString(const double i, int precision) {
	int j = (int)i;
    int p = abs(pow(10, precision));
    int k = abs(round((i-j)*p));
    p = p/10;
	std::ostringstream temp;
    temp << j;
    temp << ".";
    while(k < p) {
    	temp << "0";
    	p /= 10;
    }
    temp << k;
    return(temp.str());
}

inline clock_t currentTimeOfDayMilliseconds() {
	boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
	return(current_date_microseconds.time_of_day().total_milliseconds()); // get local timestamp
}

inline INT8 randChar() {
	return((char)(rand()%256 - 128));
}

inline UINT8 randUnsignedChar() {
	return((unsigned char)(rand()%256));
}

inline INT16 randShort() {
	return((short)(rand()%32768 - 16384));
}

inline UINT16 randUnsignedShort() {
	return((unsigned short)(rand()%32768));
}

inline boost::array<char, 2> randBytes() {
	short random = randShort();
	char first = (char)(random >> 4);
	char second = (char)(0x00FF | random);
	boost::array<char, 2> retval = {{ first, second }};
	return(retval);
}

inline boost::array<char, 4> splitWordToBytes(int value) {
	boost::array<char, 4> retval = boost::array<char, 4>();
	retval[0] = (char)(value >> 24);
	retval[1] = (char)((0x00ff0000 & value) >> 16);
	retval[2] = (char)((0x0000ff00 & value) >> 8);
	retval[3] = (char)(0x000000ff & value);
	return(retval);
}

inline boost::array<char, 2> splitHalfwordToBytes(short value) {
	boost::array<char, 2> retval = boost::array<char, 2>();
	retval[0] = (char)(value >> 8);
	retval[1] = (char)(0xff & value);
	return(retval);
}

inline char signedChar(unsigned char unsignedChar) {
	short retval = (short)unsignedChar;
	return((char)(retval-128));
}

inline short signedShort(unsigned short unsignedShort) {
	int retval = (int)unsignedShort;
	return((short)(retval-32768));
}

inline int signedInt(unsigned int unsignedInt) {
	long long retval = (long long)unsignedInt;
	return((int)(retval-2e31));
}

/**
 * General information on a gige vision camera, like address manufacturer and model
 */
struct GigeCameraInfo {
	UINT32 ipAddress;
	UINT64 macAddress;
	std::string manufacturerName;
	std::string modelName;

	void print() {
		// print out info to console
		std::cout << "Camera IP\t\t: " << ipV4AddressToString(ipAddress) << std::endl;
		std::cout << "MAC Address\t: " << macAddressToString(macAddress) << std::endl;
		std::cout << "Manufacturer Name\t: " << manufacturerName << std::endl;
		std::cout << "Model Name\t: " << modelName << std::endl;
	}
};

}
#endif /* HELPER_H_ */
