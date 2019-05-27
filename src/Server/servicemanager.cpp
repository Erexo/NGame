#include "pch.h"
#include "servicemanager.h"
#include "tools.h"

bool ServiceManager::start()
{
	if (running)
		return false;

	LOG_TRACE("Service manager is running")
	io_service.run();

	return true;
}

bool ServiceManager::stop()
{
	if (!running)
		return false;

	// todo: shutdown services

	LOG_TRACE("Service manager stopped")
	services.clear();

	return true;
}