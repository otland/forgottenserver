#include "../otpch.h"

#include "response_writer.h"
#include "peer.h"

namespace http
{

ResponseWriter::ResponseWriter(PeerSharedPtr peer, Request request, RequestID requestID):
	peer(std::move(peer)),
	requestID(requestID),
	request(std::move(request))
{
	//
}

void ResponseWriter::send()
{
	peer->send(std::move(response), requestID);
}

} //namespace http