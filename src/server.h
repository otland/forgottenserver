/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_SERVER_H_984DA68ABF744127850F90CC710F281B
#define FS_SERVER_H_984DA68ABF744127850F90CC710F281B

class Connection;
typedef std::shared_ptr<Connection> Connection_ptr;
class Protocol;
class NetworkMessage;

class ServiceBase;

typedef std::shared_ptr<ServiceBase> Service_ptr;

class ServiceBase
{
	public:
		virtual ~ServiceBase() {} // Redundant, but stifles compiler warnings

		virtual bool is_single_socket() const = 0;
		virtual bool is_checksummed() const = 0;
		virtual uint8_t get_protocol_identifier() const = 0;
		virtual const char* get_protocol_name() const = 0;

		virtual Protocol* make_protocol(Connection_ptr c) const = 0;
};

template <typename ProtocolType>
class Service : public ServiceBase
{
	public:
		bool is_single_socket() const {
			return ProtocolType::server_sends_first;
		}
		bool is_checksummed() const {
			return ProtocolType::use_checksum;
		}
		uint8_t get_protocol_identifier() const {
			return ProtocolType::protocol_identifier;
		}
		const char* get_protocol_name() const {
			return ProtocolType::protocol_name();
		}

		Protocol* make_protocol(Connection_ptr c) const {
			return new ProtocolType(c);
		}
};

class ServicePort : public std::enable_shared_from_this<ServicePort>
{
	public:
		ServicePort(boost::asio::io_service& io_service);
		~ServicePort();

		// non-copyable
		ServicePort(const ServicePort&) = delete;
		ServicePort& operator=(const ServicePort&) = delete;

		static void openAcceptor(std::weak_ptr<ServicePort> weak_service, uint16_t port);
		void open(uint16_t port);
		void close();
		bool is_single_socket() const;
		std::string get_protocol_names() const;

		bool add_service(Service_ptr);
		Protocol* make_protocol(bool checksummed, NetworkMessage& msg) const;

		void onStopServer();
		void onAccept(boost::asio::ip::tcp::socket* socket, const boost::system::error_code& error);

	protected:
		void accept();

		boost::asio::io_service& m_io_service;
		boost::asio::ip::tcp::acceptor* m_acceptor;
		std::vector<Service_ptr> m_services;

		uint16_t m_serverPort;
		bool m_pendingStart;
};

typedef std::shared_ptr<ServicePort> ServicePort_ptr;

class ServiceManager
{
	public:
		ServiceManager();
		~ServiceManager();

		// non-copyable
		ServiceManager(const ServiceManager&) = delete;
		ServiceManager& operator=(const ServiceManager&) = delete;

		void run();
		void stop();

		bool okay();

		template <typename ProtocolType>
		bool add(uint16_t port);

		bool is_running() const {
			return m_acceptors.empty() == false;
		}

	protected:
		void die();

		std::map<uint16_t, ServicePort_ptr> m_acceptors;

		boost::asio::io_service m_io_service;
		boost::asio::deadline_timer death_timer;
		bool running;
};

template <typename ProtocolType>
bool ServiceManager::add(uint16_t port)
{
	if (port == 0) {
		std::cout << "ERROR: No port provided for service " << ProtocolType::protocol_name() << ". Service disabled." << std::endl;
		return false;
	}

	ServicePort_ptr service_port;

	std::map<uint16_t, ServicePort_ptr>::iterator finder =
	    m_acceptors.find(port);

	if (finder == m_acceptors.end()) {
		service_port.reset(new ServicePort(m_io_service));
		service_port->open(port);
		m_acceptors[port] = service_port;
	} else {
		service_port = finder->second;

		if (service_port->is_single_socket() || ProtocolType::server_sends_first) {
			std::cout << "ERROR: " << ProtocolType::protocol_name() <<
			          " and " << service_port->get_protocol_names() <<
			          " cannot use the same port " << port << '.' << std::endl;
			return false;
		}
	}

	return service_port->add_service(Service_ptr(new Service<ProtocolType>()));
}

#endif
