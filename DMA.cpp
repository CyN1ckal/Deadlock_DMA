#include "pch.h"

#include "DMA.h"
#include <mutex>

DMA_Connection* DMA_Connection::GetInstance()
{
	std::lock_guard<std::mutex> lock(m_InstanceMutex);
	if (m_Instance == nullptr)
		m_Instance = new DMA_Connection();

	return m_Instance;
}

VMM_HANDLE DMA_Connection::GetHandle() const
{
	return m_VMMHandle;
}

bool DMA_Connection::EndConnection()
{
	this->~DMA_Connection();

	return true;
}

bool DMA_Connection::RestartConnection()
{
	std::lock_guard<std::mutex> lock(m_InstanceMutex);
	
	
	if (m_Instance != nullptr)
	{
		m_Instance->~DMA_Connection();
		delete m_Instance;
		m_Instance = nullptr;
	}
	
	
	try
	{
		m_Instance = new DMA_Connection();
		return m_Instance != nullptr && m_Instance->m_VMMHandle != nullptr;
	}
	catch (...)
	{
		m_Instance = nullptr;
		return false;
	}
}

DMA_Connection::DMA_Connection()
{
    ZoneScoped;

    std::println("Connecting to DMA...");

    try {
        LPCSTR args[] = {
            "-device",
            "fpga",
            "-waitinitialize"
        };

        m_VMMHandle = VMMDLL_Initialize(3, args);

        if (!m_VMMHandle)
            throw std::runtime_error("VMMDLL_Initialize failed (Check FPGA connection/drivers)");

        std::println("Connected to DMA!");
    }
    catch (const std::exception& e) {
        std::println(stderr, "\n--- CRITICAL ERROR ---");
        std::println(stderr, "{}", e.what());
        std::println(stderr, "Press ENTER to exit...");

        
        std::cin.get();

        
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
