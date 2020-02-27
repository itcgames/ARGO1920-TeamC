#pragma once
#include <WinSock2.h> //For SOCKET
#include <string> //For std::string
#include "FileTransferData.h" //For FileTransferData class
#include "PacketManager.h" //For PacketManager class

class Client
{
public: //Public functions
	Client(const char* ip, const int port, std::string& t_gameData, std::string& t_winData, std::string& t_connectionData, std::string& t_gameStartData, std::string& t_restartData);
	bool Connect();
	void Disconnect();
	void SendString(const std::string& str);
	//void SendGameData(const std::string& t_data);
	//void SendWinData(const std::string& t_data);
	//void SendConnectData(const std::string& t_data);
	//void SendStartGame(const std::string& t_data);
	bool RequestFile(const std::string& fileName);
	bool getConnected();
	~Client();
private: //Private functions
	bool CloseConnection();
	bool ProcessPacketType(const PacketType packetType);
	static void ClientThread(Client& client); //Client thread which reads messages from server
	static void PacketSenderThread(Client& client); //Packet sender thread which sends out packets existing in packet manager
	//Sending Funcs
	bool sendall(const char* data, const int totalBytes);
	//Getting Funcs
	bool recvall(char* data, const int totalBytes);
	bool Getint32_t(std::int32_t& int32_t);
	bool GetPacketType(PacketType& packetType);
	bool GetString(std::string& str);
	void joinThreads();
private: //Private variables
	bool m_terminateThreads = false;
	bool m_isConnected = false;
	FileTransferData m_file; //Object that contains information about our file that is being received from the server.
	SOCKET m_connection;//This client's connection to the server
	SOCKADDR_IN m_addr; //Address to be binded to our Connection socket
	PacketManager m_pm; //Packet manager which sends all of client's outgoing packets

	std::thread m_pst; //Create thread to send packets
	std::thread m_ct; //Create thread to listen to server

	std::string& m_gameData;
	std::string& m_winData;
	std::string& m_connectData;
	std::string& m_startData;
	std::string& m_restartData;
};