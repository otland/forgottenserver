// Copyright 2025 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "outputmessage.h"

#include "lockfree.h"
#include "protocol.h"
#include "scheduler.h"

extern Scheduler g_scheduler;

namespace {

const uint16_t OUTPUTMESSAGE_FREE_LIST_CAPACITY = 2048;
const std::chrono::milliseconds OUTPUTMESSAGE_AUTOSEND_DELAY{10};

// NOTE: A vector is used here because this container is mostly read and relatively rarely modified (only when a
// client connects/disconnects)
std::vector<Protocol_ptr> bufferedProtocols;

void sendAll(const std::vector<Protocol_ptr>& bufferedProtocols);

void scheduleSendAll(const std::vector<Protocol_ptr>& bufferedProtocols)
{
	g_scheduler.addEvent(
	    createSchedulerTask(OUTPUTMESSAGE_AUTOSEND_DELAY.count(), [&]() { sendAll(bufferedProtocols); }));
}

void sendAll(const std::vector<Protocol_ptr>& bufferedProtocols)
{
	// dispatcher thread
	for (auto& protocol : bufferedProtocols) {
		if (auto& msg = protocol->getCurrentBuffer()) {
			protocol->send(std::move(msg));
		}
	}

	if (!bufferedProtocols.empty()) {
		scheduleSendAll(bufferedProtocols);
	}
}

} // namespace

OutputMessage_ptr tfs::net::make_output_message()
{
	// LockfreePoolingAllocator<void,...> will leave (void* allocate) ill-formed because of sizeof(T), so this
	// guarantees that only one list will be initialized
	return std::allocate_shared<OutputMessage>(LockfreePoolingAllocator<void, OUTPUTMESSAGE_FREE_LIST_CAPACITY>());
}

void tfs::net::insert_protocol_to_autosend(const Protocol_ptr& protocol)
{
	// dispatcher thread
	if (bufferedProtocols.empty()) {
		scheduleSendAll(bufferedProtocols);
	}
	bufferedProtocols.emplace_back(protocol);
}

void tfs::net::remove_protocol_from_autosend(const Protocol_ptr& protocol)
{
	// dispatcher thread
	auto it = std::find(bufferedProtocols.begin(), bufferedProtocols.end(), protocol);
	if (it != bufferedProtocols.end()) {
		std::swap(*it, bufferedProtocols.back());
		bufferedProtocols.pop_back();
	}
}
