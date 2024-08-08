#pragma once

#include "SocketTypes.h"

#include "Templates/SharedPtr.h"

class FSocket;
class FAddrInfo;

class ISocketSubsystem;

typedef TSharedPtr<FSocket, ESPMode::ThreadSafe> FSocketPtr;
typedef TSharedPtr<FAddrInfo, ESPMode::ThreadSafe> FAddrInfoPtr;

enum ESockCreationFlags : int32
{
	None = 0,

	/** Whether the socket should be saved in the socket subsystem's list of sockets. */
	SOCK_CLOEXEC = 1 << 0
};

/**
* Registry for all socket subsystems that are available for use.
*/
class SOCKETS_API FSocketSubsystemRegistry
{
public:

	/**
	* Gets the singleton instance of the socket subsystem registry.
	* @return The singleton instance.
	*/
	static FSocketSubsystemRegistry& Get();

protected:

	/** Default constructor. */
	FSocketSubsystemRegistry();

public:

	/**
	* Registers a socket subsystem with the registry.
	* @param SubsystemName - The name of the subsystem.
	* @param Subsystem - The subsystem to register.
	*/
	void RegisterSocketSubsystem(const FName SubsystemName, ISocketSubsystem* Subsystem);

	/**
	* Unregisters a socket subsystem with the registry.
	* @param SubsystemName - The name of the subsystem.
	*/
	void UnregisterSocketSubsystem(const FName SubsystemName);

private:

	/** Map of all socket subsystems that are available for use. */
	TMap<FName, ISocketSubsystem*> SocketSubsystems;
};

/**
* Base class for all socket subsystems.
* A socket subsystem is a platform-specific implementation of the sockets API.
* It provides a way to create sockets and perform socket operations.
* The socket subsystem is responsible for managing the lifetime of sockets and ensuring that they are properly cleaned up.
*/
class SOCKETS_API ISocketSubsystem
{
public:

public:

	/**
	* Creates a new socket.
	* @param SocketType - The type of socket to create.
	* @param SocketDescription - A description of the socket.
	* @param ProtocolType - The protocol type to use for the socket.
	* @param Flags - Flags to use when creating the socket.
	* @return The new socket.
	*/
	virtual FSocketPtr CreateSocket(const ESocketType SocketType, const FString& SocketDescription, const ESocketFamily ProtocolType, ESockCreationFlags Flags = SOCK_CLOEXEC) = 0;

	/**
	* Resolves an address
	* @param Host - The host name to resolve.
	* @param Service - The service name to resolve.
	* @param ProtocolType - The protocol type to use for the address.
	* @param Flags - Flags to use when resolving the address.
	*/
	virtual TArray<FAddrInfoPtr> ResolveAddress(const FString& Host, const FString& Service, const ESocketFamily ProtocolType = ESocketFamily::Invalid, const ESocketType SockType = ESocketType::Stream) = 0;
};