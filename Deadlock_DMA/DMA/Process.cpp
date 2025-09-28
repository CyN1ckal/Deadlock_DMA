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

	std::println("Client Base: 0x{:X}", GetClientBase());

	return true;
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

bool Process::PopulateModules(DMA_Connection* Conn)
{
	using namespace ConstStrings;

	m_Modules[Game] = VMMDLL_ProcessGetModuleBaseU(Conn->GetHandle(), this->m_PID, Game.c_str());

	m_Modules[Client] = VMMDLL_ProcessGetModuleBaseU(Conn->GetHandle(), this->m_PID, Client.c_str());

	for (auto& [Name, Address] : m_Modules)
		std::println("Module `{}` at address 0x{:X}", Name, Address);

	return false;
}
