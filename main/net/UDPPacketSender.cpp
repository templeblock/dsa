/*
 * UDPPacketSender.cpp
 *
 *	This filter does nothing but wait to sync to a specific framerate, where possible. Since we cannot travel back
 *	in time, we can only sync to the future (i.e. wait until sync is restored).
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "UDPPacketSender.h"

/**
 * Constructor
 */
dsa::UDPPacketSender::UDPPacketSender(std::string dst, UINT16 port) {
	bool connected = openConnection(dst, port);
	if(!connected)
		std::cerr << "Could not connect to Socket" << std::endl;
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::UDPPacketSender::~UDPPacketSender() {
	closeConnection();
}


/**
 * An ImageFilter is considered ready, when it's input image exists and has data
 *
 * @return true if we have a real image to process, false otherwise
 */
bool dsa::UDPPacketSender::isReady() const {
	return(socket_->is_open());
}

bool dsa::UDPPacketSender::openConnection(std::string &dst, UINT16 port) {
	try {
		// resolve recipient name
		io_service_ = new boost::asio::io_service();
		udp::resolver resolver(*io_service_);
		udp::resolver::query query(udp::v4(), dst, intToString((int)port));
		receiver_endpoint_ = *resolver.resolve(query);
		// open socket to reciever
		socket_ = new udp::socket(*io_service_);
		socket_->open(udp::v4());
		bool open = socket_->is_open();
		socket_->connect(receiver_endpoint_);
		return(true);
	} catch(std::exception &e) {
	    std::cerr << e.what() << std::endl;
	    return(false);
	}
}

bool dsa::UDPPacketSender::closeConnection() {
	try {
		if(socket_->is_open())
			socket_->close();
		delete socket_;
		delete io_service_;
		return(true);
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return(false);
	}
}

int dsa::UDPPacketSender::send(std::vector<char> &payload) {
	if(!payload.empty()) {
		try {
			boost::array<char, UDP_BUFFER_SIZE> send_buf;
			std::copy(payload.begin(), payload.end(), send_buf.begin());
			//return(socket_->send_to(boost::asio::buffer(send_buf, input->size()*sizeof(char)), receiver_endpoint_));
			return(socket_->send(boost::asio::buffer(send_buf, payload.size()*sizeof(char))));
		} catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
			return(-1);
		}
	} else return(0);
}
