//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_OTTHREAD_H__
#define __OTSERV_OTTHREAD_H__

#include "logger.h"

#include <list>
#include <vector>
#include <algorithm>

typedef std::vector< std::pair<uint32_t, uint32_t> > IPList;

#ifdef WIN32
#ifdef __WIN_LOW_FRAG_HEAP__
#define _WIN32_WINNT 0x0501
#endif
#include <stddef.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <WinSock2.h>

#define OTSYS_CREATE_THREAD(a, b) _beginthread(a, 0, b)

#define OTSYS_THREAD_LOCKVAR		CRITICAL_SECTION

#define OTSYS_THREAD_LOCKVARINIT(a)	InitializeCriticalSection(&a);
#define OTSYS_THREAD_LOCK(a, b)		EnterCriticalSection(&a);
#define OTSYS_THREAD_UNLOCK(a, b)	LeaveCriticalSection(&a);
#define OTSYS_THREAD_UNLOCK_PTR(a, b)	LeaveCriticalSection(a);

#define OTSYS_THREAD_TIMEOUT WAIT_TIMEOUT
#define OTSYS_THREAD_SIGNALVARINIT(a) a = CreateEvent(NULL, FALSE, FALSE, NULL)
#define OTSYS_THREAD_SIGNAL_SEND(a) SetEvent(a);

typedef HANDLE OTSYS_THREAD_SIGNALVAR;

inline int64_t OTSYS_TIME()
{
	_timeb t;
	_ftime(&t);
	return int64_t(t.millitm) + int64_t(t.time) * 1000;
}

inline int OTSYS_THREAD_WAITSIGNAL(OTSYS_THREAD_SIGNALVAR& signal, OTSYS_THREAD_LOCKVAR& lock)
{
	//LeaveCriticalSection(&lock);
	OTSYS_THREAD_UNLOCK(lock, "OTSYS_THREAD_WAITSIGNAL");
	WaitForSingleObject(signal, INFINITE);
	//EnterCriticalSection(&lock);
	OTSYS_THREAD_LOCK(lock, "OTSYS_THREAD_WAITSIGNAL");

	return -0x4711;
}

inline void OTSYS_SLEEP(uint32_t t)
{
	Sleep(t);
}

inline int OTSYS_THREAD_WAITSIGNAL_TIMED(OTSYS_THREAD_SIGNALVAR& signal, OTSYS_THREAD_LOCKVAR& lock, int64_t cycle)
{
	int64_t tout64 = (cycle - OTSYS_TIME());

	DWORD tout = 0;

	if (tout64 > 0) {
		tout = (DWORD)(tout64);
	}

	OTSYS_THREAD_UNLOCK(lock, "OTSYS_THREAD_WAITSIGNAL_TIMED");
	int ret = WaitForSingleObject(signal, tout);
	OTSYS_THREAD_LOCK(lock, "OTSYS_THREAD_WAITSIGNAL_TIMED");

	return ret;
}

#else

#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <errno.h>

#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE

inline void OTSYS_CREATE_THREAD(void * (*a)(void*), void* b)
{
	pthread_attr_t attr;
	pthread_t id;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&id, &attr, a, b);
}

typedef pthread_mutex_t OTSYS_THREAD_LOCKVAR;

inline void OTSYS_THREAD_LOCKVARINIT(OTSYS_THREAD_LOCKVAR& l)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&l, &attr);
}

#define OTSYS_THREAD_LOCK(a, b)          pthread_mutex_lock(&a);
#define OTSYS_THREAD_UNLOCK(a, b)        pthread_mutex_unlock(&a);
#define OTSYS_THREAD_UNLOCK_PTR(a, b)    pthread_mutex_unlock(a);
#define OTSYS_THREAD_TIMEOUT			  ETIMEDOUT
#define OTSYS_THREAD_SIGNALVARINIT(a) pthread_cond_init(&a, NULL);
#define OTSYS_THREAD_SIGNAL_SEND(a)   pthread_cond_signal(&a);

typedef pthread_cond_t OTSYS_THREAD_SIGNALVAR;

inline void OTSYS_SLEEP(int t)
{
	timespec tv;
	tv.tv_sec  = t / 1000;
	tv.tv_nsec = (t % 1000) * 1000000;
	nanosleep(&tv, NULL);
}

inline int64_t OTSYS_TIME()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

inline int OTSYS_THREAD_WAITSIGNAL(OTSYS_THREAD_SIGNALVAR& signal, OTSYS_THREAD_LOCKVAR& lock)
{
	return pthread_cond_wait(&signal, &lock);
}

inline int OTSYS_THREAD_WAITSIGNAL_TIMED(OTSYS_THREAD_SIGNALVAR& signal, OTSYS_THREAD_LOCKVAR& lock, int64_t cycle)
{
	timespec tv;
	tv.tv_sec = (int64_t)(cycle / 1000);
	// tv_nsec is in nanoseconds while we only store microseconds...
	tv.tv_nsec = (int64_t)(cycle % 1000) * 1000000;
	return pthread_cond_timedwait(&signal, &lock, &tv);
}

#endif

class OTSYS_THREAD_LOCK_CLASS
{
	public:
		inline OTSYS_THREAD_LOCK_CLASS(OTSYS_THREAD_LOCKVAR& a) {
			mutex = &a;
			OTSYS_THREAD_LOCK(a, NULL)
		}

		inline ~OTSYS_THREAD_LOCK_CLASS() {
			OTSYS_THREAD_UNLOCK_PTR(mutex, NULL)
		}

		OTSYS_THREAD_LOCKVAR* mutex;
};

#endif // #ifndef __OTSYSTEM_H__
