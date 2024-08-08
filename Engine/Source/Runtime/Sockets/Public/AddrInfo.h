#pragma once

#include "SocketsModule.h"
#include "SocketTypes.h"

#include "Containers/ImpulseString.h"

class SOCKETS_API FAddrInfo
{
public:

	/**
	* Whether the address info is ip v4.
	* @return True if the address info is ip v4, false otherwise.
	*/
	inline bool IsIPv4() const { return GetSocketFamily() == ESocketFamily::IPv4; }

	/**
	* Whether the address info is ip v6.
	* @return True if the address info is ip v6, false otherwise.
	*/
	inline bool IsIPv6() const { return GetSocketFamily() == ESocketFamily::IPv6; }

public:

	/**
	* Gets the ip address of the address info.
	* @return The ip address as string.
	*/
	virtual FString GetIPAddress() const = 0;

	/**
	* Gets the port of the address info.
	* @return The port.
	*/
	virtual int32 GetPort() const = 0;

	/**
	* Gets the socket type of the address info.
	* @return The socket type.
	*/
	virtual ESocketType GetSocketType() const = 0;

	/**
	* Gets the socket family of the address info.
	* @return The socket family.
	*/
	virtual ESocketFamily GetSocketFamily() const = 0;
};