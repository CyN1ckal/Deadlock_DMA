#pragma once

namespace ConstStrings
{
	const std::string Game = "Deadlock.exe";
	const std::string Client = "client.dll";
}

class Process
{
private:
	DWORD m_PID = 0;
	std::unordered_map<std::string, uintptr_t> m_Modules;

public:
	bool GetProcessInfo(const std::string& ProcessName, DMA_Connection* Conn);
	const uintptr_t GetBaseAddress() const;
	const uintptr_t GetClientBase() const;
	const DWORD GetPID() const;
	const uintptr_t GetModuleAddress(const std::string& ModuleName);

private:
	bool PopulateModules(DMA_Connection* Conn);

public:
	template<typename T> inline T ReadMem(DMA_Connection* Conn, uintptr_t Address)
	{
		VMMDLL_SCATTER_HANDLE vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), m_PID, VMMDLL_FLAG_NOCACHE);
		DWORD BytesRead{ 0 };
		T Buffer{ 0 };

		VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), reinterpret_cast<BYTE*>(&Buffer), &BytesRead);

		VMMDLL_Scatter_Execute(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != sizeof(T))
			std::println("Incomplete read: {}/{}", BytesRead, sizeof(T));

		return Buffer;
	}
};