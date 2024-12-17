#pragma once
struct LanClientSession
{
	static constexpr LONG RELEASE_FLAG = 0x80000000;
	SOCKET sock_ = INVALID_SOCKET;
	ULONGLONG id_;
	ULONGLONG lastRecvTime;
	LONG lSendBufNum_;
	BOOL bDisconnectCalled_;
	MYOVERLAPPED connectOverlapped;
	MYOVERLAPPED sendOverlapped;
	MYOVERLAPPED recvOverlapped;
	MYOVERLAPPED disconnectOverlapped;
	LONG IoCnt_;
	CLockFreeQueue<Packet*> sendPacketQ_;
	BOOL bSendingInProgress_;
	Packet* pSendPacketArr_[50];
	RingBuffer recvRB_;
	BOOL Init(ULONGLONG ullClientID, SHORT shIdx);

#pragma warning(disable : 26495)
	LanClientSession()
		:IoCnt_{ LanClientSession::RELEASE_FLAG | 0 }
	{}
#pragma warning(default: 26495)

	__forceinline static short GET_SESSION_INDEX(ULONGLONG id)
	{
		return id & 0xFFFF;
	}

};
