/*
 * HttpStreamVideoCapture.h
 *
 *  Created on: 10.07.2012
 *      Author: jung
 */

#ifndef HTTPSTREAMVIDEOCAPTURE_H_
#define HTTPSTREAMVIDEOCAPTURE_H_

#include <generic/helper.h>

namespace dsa {

class HttpStreamVideoCapture: public cv::VideoCapture {

public:

	HttpStreamVideoCapture(const std::string& url);
	HttpStreamVideoCapture(const std::string& host, const std::string &path, const std::string& user="", const std::string& password="", const UINT16 port=80);

    // the destructor
	virtual ~HttpStreamVideoCapture();
};

}
#endif /* HTTPSTREAMVIDEOCAPTURE_H_ */
