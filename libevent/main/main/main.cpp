// main.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include <sys/types.h>
#include <event2/event-config.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <sys/queue.h>
#include <unistd.h>
#endif
#include <time.h>
#ifdef EVENT__HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

int Connect(const char* server_ip, int port)
{
	WORD wVR;
	WSADATA wsaData;
	wVR = MAKEWORD(1, 1);
	int nError = WSAStartup(wVR, &wsaData);
	if (nError != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		WSACleanup();
		return -1;
	}

	struct sockaddr_in server_in;
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(port);
	server_in.sin_addr.S_un.S_addr = inet_addr(server_ip);
	int status = connect(sockfd, (struct sockaddr*)&server_in, sizeof(server_in));

	if (status == SOCKET_ERROR)
	{
		closesocket(sockfd);//关闭socket
		WSACleanup();
		return -1;
	}
	evutil_make_socket_nonblocking(sockfd);//设置为非阻塞模式
	return sockfd;
}

void RecvData(int fd, short events, void* arg)
{
	char msg[256] = { 0 };
	struct event *ev = (struct event*)arg;
	int ret = recv(fd, msg, 256, 0);
	if (ret <= 0)
	{
		closesocket(fd);
		return;
	}
	std::cout << "recv " << ret << "bytes:" << msg << std::endl;
	const char* sendmsg = "recv success!";
	ret = send(fd, sendmsg, strlen(sendmsg), 0);
	if (ret <= 0)
	{
		closesocket(fd);
		return;
	}
	return;
}
bool startEvents()
{
	int sockfd = Connect("127.0.0.1", 8999);
	if (sockfd == -1)
	{
		return false;
	}
	struct event_base* base = event_base_new();

	struct event *ev_sockfd = event_new(base, sockfd, EV_READ | EV_PERSIST, RecvData, NULL);//可读事件
	event_add(ev_sockfd, NULL);//添加事件
	event_base_dispatch(base);//循环监听事件
	return false;
}

int main()
{
	startEvents();//启动
    return 0;
}

