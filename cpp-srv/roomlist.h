#include <unordered_map>
#include <string>

#include "userlist.h"

class RoomList 
{
	std::unordered_map<std::string, UserList> rooms;

public:
	RoomList();
	UserList::user_ptr& add_user(const UserList::user_ptr& user,
	                            std::string room = "");
	void send_message(std::string message,
	                  std::string destination);
};
