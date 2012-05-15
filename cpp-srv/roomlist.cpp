#include "roomlist.h"

using namespace std;

RoomList::RoomList()
{
	//	rooms[""] = UserList();
}

UserList::user_ptr RoomList::add_user(UserList::user_ptr user, 
                                      const string& room)
{
	if (rooms.find(room) == rooms.end())
		rooms[room] = shared_ptr<UserList>(new UserList);
	return rooms[room]->add(user);
}

void RoomList::send_message(const string& message, const string& destination)
{
	auto dest = find(destination);
	if (dest)
		dest->send_message(message, destination);
}

shared_ptr<sendable> RoomList::find(const std::string& name)
{
	for (auto room : rooms)
		if (room.first == name)
			return shared_ptr<sendable>(room.second);
	return rooms[DEF_ROOM]->find_user(name);
}
