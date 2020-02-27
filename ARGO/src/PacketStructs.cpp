#include "stdafx.h"
#include "PacketStructs.h"

namespace PS
{
	ChatMessage::ChatMessage(const std::string& msg)
		:m_message(msg)
	{
	}

	std::shared_ptr<Packet> ChatMessage::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::ChatMessage);
		p->Append(m_message.size());
		p->Append(m_message);
		return p;
	}

	std::shared_ptr<Packet> FileDataBuffer::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::FileTransferByteBuffer);
		p->Append(m_size);
		p->Append(m_databuffer, m_size);
		return p;
	}

	//GameData::GameData(const std::string& t_msg) :
	//	m_gameData(t_msg)
	//{
	//}

	//std::shared_ptr<Packet> GameData::toPacket()
	//{
	//	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	//	p->Append(PacketType::GameData);
	//	p->Append(m_gameData.size());
	//	p->Append(m_gameData);
	//	return p;
	//}

	//WinData::WinData(const std::string& t_str) :
	//	m_message{ t_str }
	//{
	//}

	//std::shared_ptr<Packet> WinData::toPacket()
	//{
	//	std::shared_ptr<Packet> packet = std::make_shared<Packet>();
	//	packet->Append(PacketType::WinData);
	//	packet->Append(m_message.size());
	//	packet->Append(m_message);
	//	return packet;
	//}

	//ConnectData::ConnectData(const std::string& t_str) :
	//	m_connectData(t_str)
	//{
	//}

	//std::shared_ptr<Packet> ConnectData::toPacket()
	//{
	//	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	//	p->Append(PacketType::ConnectData);
	//	p->Append(m_connectData.size());
	//	p->Append(m_connectData);
	//	return p;
	//}
}