
// Protocol17x.h

/*
Declares the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
(others may be added later in the future for the 1.7 release series)
*/





#pragma once

#include "Protocol16x.h"





class cProtocol172 :
	public cProtocol162  // TODO
{
	typedef cProtocol162 super;  // TODO
	
public:

	cProtocol172(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);
	
	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;

protected:

	AString m_ServerAddress;
	
	UInt16 m_ServerPort;
	
	/// State of the protocol. 1 = status, 2 = login
	UInt32 m_State;

	/// Buffer for the received data
	cByteBuffer m_ReceivedData;
	
	bool m_IsEncrypted;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption m_Decryptor;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption m_Encryptor;
	
	/// (Unencrypted) data to be sent to the client. Written by SendData, cleared by Flush()
	AString m_DataToSend;
	
	
	/// Adds the received (unencrypted) data to m_ReceivedData, parses complete packets
	void AddReceivedData(const char * a_Data, int a_Size);
	
	/// Reads and handles the packet. The packet length and type have already been read.
	void HandlePacket(UInt32 a_PacketType, UInt32 a_RemainingBytes);
	
	// Packet handlers while in the Status state (m_State == 1)
	void HandlePacketStatusRequest(UInt32 a_RemainingBytes);
	void HandlePacketStatusPing   (UInt32 a_RemainingBytes);
	
	/// Writes an entire packet into the output stream. a_Packet is expected to start with the packet type; data length is prepended here.
	void WritePacket(cByteBuffer & a_Packet);

	/// Adds unencrypted data to the outgoing data buffer
	virtual void SendData(const char * a_Data, int a_Size) override;

	/// Flushes m_DataToSend through the optional encryption into the outgoing socket data
	virtual void Flush(void) override;
} ;




