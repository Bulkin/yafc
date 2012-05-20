#ifndef SENDABLE_H
#define SENDABLE_H

#include <string>

class sendable
{
public:
	virtual void send_message(const std::string& message,
	                          const std::string& destination) = 0;
};

#endif
