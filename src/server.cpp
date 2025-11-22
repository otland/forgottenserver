// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "server.h"

#include "configmanager.h"
#include "scheduler.h"
#include "tools.h"

namespace {

struct ConnectBlock
{
	uint64_t lastAttempt;
	uint64_t blockTime = 0;
	uint32_t count = 1;
};

bool acceptConnection(const Connection::Address& clientIP)
{
	static std::recursive_mutex mu;
	std::lock_guard lock{mu};

	uint64_t currentTime = OTSYS_TIME();

	static std::map<Connection::Address, ConnectBlock> ipConnectMap;
	auto it = ipConnectMap.find(clientIP);
	if (it == ipConnectMap.end()) {
		ipConnectMap.emplace(clientIP, ConnectBlock{.lastAttempt = currentTime});
		return true;
	}

	ConnectBlock& connectBlock = it->second;
	if (connectBlock.blockTime > currentTime) {
		connectBlock.blockTime += 250;
		return false;
	}

	int64_t timeDiff = currentTime - connectBlock.lastAttempt;
	connectBlock.lastAttempt = currentTime;
	if (timeDiff <= 5000) {
		if (++connectBlock.count > 5) {
			connectBlock.count = 0;
			if (timeDiff <= 500) {
				connectBlock.blockTime = currentTime + 3000;
				return false;
			}
		}
	} else {
		connectBlock.count = 1;
	}
	return true;
}

boost::asio::ip::address getListenAddress()
{
	if (getBoolean(ConfigManager::BIND_ONLY_GLOBAL_ADDRESS)) {
		return boost::asio::ip::make_address(getString(ConfigManager::IP));
	}
	return boost::asio::ip::address_v6::any();
}

void openAcceptor(std::weak_ptr<ServicePort> weak_service, uint16_t port)
{
	if (auto service = weak_service.lock()) {
		service->open(port);
	}
}

} // namespace

ServiceManager::~ServiceManager() { stop(); }

void ServiceManager::die() { io_context.stop(); }

void ServiceManager::run()
{
	assert(!running);
	running = true;
	io_context.run();
}

void ServiceManager::stop()
{
	if (!running) {
		return;
	}

	running = false;

	for (auto& servicePortIt : acceptors) {
		try {
			boost::asio::post(io_context, [servicePort = servicePortIt.second]() { servicePort->onStopServer(); });
		} catch (boost::system::system_error& e) {
			std::cout << "[ServiceManager::stop] Network Error: " << e.what() << std::endl;
		}
	}

	acceptors.clear();

	death_timer.expires_after(std::chrono::seconds(3));
	death_timer.async_wait([this](const boost::system::error_code&) { die(); });
}

ServicePort::~ServicePort() { close(); }

bool ServicePort::is_single_socket() const { return !services.empty() && services.front()->is_single_socket(); }

std::string ServicePort::get_protocol_names() const
{
	if (services.empty()) {
		return std::string();
	}

	std::string str = services.front()->get_protocol_name();
	for (size_t i = 1; i < services.size(); ++i) {
		str.push_back(',');
		str.push_back(' ');
		str.append(services[i]->get_protocol_name());
	}
	return str;
}

void ServicePort::accept()
{
	if (!acceptor) {
		return;
	}

	auto connection = ConnectionManager::getInstance().createConnection(io_context, shared_from_this());
	acceptor->async_accept(connection->getSocket(),
	                       [=, thisPtr = shared_from_this()](const boost::system::error_code& error) {
		                       thisPtr->onAccept(connection, error);
	                       });
}

void ServicePort::onAccept(Connection_ptr connection, const boost::system::error_code& error)
{
	if (!error) {
		if (services.empty()) {
			return;
		}

		const auto& remote_ip = connection->getIP();
		if (acceptConnection(remote_ip)) {
			Service_ptr service = services.front();
			if (service->is_single_socket()) {
				connection->accept(service->make_protocol(connection));
			} else {
				connection->accept();
			}
		} else {
			connection->close(Connection::FORCE_CLOSE);
		}

		accept();
	} else if (error != boost::asio::error::operation_aborted) {
		if (!pendingStart) {
			close();
			pendingStart = true;
			g_scheduler.addEvent(createSchedulerTask(
			    15000, [serverPort = this->serverPort, service = std::weak_ptr<ServicePort>(shared_from_this())]() {
				    openAcceptor(service, serverPort);
			    }));
		}
	}
}

Protocol_ptr ServicePort::make_protocol(NetworkMessage& msg, const Connection_ptr& connection) const
{
	uint8_t protocolID = msg.getByte();
	for (auto& service : services) {
		if (protocolID != service->get_protocol_identifier()) {
			continue;
		}
		return service->make_protocol(connection);
	}
	return nullptr;
}

void ServicePort::onStopServer() { close(); }

void ServicePort::open(uint16_t port)
{
	namespace ip = boost::asio::ip;

	close();

	serverPort = port;
	pendingStart = false;

	try {
		auto address = getListenAddress();

		acceptor = std::make_unique<ip::tcp::acceptor>(io_context, ip::tcp::endpoint{address, serverPort});
		if (address.is_v6()) {
			ip::v6_only option;
			acceptor->get_option(option);
			if (option) {
				boost::system::error_code err;
				acceptor->set_option(ip::v6_only{false}, err);
				if (err) {
					std::cout << "[Warning - ServicePort::open] Enabling IPv4 support failed: " << err.message()
					          << std::endl;
				}
			}
		}
		acceptor->set_option(ip::tcp::no_delay{true});

		accept();
	} catch (boost::system::system_error& e) {
		std::cout << "[ServicePort::open] Error: " << e.what() << std::endl;

		pendingStart = true;
		g_scheduler.addEvent(createSchedulerTask(
		    15000,
		    [port, service = std::weak_ptr<ServicePort>(shared_from_this())]() { openAcceptor(service, port); }));
	}
}

void ServicePort::close()
{
	if (acceptor && acceptor->is_open()) {
		boost::system::error_code error;
		acceptor->close(error);
	}
}

bool ServicePort::add_service(const Service_ptr& new_svc)
{
	if (std::any_of(services.begin(), services.end(), [](const Service_ptr& svc) { return svc->is_single_socket(); })) {
		return false;
	}

	services.push_back(new_svc);
	return true;
}
