#ifndef CHATSESSION_H
#define CHATSESSION_H

#include <functional>
#include <string>

#include <boost/asio.hpp>

static const MAX_MSG_LEN 255

class ChatSession
{
	enum { max_length = 1023 };
public:
	typedef std::function<void(void)> disconnect_handler;
	typedef std::function<void(std::string)> read_handler;

	ChatSession(boost::asio::io_service& io_service,
	            disconnect_handler disconnector,
	            read_handler reader);

	boost::asio::ip::tcp::socket& get_socket();
	void start();
	void send_message(const std::string& msg);

private:
	boost::asio::ip::tcp::socket socket;
	disconnect_handler disconnector;
	read_handler reader;
	char buf[max_length+1];
	std::string message;
	void handle_read(const boost::system::error_code& error,
	                 size_t bytes_transferred);

	void handle_error(const boost::system::error_code& error);
};

#endif
