/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019 Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_SIGNALHANDLINGTHREAD_H_01C6BF08B0EFE9E200175D108CF0B35F
#define FS_SIGNALHANDLINGTHREAD_H_01C6BF08B0EFE9E200175D108CF0B35F

#include <boost/asio.hpp>

class Signals
{
	boost::asio::signal_set set;
	public:
		explicit Signals(boost::asio::io_service& service);

	private:
		void asyncWait();
		static void dispatchSignalHandler(int signal);

		static void sigbreakHandler();
		static void sigintHandler();
		static void sighupHandler();
		static void sigtermHandler();
		static void sigusr1Handler();
};

#endif
