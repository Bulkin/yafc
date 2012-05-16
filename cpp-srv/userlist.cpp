#include "userlist.h"

using namespace std;

UserList::user_ptr UserList::add(user_ptr user)
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

void UserList::send_message(const string& msg, const string& dest)
{
	cleanup();
	for (auto &user : users)
		if (user->get_status() == ChatUser::CONNECTED)
			user->send_message(msg, dest);
}

const UserList::user_ptr& UserList::find_user(const string& name) const
{
	for (auto &user : users)
		if (user->get_name() == name)
			return user;
	return nil;
}

int UserList::get_user_names(vector<string>& res_list)
{
	for (auto &user : users)
		if (user->get_name() != DEF_NAME)
			res_list.push_back(user->get_name());
	return users.size();
}
