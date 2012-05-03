#include <list>
#include <string>
#include <memory>

#include "chatuser.h"

class UserList
{
public:
	typedef std::shared_ptr<ChatUser> user_ptr;

	user_ptr& add(const user_ptr& user);
	void send_message(const std::string& msg);
	const user_ptr& find(const std::string& name) const;

	void cleanup();
	size_t count();
private:
	std::list<user_ptr> users;
	
};
