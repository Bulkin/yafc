#ifndef ROOMLIST_H
#define ROOMLIST_H

#include <unordered_map>
#include <string>
#include <memory>

#include "userlist.h"
#include "cmdproc.h"
#include "sendable.h"


class RoomList : public sendable
{
public:
	RoomList();
	UserList::user_ptr add_user(UserList::user_ptr user,
	                            const std::string& = DEF_ROOM);
	void send_message(const std::string& message,
	                  const std::string& destination);
	bool send_user_list(const std::string& room, sendable* destination);
	void send_room_list(sendable* destination);
	
	std::shared_ptr<sendable> find(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<UserList>> rooms;
	
	/* send joined strings with no more than max strings per line */
	void send_str_list(const std::string& prefix, int max,
	                   std::vector<std::string>& strings, 
	                   sendable* destination);
};

#endif
