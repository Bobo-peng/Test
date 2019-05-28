// sub.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include"zmq.h"
#include"zmq_utils.h"

int main(void)
{
	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket(context, ZMQ_SUB);

	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
	zmq_connect(subscriber, "tcp://127.0.0.1:9998");
	while(1)
	{
		char recvbuf[64] = {0};
		int ret = zmq_recv(subscriber, recvbuf, 64, ZMQ_DONTWAIT);//ZMQ_DONTWAIT��ʾzmq_recv����Ϊ������
		if (ret == -1)
		{
			if (errno == EAGAIN) continue;
			else
			{
				std::cout << "���մ���"<< std::endl;
				break;
			}
		}
			std::cout << "�����ߣ�"<<recvbuf << std::endl;
	}
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	getchar();
	return 0;
}