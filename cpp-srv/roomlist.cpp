#include "roomlist.h"

using namespace std;

RoomList::RoomList()
{
	//	rooms[""] = UserList();
}

UserList::user_ptr& RoomList::add_user(const UserList::user_ptr& user, 
                                      string room)
{
	return rooms[room].add(user);
}

void RoomList::send_message(string message, string destination)
{
	if (rooms.find(destination) != rooms.end())
		rooms[destination].send_message(message);
	else {
		auto user = rooms[DEF_ROOM].find(destination);
		if (user)
			user->send_message(message);
	}
}
