/*
 * HttpStreamVideoCapture.cpp
 *
 *  Created on: 10.07.2012
 *      Author: jung
 */

#include "HttpStreamVideoCapture.h"

std::string intToString(const int i);

dsa::HttpStreamVideoCapture::HttpStreamVideoCapture(const std::string& url) : cv::VideoCapture(url) {
}

dsa::HttpStreamVideoCapture::HttpStreamVideoCapture(const std::string& host, const std::string& path, const std::string& user, const std::string& password, const UINT16 port) : cv::VideoCapture() {
	std::string url = "http://";
	if(user.length() > 0) {	// optional auth info
		url.append(user);
		if(password.length() > 0)
			url.append(":" + password);
		url.append("@");
	}
	// host and port
	url.append(host);
	if(port != 80)
		url.append(":" + intToString(port));
	(path.at(0) != '/') ? url.append('/' + path) : url.append(path);	// just in case someone forgot the prefix '/'
	url.append((path.at(0) != '/') ? ('/' + path) : path);

	cv::VideoCapture::open(url);
}

dsa::HttpStreamVideoCapture::~HttpStreamVideoCapture() {
}
