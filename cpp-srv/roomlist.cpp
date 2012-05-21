#include "roomlist.h"

using namespace std;

#define NAMES_PER_LINE 12

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

void RoomList::send_str_list(const string& prefix, int max,
                             vector<string>& strings,
                             sendable* destination)
{
	auto i = strings.begin();
	do { 
		auto i_end = strings.end();
		if ((i + max) < i_end)
			i_end = i + max;
		string names;
		
		for_each(i, i_end, [&names](string& n) { names += n + " "; });

		destination->send_message(prefix + names + "\n", "");
		i = i_end;
	} while (i != strings.end());
}

bool RoomList::send_user_list(const string& room, sendable* destination)
{
	if (rooms.find(room) == rooms.end())
		return false;
	vector<string> user_names;
	// one user is always waiting for a connection
	int count = rooms[room]->get_user_names(user_names) - 1;
	int anon_count = count - user_names.size();

	string msg = "%user-list ";

	destination->send_message(msg + to_string(count) + 
	                          " " + to_string(anon_count) + "\n", "");

	send_str_list(msg, NAMES_PER_LINE, user_names, destination);

	return true;
}

bool RoomList::send_room_list(sendable* destination)
{
	vector<string> room_names;
	for (auto room : rooms) 
		room_names.push_back(room.first);
	int count = room_names.size();

	string msg = "%room-list ";

	destination->send_message(msg + to_string(count) + "\n", "");
	
	send_str_list(msg, NAMES_PER_LINE, room_names, destination);

	return true;
}

bool RoomList::join(ChatUser* user, const string& room)
{
	if (!rooms[DEF_ROOM]->find_user(room) &&
	    add_user(user->shared_from_this(), room))
		return true;
	else
		return false;
}

shared_ptr<sendable> RoomList::find(const std::string& name)
{
	for (auto room : rooms)
		if (room.first == name)
			return shared_ptr<sendable>(room.second);
	return rooms[DEF_ROOM]->find_user(name);
}
