#ifndef FS_API_SERVER_RESPONSE_WRITER_H
#define FS_API_SERVER_RESPONSE_WRITER_H

#include "../tools.h"
#include "common.h"

namespace http
{

class Peer;
using PeerSharedPtr = std::shared_ptr<Peer>;

class ResponseWriter : NonCopyable
{
	const PeerSharedPtr peer;
	const RequestID requestID;
public:
	const Request request;
	Response response;
	ResponseWriter(PeerSharedPtr peer, Request request, RequestID requestID);

	void send();
};

} //namespace http

#endif // FS_API_SERVER_RESPONSE_WRITER_H