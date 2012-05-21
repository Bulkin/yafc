#include "server.h"

using namespace boost;
using boost::asio::ip::tcp;

using std::string;

void Server::start_accept()
{
	auto user = rooms.add_user(std::shared_ptr<ChatUser> (
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
	            [this](ChatUser* sender, CmdProcessor::string_list params)
	            { 
		            if (this->rooms.find(params.back()))
			            return false;
		            return sender->set_name(params.back());
	            });

	cmdproc.set("list-users",
	            [this](ChatUser* sender, CmdProcessor::string_list params)
	            {
		            string room = DEF_ROOM;
		            if (params.size() > 1)
			            room = params.back();
		            return this->rooms.send_user_list(room, sender);
	            });

	cmdproc.set("list-rooms",
	            [this](ChatUser* sender, CmdProcessor::string_list params)
	            {
		            return this->rooms.send_room_list(sender);
	            });

	cmdproc.set("join",
	            [this](ChatUser* sender, CmdProcessor::string_list params)
	            {
		            if (params.size() < 1)
			            return false;
		            if (this->rooms.join(sender, params.back()))
			            sender->set_target(params.back());
		            else
			            return false;
		            return true;
	            });

	cmdproc.set("to",
	            [this](ChatUser* sender, CmdProcessor::string_list params)
	            {
		            if (params.size() < 1)
			            return false;
		            std::shared_ptr<sendable> dest = 
			            this->rooms.find(params.back());
		            if (dest)
			            sender->set_target(params.back());
		            else
			            return false;
		            return true;
	            });
}
