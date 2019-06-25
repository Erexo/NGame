#include <Ntity/pch.h>
#include "servicemanager.h"

bool ServiceManager::start()
{
	if (running)
		return false;

	N_DEBUG("Service manager is started");
	return running = true;
}

bool ServiceManager::stop()
{
	if (!running)
		return false;

	// todo: shutdown services

	N_DEBUG("Service manager stopped");
	services.clear();

	return true;
}

bool ServiceManager::poll()
{
	if (!running)
		return false;

	io_service.poll();
	return true;
}