/*
 * UDPPacketSender.h
 *
 *	Send data over a network channel to a recipient. We do not care if data is lost on the way, so UDP works out of the box.
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#ifndef UDPPACKETSENDER_H_
#define UDPPACKETSENDER_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <generic/helper.h>

using boost::asio::ip::udp;

namespace dsa {

class UDPPacketSender {

	udp::socket *socket_;
	boost::asio::io_service *io_service_;
    udp::endpoint receiver_endpoint_;

	bool openConnection(std::string &dst, UINT16 port);
	bool closeConnection();

public:

	UDPPacketSender(std::string dst="localhost", UINT16 port=24191);
	virtual ~UDPPacketSender();

	bool isReady() const;
	int send(std::vector<char> &payload);
};

}
#endif /* UDPPACKETSENDER_H_ */
