#pragma once
#include "session.h"
#include <boost/asio.hpp>
#include <memory>
#include <fstream>
#include <cstdio>
using boost::asio::ip::tcp;

class HttpdServer
{
public:
	HttpdServer(boost::asio::io_context& ioc , short port, const char* logName)
		:_acceptor(ioc, tcp::endpoint(tcp::v4(), port)),
	_clntSocket(ioc)
	{
		_acceptor.listen();
	}
	void start();

private:
	tcp::acceptor _acceptor;
	tcp::socket _clntSocket;
};

