#include "pch.h"

#include "DMA.h"

#include "Process.h"

bool Process::GetProcessInfo(const std::string& ProcessName, DMA_Connection* Conn)
{
    while (true)
    {
        m_PID = 0;
        VMMDLL_PidGetFromName(Conn->GetHandle(), ProcessName.c_str(), &m_PID);

        if (m_PID)
        {
            std::println("Found process `{}` with PID {}", ProcessName, m_PID);
            PopulateModules(Conn);
            std::println("Client Base: 0x{:X}", GetClientBase());
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

const uintptr_t Process::GetBaseAddress() const
{
	using namespace ConstStrings;
	return m_Modules.at(Game);
}

const uintptr_t Process::GetClientBase() const
{
	using namespace ConstStrings;
	return m_Modules.at(Client);
}

const DWORD Process::GetPID() const
{
	return m_PID;
}

const uintptr_t Process::GetModuleAddress(const std::string& ModuleName)
{
	return m_Modules.at(ModuleName);
}

void Process::PopulateModules(DMA_Connection* Conn)
{
	using namespace ConstStrings;

    VMM_HANDLE handle = Conn->GetHandle();
    if (!handle) {
        // TODO: add error logging?
        return;
    }

    while (true) {

        m_Modules[Game] = VMMDLL_ProcessGetModuleBaseU(handle, this->m_PID, Game.c_str());
        m_Modules[Client] = VMMDLL_ProcessGetModuleBaseU(handle, this->m_PID, Client.c_str());

        // break out of the loop when the module base addresses have been read
        if (m_Modules[Game] && m_Modules[Client])
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

	for (auto& [Name, Address] : m_Modules)
		std::println("Module `{}` at address 0x{:X}", Name, Address);
}
