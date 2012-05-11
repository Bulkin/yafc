#include <boost/asio.hpp>

#include "roomlist.h"
#include "cmdproc.h"

class Server 
{
public:
	Server(boost::asio::io_service& io_service, short port);

private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	CmdProcessor cmdproc;
	
	RoomList rooms;
	
	void start_accept();
	void setup_cmdproc();
	void handle_accept(UserList::user_ptr user,
	                   const boost::system::error_code& error);
};
