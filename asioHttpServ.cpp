#include "httpdserver.h"
#include <iostream>
using namespace std;

int main()
{
	try {
		boost::asio::io_context ioc;
		HttpdServer server(ioc, 4396, "log.txt");
		server.start();
		ioc.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception " << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
}