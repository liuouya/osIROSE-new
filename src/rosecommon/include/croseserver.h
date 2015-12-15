#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include <list>
#include "cnetwork_asio.h"

class CRoseClient;
class CRoseISC;

class CRoseServer : public CNetwork_Asio
{
public:
	CRoseServer( bool _iscServer = false );
	virtual ~CRoseServer( );

	virtual bool Run( );

	bool IsISCServer() { return m_ISCServer; }

	static std::list< CRoseClient* > &GetClientList() { return m_ClientList; }
	static std::list< CRoseISC* > &GetISCList() { return m_ISCList; }
	static std::mutex &GetListMutex() { return m_ClientListMutex; }

protected:
	// Callback functions
	virtual bool OnConnect( );
	virtual void OnConnected( );
	virtual bool OnListen( );
	virtual void OnListening( );
	virtual bool OnDisconnect( );
	virtual void OnDisconnected( );
//	virtual bool OnReceive( );
//	virtual void OnReceived( uint8_t* _buffer, uint16_t _size );
//	virtual bool OnSend( uint8_t* _buffer );
//	virtual void OnSent( );
	virtual bool OnAccept( );
	virtual void OnAccepted( tcp::socket _sock );

	bool m_ISCServer;
	static std::list< CRoseClient* > m_ClientList;
	static std::list< CRoseISC* > m_ISCList;
	static std::mutex m_ClientListMutex;
};

#endif
