#include "userlist.h"

using namespace std;

UserList::user_ptr& UserList::add(const user_ptr& user)
{
	users.push_back(user);
	return users.back();
}

size_t UserList::count()
{
	return users.size();
}

void UserList::cleanup()
{
	users.remove_if([](const user_ptr& user)
	                { return user->get_status() == ChatUser::DISCONNECTED; });
}

void UserList::send_message(const string& msg)
{
	cleanup();
	for (auto &user : users)
		if (user->get_status() == ChatUser::CONNECTED)
			user->send_message(msg);
}

const UserList::user_ptr& UserList::find(const string& name) const
{
	for (auto &user : users)
		if (user->get_name() == name)
			return user;
	return nullptr;
}
