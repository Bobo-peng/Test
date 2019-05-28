// pub.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include"zmq.h"
#include"zmq_utils.h"

int main() {
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUB);
	zmq_bind(publisher, "tcp://*:9998");
	char sendbuf[64] = {0};
	int cnt = 0;
	while (cnt < 1000)
	{
		sprintf_s(sendbuf, "msg消息----%d", cnt++);
		std::cout << "发布者：msg消息----" << cnt << std::endl;
		zmq_send(publisher, sendbuf, strlen(sendbuf), 0);
		Sleep(1000);
		
	}
	std::cout << "publisher over!" << std::endl;
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	getchar();
	return 0;
}