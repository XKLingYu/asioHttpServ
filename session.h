#pragma once
#include <boost\asio.hpp>
#include <memory>
#include <cstdio>
#include <regex>
using boost::asio::ip::tcp;
class Session: public std::enable_shared_from_this<Session>
{
public:
	~Session() { printf("连接断开\n"); }
	Session(tcp::socket socket) 
		:_socket(std::move(socket)){ printf("有新的连接\n"); }
	void work();
	tcp::socket& getSocket() { return _socket; }
private:
	void _readRequest();
	void _respondGet(const std::string&);
	bool _readFile(const std::string&, std::string&);
	void _setContent_Type(const std::string&);
	tcp::socket _socket;
	boost::asio::streambuf _stream_buffer;
	std::string _response;
};

