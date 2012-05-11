#include <algorithm>
#include <iostream>

#include <boost/asio.hpp>

#include "server.h"
#include "cmdproc.h"

using namespace std;
using namespace boost;

int main()
{
	CmdProcessor().test_parser();
	try  {
		asio::io_service io_service;
		Server server(io_service, 48879);
		io_service.run();

	} catch(std::exception &e) {
		cout << "Exception: " << e.what() << endl;
	}

	return 0;
}

