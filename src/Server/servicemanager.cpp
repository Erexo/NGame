#include "pch.h"
#include "servicemanager.h"

bool ServiceManager::start()
{
	if (running)
		return false;

	N_DEBUG("Service manager is started");
	io_service.run();

	return true;
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