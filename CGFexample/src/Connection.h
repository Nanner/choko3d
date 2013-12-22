#ifndef _CONNECTION_
#define _CONNECTION_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef __APPLE__
#define linux
#endif

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string>
#include <iostream>

using namespace std;

class Connection {
public:
    Connection(short port);
	~Connection();
    
    string sendMsg(string msg);
    string readLine();
private:
#ifdef linux
    int sock;
#else
    SOCKET sock;
#endif
};

#endif
