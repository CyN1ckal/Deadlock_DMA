#include "pch.h"

#include "DMA.h"

DMA_Connection* DMA_Connection::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new DMA_Connection();

	return m_Instance;
}

VMM_HANDLE DMA_Connection::GetHandle()
{
	return m_VMMHandle;
}

bool DMA_Connection::EndConnection()
{
	this->~DMA_Connection();

	return true;
}

DMA_Connection::DMA_Connection()
{
    ZoneScoped;

    std::println("Connecting to DMA...");

    try {
        LPCSTR args[] = { "", "-device", "FPGA" };
        m_VMMHandle = VMMDLL_Initialize(3, args);

        if (!m_VMMHandle)
            throw std::runtime_error("VMMDLL_Initialize failed (Check FPGA connection/drivers)");

        std::println("Connected to DMA!");
    }
    catch (const std::exception& e) {
        std::println(stderr, "\n--- CRITICAL ERROR ---");
        std::println(stderr, "{}", e.what());
        std::println(stderr, "Press ENTER to exit...");

        // Wait for user input so the window stays open
        std::cin.get();

        // Re-throw so the program doesn't try to use a null handle
        throw;
    }
}

DMA_Connection::~DMA_Connection()
{
	ZoneScoped;

	VMMDLL_Close(m_VMMHandle);

	m_VMMHandle = nullptr;

	std::println("Disconnected from DMA!");
}