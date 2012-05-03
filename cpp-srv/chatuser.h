#ifndef CHATUSER_H
#define CHATUSER_H

#include <string>
#include <functional>
#include <memory>
#include <boost/asio.hpp>

#include "chatsession.h"

static const char* DEF_NAME = "anonymous";

class ChatUser : public std::enable_shared_from_this<ChatUser>
{

public:

	typedef enum {
		CONNECTING = 1,
		CONNECTED = 2,
		DISCONNECTED = 3
	} state;

	typedef std::function<void(std::string, std::string)> send_func;

	ChatUser(boost::asio::io_service& io_service,
	         send_func sender);
	boost::asio::ip::tcp::socket& get_socket();
	void disconnect();	
	state get_status();
	void start_session();
	const std::string& get_name();
	void send_message(const std::string& msg);

private:

	ChatSession session;
	std::string name;
	state status;
	send_func sender;

	void connect();

	std::string gen_msg(const std::string& msg);
	void read_handler(std::string msg);
};

#endif
