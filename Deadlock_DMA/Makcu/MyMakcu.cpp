#include "pch.h"

#include "MyMakcu.h"

bool MyMakcu::Initialize()
{
	std::println("[Makcu] Initializing Makcu Device...");

	if (!m_Device.connect())
	{
		std::println("[Makcu] Failed to connect to Makcu");
		return false;
	}

	auto DeviceInfo = MyMakcu::m_Device.getDeviceInfo();
	std::println("[Makcu] Connected to Makcu Device on port: {}", DeviceInfo.port);

	return false;
}