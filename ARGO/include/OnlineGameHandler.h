#pragma once
#include "Client.h"
#include <iostream>
#include <string>

class OnlineGameHandler
{
public:
	OnlineGameHandler();
	~OnlineGameHandler();

	void connectToServer(std::string t_ip, int t_port);
	void disconnect();
	std::string& getGameData();
	std::string& getWinData();
	std::string& getStartData();
	std::string& getRestartData();
	std::string& getConnectData();
	bool isConnected();
	void sendGameData(int t_posX, int t_posY);
	void sendWinData(int t_ms);
	void sendConnectData();
	void sendStartData();
	void sendStartData(std::string t_playerType);

private:
	void clearReceivedData();
	std::string m_gameData;
	std::string m_winData;
	std::string m_connectData;
	std::string m_gameStartData;
	std::string m_restartData;
	Client* m_client;

	const std::string LOCAL_HOST = "127.0.0.1";
	const std::string CONNECT_DATA = "self:cd:";
	const std::string GAME_DATA = "other:gd:";
	const std::string START_DATA = "other:sd:";
	const std::string WIN_DATA = "all:wd:";

};