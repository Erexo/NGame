#pragma once

#include "service.h"

class ServiceManager
{
public:
	ServiceManager() = default;
	ServiceManager(const ServiceManager&) = delete;
	ServiceManager& operator=(const ServiceManager&) = delete;

	template<typename ProtocolT>
	bool add(uint16_t port);
	
	bool start();
	bool stop();

	inline bool isRunning() { return running; }

private:
	boost::asio::io_service io_service;
	std::unordered_map<uint16_t, Service_ptr> services;
	bool running = false;
};

template <typename ProtocolT>
bool ServiceManager::add(uint16_t port)
{
	if (port == 0)
		return false;

	if (services.find(port) != services.end())
		return false;

	Service_ptr service = std::make_shared<Service>(io_service, port, std::make_shared<ProtocolFactory<ProtocolT>>());
	N_DEBUG("New service created");
	service->open();
	services[port] = service;
	return true;
}
