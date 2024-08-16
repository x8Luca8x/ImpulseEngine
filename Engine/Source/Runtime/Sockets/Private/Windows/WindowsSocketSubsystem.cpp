#include "Windows/WindowsSocketSubsystem.h"

#if PLATFORM_WINDOWS

FName NAME_WindowsSocketSubsystem = FName(TEXT("Windows"));

FWindowsSocketSubsystem* FWindowsSocketSubsystem::Create()
{
	FWindowsSocketSubsystem* socketSubsystem = new FWindowsSocketSubsystem();
	if (!socketSubsystem->Init())
	{
		delete socketSubsystem;
		socketSubsystem = nullptr;
	}

	return socketSubsystem;
}

bool FWindowsSocketSubsystem::Init()
{
	constexpr WORD version = MAKEWORD(2, 2);
	return WSAStartup(version, &m_WsaData) == 0;
}

void FWindowsSocketSubsystem::Shutdown()
{
	WSACleanup();
}

FSocketPtr FWindowsSocketSubsystem::CreateSocket(const ESocketType SocketType, const FString& SocketDescription, const ESocketFamily ProtocolType)
{
	return FSocketPtr();
}

TArray<FAddrInfoPtr> FWindowsSocketSubsystem::ResolveAddress(const FString& Host, const FString& Service, const ESocketFamily ProtocolType, const ESocketType SockType)
{
	return TArray<FAddrInfoPtr>();
}


#endif