#pragma once

#include "SocketSubsystem.h"

#if PLATFORM_WINDOWS

extern SOCKETS_API FName NAME_WindowsSocketSubsystem;

#define DEFAULT_PLATFORM_SOCKET_SUBSYSTEM NAME_WindowsSocketSubsystem

class FWindowsSocketSubsystem : public ISocketSubsystem
{
public:

	static FWindowsSocketSubsystem* Create();

public:

	//~ ISocketSubsystem interface

	bool Init() override;
	void Shutdown() override;
	FSocketPtr CreateSocket(const ESocketType SocketType, const FString& SocketDescription, const ESocketFamily ProtocolType) override;
	TArray<FAddrInfoPtr> ResolveAddress(const FString& Host, const FString& Service, const ESocketFamily ProtocolType = ESocketFamily::Invalid, const ESocketType SockType = ESocketType::Stream) override;

	//~ ISocketSubsystem interface

private:

	WSADATA m_WsaData{};
};

#endif