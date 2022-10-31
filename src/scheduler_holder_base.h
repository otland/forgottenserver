// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_SCHEDULER_HOLDER_BASE_H
#define FS_SCHEDULER_HOLDER_BASE_H

#include "thread_holder_base.h"

class SchedulerHolder : public ThreadHolder<SchedulerHolder>
{
public:
	SchedulerHolder() {}

	virtual void run() = 0;
	virtual void shutdown() = 0;

protected:
	std::mutex taskLock;
	std::condition_variable taskSignal;
};

#endif // FS_SCHEDULER_HOLDER_BASE_H
