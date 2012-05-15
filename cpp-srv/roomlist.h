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
	std::unordered_map<std::string, std::shared_ptr<UserList>> rooms;
public:
	RoomList();
	UserList::user_ptr add_user(UserList::user_ptr user,
	                            const std::string& = DEF_ROOM);
	void send_message(const std::string& message,
	                  const std::string& destination);
	
	std::shared_ptr<sendable> find(const std::string& name);

};

#endif
