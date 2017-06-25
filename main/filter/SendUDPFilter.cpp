/*
 * WaitFilter.cpp
 *
 *	This filter does nothing but wait to sync to a specific framerate, where possible. Since we cannot travel back
 *	in time, we can only sync to the future (i.e. wait until sync is restored).
 *
 *  Created on: 28.06.2012
 *      Author: jung
 */

#include "SendUDPFilter.h"

/**
 * Constructor
 */
dsa::SendUDPFilter::SendUDPFilter(size_t input_size, std::string dst, UINT16 port) : resolver_(io_service_), socket_(io_service_) {
	input = std::vector<std::vector<char>*>(input_size);
	init();
	bool connected = openConnection(dst, port);
	if(!connected) {
		exit(1);
	}
}

/**
 * Destructor
 *
 * Nothing to destroy.
 */
dsa::SendUDPFilter::~SendUDPFilter() {
	closeConnection();
}

void dsa::SendUDPFilter::init() {
}

/**
 * An ImageFilter is considered ready, when it's input image exists and has data
 *
 * @return true if we have a real image to process, false otherwise
 */
bool dsa::SendUDPFilter::isReady() const {
	return(socket_.is_open() && input.size() > 0);
}

/**
 * Start waiting. Continue depending on configuration either after a key is pressed (any key by default) or enough time has passed
 * to not exceed the framerate.
 */
void dsa::SendUDPFilter::evaluate() {
	for(size_t i = 0; i < input.size(); i++) {
		sendData(i);
	}
}

bool dsa::SendUDPFilter::openConnection(std::string &dst, UINT16 port) {
	try {
		// open socket
		socket_.open(udp::v4());
		bool open = socket_.is_open();

		// resolve recipient name
		udp::resolver::query query(udp::v4(), dst, intToString((int)port));
		receiver_endpoint_ = *resolver_.resolve(query);

		// connect to reciever
		//sock.connect(receiver_endpoint);
		return(open);
	} catch(std::exception &e) {
	    std::cerr << e.what() << std::endl;
	    return(false);
	}
}

bool dsa::SendUDPFilter::closeConnection() {
	try {
		if(socket_.is_open())
			socket_.close();
		return(true);
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return(false);
	}
}

int dsa::SendUDPFilter::sendData(size_t index) {
	if(!(input[index] == NULL || input[index]->empty())) {
		try {
			boost::array<char, UDP_BUFFER_SIZE> send_buf;
			std::copy(input[index]->begin(), input[index]->end(), send_buf.begin());
			return(socket_.send_to(boost::asio::buffer(send_buf, input[index]->size()*sizeof(char)), receiver_endpoint_));
			//return(sock.send(boost::asio::buffer(send_buf, input[index]->size()*sizeof(char))));
		} catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
			return(-1);
		}
	} else return(0);
}
