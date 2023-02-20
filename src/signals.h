// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SIGNALS_H
#define FS_SIGNALS_H

class Signals
{
	boost::asio::signal_set set;

public:
	explicit Signals(boost::asio::io_service& service);

private:
	void asyncWait();
};

#endif // FS_SIGNALS_H
