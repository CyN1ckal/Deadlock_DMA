#include "pch.h"

#include "DMA.h"

#include "Process.h"

bool Process::GetProcessInfo(const std::string& ProcessName, DMA_Connection* Conn)
{
	VMMDLL_PidGetFromName(Conn->GetHandle(), ProcessName.c_str(), &m_PID);

	if (!this->m_PID)
	{
		std::println("Failed to find process `{}`", ProcessName);
		return false;
	}

	std::println("Found process `{}` with PID {}", ProcessName, m_PID);

	PopulateModules(Conn);

	return true;
}

const uintptr_t Process::GetBaseAddress() const
{
	return m_Modules.at("deadlock.exe");
}

const DWORD Process::GetPID() const
{
	return m_PID;
}

const uintptr_t Process::GetModuleAddress(const std::string& ModuleName)
{
	return m_Modules.at(ModuleName);
}

bool Process::PopulateModules(DMA_Connection* Conn)
{
	m_Modules["deadlock.exe"] = VMMDLL_ProcessGetModuleBaseU(Conn->GetHandle(), this->m_PID, "deadlock.exe");

	m_Modules["client.dll"] = VMMDLL_ProcessGetModuleBaseU(Conn->GetHandle(), this->m_PID, "client.dll");

	for (auto& [Name, Address] : m_Modules)
		std::println("Module `{}` at address 0x{:X}", Name, Address);

	return false;
}
