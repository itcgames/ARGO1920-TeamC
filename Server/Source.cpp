#include "Server.h"
#include <chrono>
#include <iostream>

int main()
{
	Server MyServer(1111, false); //Create server on port 1111, false=do not loopback to localhost (others can connect)
	auto t1 = std::chrono::high_resolution_clock::now();
	float startTime = clock();
	float endTime = clock();
	bool running = true;
	std::cout << "Server started!" << std::endl;
	const int TIME_OUT = 5.0f;
	while (running && !MyServer.killServer())
	{
		std::cout << "Before ifs" << std::endl;
		if (MyServer.getConnectionCount() == 0 && (endTime - startTime) / (double)CLOCKS_PER_SEC <= TIME_OUT)
		{
			std::cout << "0 connections and time less than timeout" << std::endl;
			MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
			endTime = clock();
		}
		if (MyServer.getConnectionCount() == 0 && (endTime - startTime) / (double)CLOCKS_PER_SEC > TIME_OUT)
		{
			std::cout << "Server timed out" << std::endl;
			running = false;
		}
		else if (MyServer.getConnectionCount() > 0)
		{
			std::cout << "1 or more connections" << std::endl;
			MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
			startTime = clock();
			endTime = clock();
		}
		std::cout << ((endTime - startTime) / (double)CLOCKS_PER_SEC) << std::endl;
	}
	std::cout << "Server timed out" << std::endl;
	system("pause");
	return 0;
}