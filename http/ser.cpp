#include "head.h"
#include <iostream>
using namespace std;
const int DIS_PORT = 7800;
const int SER_PORT = 8000;
const char* ip = "127.0.0.1";

class CSockInit
{
public:
	static CSockInit *GetSockfd()
	{
		return &sockClass;
	}

	int Accept(struct sockaddr* addr, socklen_t *len)
	{
		int c = accept(_sockfd, addr, len);	
		if(c < 0)
		{
			cout << "--------" << endl;
			throw "accept error\n";
		}
		return c;
	}
	void send_response(int c, string url)
	{
		string str("HTTP/1.0 Moved Permanently\r\n");
		str += "Server: LOU-server\r\n";
		str += "Location: ";
		str += url;
		str += "\r\n";

		cout << str << endl;
		cout << str.length() << endl;
		send(c, str.c_str(), str.length(),0);
	}	

	ssize_t Send(int c, char *buff, const int size)
	{
		return send(c, buff, size, 0);
	}
	
	ssize_t Recv(int c, char *buff, const int size)
	{
		return recv(c, buff, size, 0);
	}


	~CSockInit()
	{
		close(_sockfd);
	}

	int _sockfd;

private:
	CSockInit()
	{
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		assert(_sockfd != -1);

		struct sockaddr_in saddr;
		memset(&saddr, 0, sizeof(saddr));
		
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(SER_PORT);
		saddr.sin_addr.s_addr = inet_addr(ip);
		int res = bind(_sockfd, (struct sockaddr*)&saddr, sizeof(saddr));

		listen(_sockfd, 5);
	}
private:
	

	static CSockInit sockClass;
};
CSockInit CSockInit::sockClass;


int main()
{
	CSockInit *psock = CSockInit::GetSockfd();
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);

	bool isRuning = true;
	while(isRuning)
	{
		cout << "connect.....\n" << endl;
		//int c = accept(psock->_sockfd,(struct sockaddr*)&addr, &len);
		int c = psock->Accept((struct sockaddr*)&addr, &len);
		if(c < 0)
			isRuning = false;
		
		char *buff = new char[1024];
		int size;
		if((size = psock->Recv(c, buff, 1023)) < 0)
		{
			isRuning = false;
			close(c);
		}
		cout << buff << endl;

		delete[]buff;
	}
	
	return 0;
}
