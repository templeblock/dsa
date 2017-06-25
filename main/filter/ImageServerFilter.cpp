#include "ImageServerFilter.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

dsa::ImageServerHandler::ImageServerHandler(){}
dsa::ImageServerHandler::~ImageServerHandler(){}

class Handler: public dsa::ImageServerHandler
{	public:
	
	Handler(boost::asio::io_service* io):
		_socket(*io)
	{	if(Handler::_io==NULL){Handler::_io=io;}	}
	
	~Handler(){/*std::cout<<"Handler deleted"<<std::endl;*/}
	
	boost::asio::ip::tcp::socket* get_socket()
	{return(&this->_socket);}
	
	void start()
	{
		std::cout<<"New connection"<<std::endl;
		Handler::handler_list.insert(this);
		this->_socket.async_receive(boost::asio::buffer(&_dummy_buffer,1),
		                            boost::bind(&Handler::close,this));
	}
	
	void close()
	{
		std::cout<<"Closing connection"<<std::endl;
		handler_list.erase(this);
		delete this;
	}
	
	static void send_image(cv::Mat* image)
	{
		if(!Handler::sending)
		{
			Handler::sending=true;
			//Handler::_imagebuffer=cv::Mat(image,true);
			image->copyTo(Handler::_imagebuffer);
			Handler::_io->post(&Handler::send_in_thread);
		}
	}
	
	
	private:
	static void send_in_thread()
	{
		for (std::set<Handler*>::iterator it=handler_list.begin();
		     it!=handler_list.end(); it++)
		{
			boost::asio::async_write(
			    (*it)->_socket,
			    boost::asio::buffer(
			        Handler::_imagebuffer.data,
			        Handler::_imagebuffer.rows
			          *Handler::_imagebuffer.cols
			          *Handler::_imagebuffer.channels()),
			    boost::bind(
			        &Handler::handle_send,(*it),
			        boost::asio::placeholders::error,
			        boost::asio::placeholders::bytes_transferred));
		}
		Handler::_io->post(&Handler::finish_send);
	}
	static void finish_send()	{Handler::sending=false;}
	
	void handle_send(const boost::system::error_code& error, 
	                 std::size_t bytes_transferred)
	{	//ignore for now
		/*if(error)
		{
			std::cout<<"Error occured on sending data"<<std::endl;
			this->close();
		}*/
	}
	
	static std::set<Handler* > handler_list;
	static bool sending;
	static boost::asio::io_service* _io;
	static cv::Mat _imagebuffer;
	
	char _dummy_buffer;
	boost::asio::ip::tcp::socket _socket;
};

cv::Mat Handler::_imagebuffer;
std::set<Handler* > Handler::handler_list;
bool Handler::sending=false;
boost::asio::io_service* Handler::_io=NULL;






dsa::ImageServerFilter::ImageServerFilter():
	_endpoint(boost::asio::ip::tcp::v4(),4265)
{
	this->_acceptor.reset(new boost::asio::ip::tcp::acceptor(_io,_endpoint));
	this->_acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	
	this->start_accept();
	
	boost::thread t(boost::bind(&boost::asio::io_service::run, &this->_io));
}


dsa::ImageServerFilter::~ImageServerFilter(){}


void dsa::ImageServerFilter::start_accept()
{
	Handler* h=new Handler(&this->_io);
	this->_acceptor->async_accept(*h->get_socket(),
	    boost::bind(&dsa::ImageServerFilter::handle_accept,
	                this, h, boost::asio::placeholders::error));
}

void dsa::ImageServerFilter::handle_accept(ImageServerHandler* h, 
                                           const boost::system::error_code& error)
{
	if(!error){		h->start();		}
	this->start_accept();
}



void dsa::ImageServerFilter::evaluate()
{
	Handler::send_image(this->src);
}

