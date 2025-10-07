#include "pch.h"

#include "MultiScan.h"

uintptr_t ScanBufferForPattern(PatternInfo pi, BYTE* Buffer, DWORD BufferSize)
{
	if (pi.Pattern.size() != pi.Mask.size())
	{
		std::println("[-] Pattern and Mask size do not match! {} {}", pi.Pattern.size(), pi.Mask.size());
		return 0;
	}

	DWORD MaxOffsetToScan = BufferSize - pi.Pattern.size() - 1;

	BYTE* CurrentAddress = Buffer;

	/* scan every single byte in the memory pool */
	bool bFound = false;
	for (int i = 0; i < MaxOffsetToScan; i++)
	{
		/* check if it matches pattern */
		for (int x = 0; x < pi.Pattern.size(); x++)
		{
			if (x == pi.Pattern.size() - 1)
			{
				if (pi.IsWildCard(x) || CurrentAddress[x] == (BYTE)pi.Pattern[x])
					bFound = true;

				break;
			}

			if (pi.IsWildCard(x))
				continue;

			if (CurrentAddress[x] == (BYTE)pi.Pattern[x])
				continue;

			/* if it is not a wild card and does not match; move on */
			break;
		}

		if (bFound)
			break;

		CurrentAddress++;
	}

	if (!bFound)
		return 0;

	uintptr_t Return = CurrentAddress - Buffer;

	return Return;
}

bool MultiScan::Initialize(VMM_HANDLE vmh, DWORD PID, std::string ModuleName)
{
	std::println("\n[+] Initializing MultiScan for {0:s}...", ModuleName.c_str());

	m_ModuleBase = VMMDLL_ProcessGetModuleBaseU(vmh, PID, ModuleName.c_str());

	if (!m_ModuleBase)
	{
		std::println("[-] Failed to get module base for {0:s}!", ModuleName.c_str());
		return 0;
	}

	DWORD cSections = 0;
	VMMDLL_ProcessGetSectionsU(vmh, PID, ModuleName.c_str(), NULL, 0, &cSections);

	if (!cSections)
	{
		std::println("[-] No sections in module {0:s}!", ModuleName.c_str());
		return 0;
	}

	auto SectionHeader = std::make_unique<IMAGE_SECTION_HEADER[]>(cSections);

	VMMDLL_ProcessGetSectionsU(vmh, PID, ModuleName.c_str(), SectionHeader.get(), cSections, &cSections);

	for (int i = 0; i < cSections; i++) {

		if (SectionHeader[i].Characteristics & IMAGE_SCN_MEM_EXECUTE)
		{
			std::println("[-] Found executable section! {0:s} @ {1:X} {2:X}.", reinterpret_cast<char*>(&SectionHeader[i].Name), m_ModuleBase + SectionHeader[i].VirtualAddress, SectionHeader[i].SizeOfRawData);

			auto SectionData = std::make_unique<BYTE[]>(SectionHeader[i].SizeOfRawData);

			DWORD TotalBytesRead = 0x0;
			DWORD BytesRead = 0;

			VMMDLL_MemReadEx(vmh, PID, m_ModuleBase + SectionHeader[i].VirtualAddress, SectionData.get(), SectionHeader[i].SizeOfRawData, &BytesRead, VMMDLL_FLAG_NOCACHE);

			TotalBytesRead += BytesRead;

			int MaxLoops = 100;
			int CurrentLoop = 0;

			while (TotalBytesRead < SectionHeader[i].SizeOfRawData)
			{
				VMMDLL_MemReadEx(vmh, PID, m_ModuleBase + SectionHeader[i].VirtualAddress + BytesRead, SectionData.get() + BytesRead, SectionHeader[i].SizeOfRawData - TotalBytesRead, &BytesRead, VMMDLL_FLAG_NOCACHE);
				TotalBytesRead += BytesRead;
				CurrentLoop++;

				if (CurrentLoop > MaxLoops)
				{
					std::println("[-] Couldn't read entire module! Continuing with {0:X} bytes out of {1:X} bytes", TotalBytesRead, SectionHeader[i].SizeOfRawData);
					break;
				}
			}

			m_pBuffer = std::move(SectionData);

			m_SectionBase = SectionHeader[i].VirtualAddress;

			m_BufferSize = TotalBytesRead;

			std::println("[-] Successfully copied data.");

			break;
		}
	}

	return 1;
}

/*
  brief: Return the full runtime address of the pattern.
*/
uintptr_t MultiScan::Scan(PatternInfo pi)
{
	if (!m_pBuffer)
	{
		std::println("[!] ERROR! Buffer is empty!");
		return 0;
	}

	uintptr_t Return = ScanBufferForPattern(pi, m_pBuffer.get(), m_BufferSize);

	if (!Return)
	{
		std::println("[!] ERROR! Could not find pattern!");
		return 0;
	}

	Return += m_SectionBase + m_ModuleBase;

	return Return;

};

/*
  brief: Return the offset of the pattern from the start of the module
*/
uintptr_t MultiScan::ScanOffset(PatternInfo pi)
{
	if (!m_pBuffer)
	{
		std::println("[!] ERROR! Buffer is empty!");
		return 0;
	}

	uintptr_t Return = ScanBufferForPattern(pi, m_pBuffer.get(), m_BufferSize);

	if (!Return)
	{
		std::println("[!] ERROR! Could not find pattern!");
		return 0;
	}

	Return += m_SectionBase;

	return Return;

};

/*
  brief: Return the offset of the pattern from the start of the module
*/
uintptr_t MultiScan::ScanSectionOffset(PatternInfo pi)
{
	if (!m_pBuffer)
	{
		std::println("[!] ERROR! Buffer is empty!");
		return 0;
	}

	uintptr_t Return = ScanBufferForPattern(pi, m_pBuffer.get(), m_BufferSize);

	if (!Return)
	{
		std::println("[!] ERROR! Could not find pattern!");
		return 0;
	}

	return Return;

};

bool MultiScan::Close()
{
	std::println("[-] Closing MultiScan...");

	if (m_pBuffer)
	{
		m_pBuffer.reset();
		m_BufferSize = 0;
	}

	std::println("[-] MultiScan closed successfully.\n");

	return 1;
}

BYTE* MultiScan::GetBuffer()
{
	return m_pBuffer.get();
};

uintptr_t MultiScan::GetModuleBase()
{
	return m_ModuleBase;
};

uintptr_t MultiScan::GetSectionBase()
{
	return m_SectionBase;
};