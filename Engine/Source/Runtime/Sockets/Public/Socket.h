#pragma once

#include "SocketTypes.h"

/**
* Base class for socket implementations.
* Sockets can be used for communication between machines, or within a single machine.
*/
class SOCKETS_API FSocket
{
public:

	/**
	* Close the socket.
	*/
	virtual void Close() = 0;

	/**
	* Get the socket type.
	*/
};