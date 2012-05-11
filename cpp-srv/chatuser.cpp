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

bool ChatUser::is_cmd(const char* msg)
{
	return msg[0] == CMD_PREFIX[0];
}

void ChatUser::read_handler(std::string msg)
{
	if (!parser(msg, this))
		sender(gen_msg(msg), DEF_ROOM);
}

ChatUser::ChatUser(asio::io_service& io_service,
                   send_func sender,
                   parse_func parser) :
	status(CONNECTING),
	name(DEF_NAME),
	session(io_service, 
	        [this](){ this->disconnect(); },
	        [this](std::string msg){ this->read_handler(msg); }),
	sender(sender),
	parser(parser)
{
	
}

const string& ChatUser::get_name()
{
	return name;
}

void ChatUser::set_name(const string& name)
{
	this->name = name;
}

tcp::socket& ChatUser::get_socket()
{
	return session.get_socket();
}
