#ifndef CMDPROC_H
#define CMDPROC_H

#include <functional>
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

#include "chatuser.h"

class CmdProcessor
{
public:
	typedef std::list<std::string> string_list;
	typedef std::function<bool(ChatUser* sender, 
	                           string_list params)> cmd_func;
	
	void set(std::string name, cmd_func handler);
	bool parse(std::string cmd_msg, ChatUser* sender);

	bool test_parser();
private:
	std::unordered_map<std::string, cmd_func> handlers;

	bool is_cmd(const std::string& msg);
};

#endif
