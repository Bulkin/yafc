#include "cmdproc.h"

#include <boost/regex.hpp>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace boost;

bool CmdProcessor::is_cmd(const std::string& msg)
{
	return msg[0] == CMD_PREFIX[0];
}

void CmdProcessor::set(string name, cmd_func handler)
{
	handlers[name] = handler;
}

bool CmdProcessor::parse(string cmd_msg, ChatUser* sender)
{
	if (!is_cmd(cmd_msg))
		return false;
	regex re("[a-zA-Z0-9_-]+");
	sregex_token_iterator i(cmd_msg.begin(), cmd_msg.end(), re, 0);
	string_list tokens;
	for_each(i, sregex_token_iterator(), 
	         [&tokens](string it)
	         { tokens.push_back(it); });
	if (handlers.find(tokens.front()) != handlers.end())
		handlers[tokens.front()](sender, tokens);
	else
		return false;
	return true;
}

bool CmdProcessor::test_parser()
{
	handlers[string("test")] = [](ChatUser* user, string_list params)
		{ for_each(params.begin(), params.end(), 
		           [](string s ){ cout << s << " "; }); };
	string cmd = "%test param1 param2";
	cout << parse(cmd, NULL) << endl;
}
