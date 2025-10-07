#pragma once

/*
  brief: Holds the information necessary for pattern scanning
*/
struct PatternInfo
{
public:
	std::string Pattern;
	std::string Mask;
	std::string ModuleName;

public:
	bool IsWildCard(int index)
	{
		if (index >= Mask.size())
			return false;

		if (Mask[index] == '?')
			return true;

		return false;
	}

};

/*
  brief: The multi scan class allows for a 1 time copy of the module
		 sections into a buffer. This allows for faster scanning of
		 multiple patterns in the same module.
*/
class MultiScan
{
public:

	// Initialize will find the first executable section in the given module and copy it into a local buffer
	bool Initialize(VMM_HANDLE vmh, DWORD PID, std::string ModuleName);

	// Scan will return the full runtime address of the pattern
	uintptr_t Scan(PatternInfo pi);

	// ScanOffset will return the offset of the pattern from the start of the module
	uintptr_t ScanOffset(PatternInfo pi);

	// ScanSectionOffset will return the offset of the pattern from the start of the section
	uintptr_t ScanSectionOffset(PatternInfo pi);

	// Close will free the buffer
	bool Close();

	// GetBuffer gets the buffer pointer
	BYTE* GetBuffer();

	// GetModuleBase
	uintptr_t GetModuleBase();

	// GetSectionBase
	uintptr_t GetSectionBase();

private:
	std::unique_ptr<BYTE[]> m_pBuffer;
	DWORD m_BufferSize;
	uintptr_t m_ModuleBase;
	uintptr_t m_SectionBase;
};