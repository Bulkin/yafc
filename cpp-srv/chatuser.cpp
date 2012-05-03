#include "chatuser.h"

using namespace boost;
using boost::asio::ip::tcp;

using std::string;

void ChatUser::connect()
{
	status = CONNECTED;
}

void ChatUser::disconnect()
{
	status = DISCONNECTED;
}

ChatUser::state ChatUser::get_status()
{
	return status;
}

void ChatUser::start_session()
{
	connect();
	session.start();
}

void ChatUser::send_message(const string& msg)
{
	if (status == CONNECTED)
		session.send_message(msg);
}

string ChatUser::gen_msg(const string& msg)
{
	return name + "> " + msg;
}

void ChatUser::read_handler(std::string msg)
{
	sender(gen_msg(msg), "");
}

ChatUser::ChatUser(asio::io_service& io_service,
                   send_func sender) :
	status(CONNECTING),
	name(DEF_NAME),
	session(io_service, 
	        [this](){ this->disconnect(); },
	        [this](std::string msg){ this->read_handler(msg); }),
	sender(sender)
{
	
}

const string& ChatUser::get_name()
{
	return name;
}

tcp::socket& ChatUser::get_socket()
{
	return session.get_socket();
}
