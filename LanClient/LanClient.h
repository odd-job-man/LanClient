#pragma once
#include <windows.h>

struct LanClientSession;

class SmartPacket;
class Packet;
struct LanClientSession;
#include "CLockFreeQueue.h"
#include "CLockFreeStack.h"
#include <MSWSock.h>

class LanClient
{
public:
	LanClient();
	~LanClient();
	void InitialConnect();
	void SendPacket(ULONGLONG id, SmartPacket& sendPacket);
	void SendPacket_ALREADY_ENCODED(ULONGLONG id, Packet* pPacket);
	virtual void OnRecv(ULONGLONG id, Packet* pPacket) = 0;
	virtual void OnError(ULONGLONG id, int errorType, Packet* pRcvdPacket) = 0;
	virtual void OnConnect(ULONGLONG id) = 0;
	virtual void OnRelease(ULONGLONG id) = 0;
	virtual void OnConnectFailed(ULONGLONG id) = 0;
	void Disconnect(ULONGLONG id);
protected:
	bool ConnectPost(LanClientSession* pSession);
	bool DisconnectExPost(LanClientSession* pSession);
	BOOL SendPost(LanClientSession* pSession);
	BOOL RecvPost(LanClientSession* pSession);
	void ReleaseSession(LanClientSession* pSession);
	void RecvProc(LanClientSession* pSession, int numberOfBytesTransferred);
	void SendProc(LanClientSession* pSession, DWORD dwNumberOfBytesTransferred);
	void ConnectProc(LanClientSession* pSession);
	void DisconnectProc(LanClientSession* pSession);
	friend class Packet;
	static unsigned __stdcall IOCPWorkerThread(LPVOID arg);
	static bool SetLinger(SOCKET sock);
	static bool SetZeroCopy(SOCKET sock);
	static bool SetReuseAddr(SOCKET sock);
	static bool SetClientBind(SOCKET sock);

	DWORD IOCP_WORKER_THREAD_NUM_ = 0;
	DWORD IOCP_ACTIVE_THREAD_NUM_;
	LONG lSessionNum_ = 0;
	LONG maxSession_;
	ULONGLONG ullIdCounter_ = 0;
	LanClientSession* pSessionArr_;
	HANDLE* hIOCPWorkerThreadArr_;
	CLockFreeStack<short> DisconnectStack_;
	HANDLE hcp_;
	LPFN_CONNECTEX lpfnConnectExPtr_;
	LPFN_DISCONNECTEX lpfnDisconnectExPtr_;
	SOCKADDR_IN sockAddr_;
};

#include "Packet.h"
#include "RingBuffer.h"
#include "MYOVERLAPPED.h"
#include "LanSession.h"
