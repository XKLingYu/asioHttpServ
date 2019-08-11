#include "session.h"
#include <fstream>
#include <cstdio>

void Session::work()
{
	//printf("进入会话\n");
	boost::asio::async_read_until(
		_socket, _stream_buffer, '\n', [this, self = shared_from_this()](boost::system::error_code e, size_t slen) {
			if (!e) {
				_readRequest();
				work();
			}
			else {
				_socket.close();
			}
		}
	);
}

void Session::_readRequest()
{
	std::istream is(&_stream_buffer);
	std::string tmp;
	std::getline(is, tmp, '\n');
	//printf("%s\n", tmp.data());
	if (tmp.find("GET") != std::string::npos) {
		_respondGet(tmp);
	}
}

void Session::_respondGet(const std::string& s)
{
	std::string tmp = s.substr(s.find("GET ") + 5);//GET /
	std::string request;
	size_t flag = tmp.find('?');
	if (flag != std::string::npos) {
		request = tmp.substr(0, flag);
	}
	else {
		request = tmp.substr(0, tmp.find(' '));
	}
	std::string fileCon;

	if(_readFile(request, fileCon)) {//有文件
		_setContent_Type(request);
		_response += "\r\nContent-Length: " + std::to_string(fileCon.length()) + "\r\n\r\n" + fileCon;
	}
	else {//没文件 返回404
		_response = "HTTP/1.1 404 Not Found\r\nConnection: keep-alive\r\nContent-Type: text/html";
	}

	boost::asio::async_write(
		_socket, boost::asio::buffer(_response, _response.size() + 1),
		[this, self = shared_from_this()](boost::system::error_code e, size_t) {
			if (e) {
				_socket.close();
			}
		}
	);

}

bool Session::_readFile(const std::string& path, std::string& outf)
{
	std::ifstream inf(path,std::ios::binary);
	if (!inf) {
		return false;
	}
	std::stringstream sstr;
	while (inf >> sstr.rdbuf());
	outf = sstr.str();
	return true;
}

void Session::_setContent_Type(const std::string& request)
{
	std::regex regImg(".+\.(gif|jpg|png|bmp)$");
	std::smatch s;
	if (std::regex_match(request, s, regImg)) {
		_response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\ncontent-type: image/" + s.str(s.size() - 1);
	}
	else if (request.find(".css") != std::string::npos) {
		_response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\ncontent-type: text/css; charset=utf-8";
	}
	else if(request.find(".js") != std::string::npos) {
		_response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\ncontent-type: text/javascript";
	}
	else {
		_response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html";
	}
}

