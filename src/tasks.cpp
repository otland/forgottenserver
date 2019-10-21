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

#include "otpch.h"

#include "tasks.h"
#include "game.h"

extern Game g_game;

Task* createTask(std::function<void (void)> f)
{
	/* The 'std::move()' casts the 'f' parameter to an xvalue (which is an rvalue),
	triggering the move constructor. */
	return new Task(std::move(f));
}

Task* createTask(uint32_t expiration, std::function<void (void)> f)
{
	/* The 'std::move()' casts the 'f' parameter to an xvalue (which is an rvalue),
	triggering the move constructor. */
	return new Task(expiration, std::move(f));
}

/* TODO DOC*/
void Dispatcher::addTask(Task* task, bool push_front /*= false*/)
{
	/* As this function adds a task to the dispatcher's list,
	we assume it is not empty. */
	bool isListEmpty = false;

	/* Blocks access to 'taskList', as this piece of code will access and mutate it. */
	taskLock.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		isListEmpty = taskList.empty();

		if (push_front) {
			taskList.push_front(task);
		} else {
			taskList.push_back(task);
		}
	} else {
		/* If the state of the dispatcher thread is not set to running, delete the task. */
		delete task;
	}

	/* Unblocks access to 'taskList', as we finished accessing and mutating it. */
	taskLock.unlock();

	/* Because 'threadMain' waits for a signal when 'taskList' is empty,
	we have to send it one everytime we BOTH add a task AND the list is empty,
	otherwise it would never loop again and execute the added task. */
	if (isListEmpty) taskSignal.notify_one();
}

/* The end result of executing this function is:
1. immediately preventing other functions from accessing 'taskList' and
2. adding a task that prevents new tasks from being added to the end of 'taskList'.
This guarantees that all tasks added previous to 'shutdown()' will get executed
before the the dispatcher thread exits its loop. */
void Dispatcher::shutdown()
{
	/* Creates a task with a lambda function that
	1. sets the dispatcher's thread state to terminated and
	2. notifies the dispatcher that a new task has been added to 'taskList'. */
	Task* task = createTask([this]() {
		setState(THREAD_STATE_TERMINATED);
		taskSignal.notify_one();
		});

	/* As opposed to a 'std::unique_lock', a 'std::lock_guard' can't
	be locked and unlocked at will. From the moment you create it, it will
	take control of the mutex and will only release when it is destroyed
	(in this case, when this function finishes). */
	std::lock_guard<std::mutex> lockClass(taskLock);

	/* This will add the newly created task to the end of 'taskList'. */
	taskList.push_back(task); // DEV NOTE: Only line that directly manipulates 'taskList'.

	/* As when 'taskList' is empty we tell 'threadMain' to wait for a signal,
	we always have to send it a signal so it iterates over its loop again
	and eventually executes the tasks added to 'taskList'. */
	taskSignal.notify_one();
}

void Dispatcher::threadMain() {
	// Second argument 'defer_lock' prevents immediate locking TODO
	std::unique_lock<std::mutex> taskLockUnique(taskLock, std::defer_lock);

	while (getState() != THREAD_STATE_TERMINATED) {

		/* Because there are other parts of the code beyond this function
		that access and mutate 'taskList', we have to prevent data races.
		We do that by blocking access to it with the usage of a mutex
		and a lock that */
		taskLockUnique.lock();

		/* If there are no tasks to be executed at the moment,
		wait until 'addTask' notify that there's a task.*/
		if (taskList.empty()) {
			/* If the list is empty, the dispatcher waits until something
			signals they have mutated the 'taskList' to iterate over this loop again. */
			taskSignal.wait(taskLockUnique);
		}

		 /* Checks if there are pending tasks. */
		if (!taskList.empty()) {

			/* Because both are of type *Task, that is, are lvalues,
			the move constructor is not used, so the values are actually copied.
			It's very inexpensive, since pointers are actually just integers. */
			Task* task = taskList.front();

			/* Destroys the first element of 'taskList'.
			This is not a problem because the element (a pointer)
			was copied to the local 'task' pointer variable. */
			taskList.pop_front();

			/* Because we finished accessing 'taskList', we now
			unlock access to it by other parts enclosed by locking & unlocking functions. */
			taskLockUnique.unlock();

			 /* Checks if the task is still valid and then executes it. */
			if (!task->hasExpired()) {
				++dispatcherCycle;	// Updates dispatcherCycle
				(*task)();			// Dereferences 'task' pointer and execute its 'func' function

				g_game.map.clearSpectatorCache(); // TODO DOC
			}
			/* Because the task was created only once and never copied,
			this is the only delete necessary to free its memory. */
			delete task;
		} else {
			/* Eventually the dispatcher will execute all tasks and 'taskList' will be empty,
			which means its job is done (for a while), we then unblock acces to'taskList' so other functions
			can access and mutate it. */
			taskLockUnique.unlock();
		}
	}
}
