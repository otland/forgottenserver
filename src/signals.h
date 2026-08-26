// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SIGNALHANDLINGTHREAD_H_01C6BF08B0EFE9E200175D108CF0B35F
#define FS_SIGNALHANDLINGTHREAD_H_01C6BF08B0EFE9E200175D108CF0B35F

#include <asio.hpp>

class Signals
{
	asio::signal_set set;
	public:
		explicit Signals(asio::io_context& service);

	private:
		void asyncWait();
};

#endif
