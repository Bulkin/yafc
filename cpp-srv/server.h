#include <boost/asio.hpp>

#include "roomlist.h"

class Server 
{
public:
	Server(boost::asio::io_service& io_service, short port);

private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	
	RoomList rooms;
	
	void start_accept();

	void handle_accept(UserList::user_ptr user,
	                   const boost::system::error_code& error);
};
