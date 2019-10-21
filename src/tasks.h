/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_TASKS_H_A66AC384766041E59DCA059DAB6E1976
#define FS_TASKS_H_A66AC384766041E59DCA059DAB6E1976

#include <condition_variable>
#include "thread_holder_base.h"
#include "enums.h"

const int DISPATCHER_TASK_EXPIRATION = 2000;
const auto SYSTEM_TIME_ZERO = std::chrono::system_clock::time_point(std::chrono::milliseconds(0));

/* A task is a wrapper (that might have an expiration) around a function.
This is done so the dispatcher can efficiently manage computations in the server. TODO DOC*/
class Task
{
	public:
		/* This is a special class function called 'move constructor'.
		We know it because of the rvalue reference (&&) parameter.
		It is called everytime we create an object of this class with an rvalue.
		Because the only way to create a dispatcher task is the 'createTask' function
		(and it converts a lvalue to an rvalue with 'std::move()'),
		we end up always using this constructor. This prevents unnecessary copies.
		However, in the scope of the constructor, the argument is seen as an lvalue.
		In order to prevent copying, we also cast 'f' to an xvalue inside it.
		After all that, a task's 'func' variable "owns" that resource without unnecessary copies .*/
		explicit Task(std::function<void (void)>&& f) : func(std::move(f)) {}

		/* This is also a move constructor, but with two parameters. */
		Task(uint32_t ms, std::function<void (void)>&& f)
			/* This is the time of the task's creation + some miliseconds. */
			: expiration(std::chrono::system_clock::now() + std::chrono::milliseconds(ms))
			, func(std::move(f))
		{}

		/* The '~' (tilde) character tells the compiler this is this class' 'destructor'.
		When we assign a 'default' value to a class' special function, we're tell the compiler
		to automatically generate it. */
		virtual ~Task() = default;

		 /* This is another type of special class function, it is an 'operator overloading'.
		 In this case we are overloading the 'function call' operator.
		 By doing so, we enable an object of this class to execute an arbitrary function
		 inside its 'func' member variable like this: "someTask()". */
		void operator()() {
			func();
		}

		void setDontExpire() {
			expiration = SYSTEM_TIME_ZERO;
		}

		bool hasExpired() const {
			if (expiration == SYSTEM_TIME_ZERO) {
				return false;
			}
			return expiration < std::chrono::system_clock::now();
		}

	protected:
		/* The time the task expires. Defaults to not expiring. */
		std::chrono::system_clock::time_point expiration = SYSTEM_TIME_ZERO;

	private:
		/* This variable holds a function and its arguments, it's the actual "task"
		that the dispatcher will execute. As we don't want to waste memory and processing,
		we use move semantics to avoid copying it around.
		DEV NOTE: Possible to use perfect forwarding? */
		std::function<void (void)> func;
};

Task* createTask(std::function<void (void)> f);

Task* createTask(uint32_t expiration, std::function<void (void)> f);

/* The dispatcher's job is to concurrently execute the server's tasks
in its own thread. This makes it take advantage of multiple cores. TODO DOC */
class Dispatcher : public ThreadHolder<Dispatcher>
{
	public:
		void addTask(Task* task, bool push_front = false);

		void shutdown();

		uint64_t getDispatcherCycle() const {
			return dispatcherCycle;
		}

		void threadMain();

	private:
		std::thread thread;
		std::mutex taskLock;
		std::condition_variable taskSignal;

		/* Because our tasks are allocated in the heap at different times,
		a constant access time, memory location independent container is necessary.*/
		std::list<Task*> taskList;
		uint64_t dispatcherCycle = 0;
};

extern Dispatcher g_dispatcher;

#endif
