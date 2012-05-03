#include <algorithm>
#include <iostream>

#include <boost/asio.hpp>

#include "server.h"

using namespace std;
using namespace boost;

void test_ptr(asio::io_service &io_service)
{
	UserList::user_ptr user(new ChatUser(io_service,
	                                     [](string a, string b)
	                                     { cout << a << b << endl; }));
	UserList users;
	users.add(user)->disconnect();
	users.add(UserList::user_ptr(new ChatUser(io_service,
	                                          [](string a, string b)
	                                          { cout << b << a << endl; })));
	users.cleanup();
	cout << users.count() << endl;
}


int main()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	for (auto &x : arr)
		x += 10;

	for_each(arr, arr+5, [](int p){ cout << p;});
	cout << endl;
	
	try  {
		asio::io_service io_service;
		Server server(io_service, 48879);
		test_ptr(io_service);
		io_service.run();

	} catch(std::exception &e) {
		cout << "Exception: " << e.what() << endl;
	}

	return 0;
}

