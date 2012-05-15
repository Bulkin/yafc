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

void ChatUser::send_message(const string& msg, const string& dest)
{
	if (status == CONNECTED)
		session.send_message(msg);
}

string ChatUser::gen_msg(const string& msg)
{
	return name + "->" + target + "> " + msg;
}

void ChatUser::read_handler(std::string msg)
{
	if (!parser(msg, this))
		sender(gen_msg(msg), target);
}

ChatUser::ChatUser(asio::io_service& io_service,
                   send_func sender,
                   parse_func parser) :
	status(CONNECTING),
	name(DEF_NAME),
	target(DEF_ROOM),
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

bool ChatUser::validate_name(const string& name)
{
	if (name.size() > 2 && name.size() < 16)
		return true;
	else return false;
}

bool ChatUser::set_name(const string& name)
{
	if (validate_name(name))
		this->name = name;
	else 
		return false;
	return true;
}

tcp::socket& ChatUser::get_socket()
{
	return session.get_socket();
}
