#include "chatsession.h"

#include <iostream>

using namespace boost;
using boost::asio::ip::tcp;

using std::string;

void ChatSession::send_message(const string& msg)
{
	asio::async_write(socket,
	                  asio::buffer(msg.c_str(), msg.length()),
	                  [this](const system::error_code& error,
	                         size_t bytes_transferred)
	                  { this->handle_error(error); });
}

void ChatSession::handle_error(const system::error_code& error)
{
	if (error) {
		std::cout << "write error: " << error << std::endl;
		this->disconnector();
	} 
}

void ChatSession::handle_read(const system::error_code& error,
                              size_t bytes_transferred)
{
	if (!error)	{
		buf[MAX_MSG_LEN] = 0; // make sure we have a c-string
		message = buf; // don't bother with partial messages
		if ((message[message.size()-1]) != '\n')
			message.push_back('\n');
		reader(message);
		socket.cancel();
		socket.async_read_some(asio::buffer(buf, max_length),
		                       [this](const system::error_code& error,
		                              size_t bytes_transferred)
		                       { this->handle_read(error, 
		                                           bytes_transferred); });
	} else {
		std::cout << "read error: " << error <<std::endl;
		this->disconnector();
	}
}

ChatSession::ChatSession(asio::io_service& io_service,
                         disconnect_handler disconnector,
                         read_handler reader) : 
	socket(io_service),
	disconnector(disconnector),
	reader(reader)
{
	buf[max_length] = 0;
}
	
tcp::socket& ChatSession::get_socket() 
{ 
	return socket; 
}

void ChatSession::start()
{
	socket.async_read_some(asio::buffer(buf, max_length),
	                       [this](const system::error_code& error,
	                              size_t bytes_transfered)
	                       { this->handle_read(error,
	                                           bytes_transfered); });
}
