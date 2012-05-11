#include <unordered_map>
#include <string>

#include "userlist.h"
#include "cmdproc.h"

class RoomList 
{
	std::unordered_map<std::string, UserList> rooms;
public:
	CmdProcessor cmdproc;

	RoomList();
	UserList::user_ptr& add_user(const UserList::user_ptr& user,
	                            std::string = DEF_ROOM);
	void send_message(std::string message,
	                  std::string destination);
};
