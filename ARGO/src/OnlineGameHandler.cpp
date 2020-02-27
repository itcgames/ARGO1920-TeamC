#include "stdafx.h"
#include "OnlineGameHandler.h"

OnlineGameHandler::OnlineGameHandler() :
	m_client(nullptr)
{
}

OnlineGameHandler::~OnlineGameHandler()
{
	if (m_client)
	{
		if (m_client->getConnected())
		{
			m_client->Disconnect();
		}
		delete m_client;
	}
	m_client = nullptr;
}

void OnlineGameHandler::connectToServer(std::string t_ip, int t_port)
{
	if (m_client)
	{
		disconnect();
	}

	if (t_ip.empty())
	{
		m_client = new Client(LOCAL_HOST.c_str(), t_port, m_gameData, m_winData, m_connectData, m_gameStartData, m_restartData);
	}
	else
	{
		m_client = new Client(t_ip.c_str(), t_port, m_gameData, m_winData, m_connectData, m_gameStartData, m_restartData);
	}

	if (!m_client->Connect())
	{
		std::cout << "Failed to connect to server..." << std::endl;
	}
	else
	{
		sendConnectData();
	}
}


void OnlineGameHandler::disconnect()
{
	clearReceivedData();
	if (m_client)
	{
		if (m_client->getConnected())
		{
			m_client->Disconnect();
		}
		delete m_client;
	}
	m_client = nullptr;
}

std::string& OnlineGameHandler::getGameData()
{
	return m_gameData;
}

std::string& OnlineGameHandler::getWinData()
{
	return m_winData;
}

std::string& OnlineGameHandler::getStartData()
{
	return m_gameStartData;
}

std::string& OnlineGameHandler::getRestartData()
{

	return m_restartData;
}

bool OnlineGameHandler::isConnected()
{
	if (m_client)
	{
		return m_client->getConnected();
	}
	else
	{
		return false;
	}
}

void OnlineGameHandler::sendGameData(glm::vec2 t_pos)
{
	std::string data = GAME_DATA;
	data += std::to_string(t_pos.x);
	data += ",";
	data += std::to_string(t_pos.y);
	std::cout << "Sent data: " << data << std::endl;
	m_client->SendString(data);
}

void OnlineGameHandler::sendWinData(int t_ms)
{
	std::string data = WIN_DATA;
	data += std::to_string(t_ms);
	m_client->SendString(data);
}

void OnlineGameHandler::sendConnectData()
{
	std::string data = CONNECT_DATA;
	m_client->SendString(data);
}

void OnlineGameHandler::sendStartData()
{
	std::string data = START_DATA;
	m_client->SendString(data);
}

void OnlineGameHandler::sendStartData(std::string t_playerType)
{
	std::string data = START_DATA;
	data += t_playerType;
	m_client->SendString(data);
}

void OnlineGameHandler::clearReceivedData()
{
	m_gameData = "";
	m_winData = "";
	m_connectData = "";
	m_gameStartData = "";
	m_restartData = "";
}

std::string& OnlineGameHandler::getConnectData()
{
	return m_connectData;
}