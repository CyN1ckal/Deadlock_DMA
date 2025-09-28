#pragma once

class Process
{
private:
	DWORD m_PID = 0;
	std::unordered_map<std::string, uintptr_t> m_Modules;

public:
	bool GetProcessInfo(const std::string& ProcessName, DMA_Connection* Conn);
	const uintptr_t GetBaseAddress() const;
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
			throw std::runtime_error("Failed to read memory");

		return Buffer;
	}
};