#pragma once

#include "SocketTypes.h"

/**
* Base class for socket implementations.
* Sockets can be used for communication between machines, or within a single machine.
*/
class SOCKETS_API FSocket
{
public:

	FSocket(const FString& Description)
		: m_Description(Description) {}

	/**
	* Close the socket.
	*/
	virtual void Close() = 0;

	/**
	* Get the socket type.
	*/

protected:

	/** Description of the socket. */
	FString m_Description;
};