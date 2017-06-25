/*
 * SendUDPFilter.h
 *
 *	Send data over a network channel to a recipient. We do not care if data is lost on the way, so UDP works out of the box.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef SENDUDPFILTER_H_
#define SENDUDPFILTER_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <generic/helper.h>
#include "EvaluationFilter.h"

using boost::asio::ip::udp;

namespace dsa {

class SendUDPFilter : public EvaluationFilter {

	boost::asio::io_service io_service_;
	udp::resolver resolver_;
	udp::socket socket_;
	udp::endpoint receiver_endpoint_;

	bool openConnection(std::string &dst, UINT16 port);
	bool closeConnection();
	int sendData(size_t index);

	void init();
	void evaluate();

public:

	std::vector<std::vector<char>*> input;

	SendUDPFilter(size_t input_size=1, std::string dst="localhost", UINT16 port=24191);
	virtual ~SendUDPFilter();

	bool isReady() const;
};

}
#endif /* SENDUDPFILTER_H_ */
