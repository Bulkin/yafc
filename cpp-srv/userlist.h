#include <list>
#include <string>
#include <memory>

#include "chatuser.h"
#include "sendable.h"

class UserList : public sendable
{
public:
	typedef std::shared_ptr<ChatUser> user_ptr;

	user_ptr add(user_ptr user);
	void send_message(const std::string& msg,
	                  const std::string& dest);
	const user_ptr& find_user(const std::string& name) const;

	std::shared_ptr<sendable> find(const std::string& name);

	void cleanup();
	size_t count();
private:
	std::list<user_ptr> users;
	const user_ptr null = nullptr;
};
