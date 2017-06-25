/*
*	Transfers the image over TCP.
*/




#ifndef IMAGESERVERFILTER_H_
#define IMAGESERVERFILTER_H_

#include "EvaluationFilter.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

namespace dsa {


class ImageServerHandler
{
	public:
	ImageServerHandler();
	virtual ~ImageServerHandler();
	
	virtual void start()=0;
	virtual boost::asio::ip::tcp::socket* get_socket()=0;
	static void send_image(cv::Mat* image);
	
};


class ImageServerFilter : public EvaluationFilter 
{
	boost::asio::ip::tcp::endpoint _endpoint;
	boost::scoped_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
	boost::asio::io_service _io;
	boost::thread _t;
	
	void evaluate();
	
	void start_accept();
	void handle_accept(ImageServerHandler* h, const boost::system::error_code& error);

public:

	ImageServerFilter();
	virtual ~ImageServerFilter();

};

}
#endif /* IMAGESERVERFILTER_H_ */
