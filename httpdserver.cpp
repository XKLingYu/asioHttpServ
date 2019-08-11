#include "httpdserver.h"

void HttpdServer::start()
{
	_acceptor.async_accept(
		_clntSocket, [this](boost::system::error_code e) {
			if (!e) {
				std::make_shared<Session>(std::move(_clntSocket))->work();
			}
			start();
		}
	);
}
