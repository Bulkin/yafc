#include "server.h"

using namespace boost;
using boost::asio::ip::tcp;

using std::string;

void Server::start_accept()
{
	UserList::user_ptr user = rooms.add_user(UserList::user_ptr(
	    new ChatUser(io_service,
		             [this](string message, string destination)
		{ this->rooms.send_message(message, destination); },
	                 [this](string message, ChatUser* sender)
		{ return this->cmdproc.parse(message, sender); })));
	acceptor.async_accept(user->get_socket(),
	                      [this, user](const system::error_code& error)
	                      { this->handle_accept(user, error); });
}

Server::Server(asio::io_service& io_service, short port) :
	io_service(io_service),
	acceptor(io_service, tcp::endpoint(tcp::v4(), port))
{
	setup_cmdproc();
	start_accept();
}

void Server::handle_accept(UserList::user_ptr user,
                           const system::error_code& error)
{
	if (!error) {
		user->start_session();
	} else {
		user->disconnect();
	}

	start_accept();
}

void Server::setup_cmdproc()
{
	cmdproc.set("name", 
	            [](ChatUser* sender, CmdProcessor::string_list params)
	            { sender->set_name(params.back()); });
}
