#pragma once
#include "DMA.h"

#include <chrono>

/*
	All credits for the input manager go to Metick and his DMALibrary. https://github.com/Metick/DMALibrary

	Minor tweaks and changes were made to fit the needs of this project.
*/

enum class e_registry_type
{
	none = REG_NONE,
	sz = REG_SZ,
	expand_sz = REG_EXPAND_SZ,
	binary = REG_BINARY,
	dword = REG_DWORD,
	dword_little_endian = REG_DWORD_LITTLE_ENDIAN,
	dword_big_endian = REG_DWORD_BIG_ENDIAN,
	link = REG_LINK,
	multi_sz = REG_MULTI_SZ,
	resource_list = REG_RESOURCE_LIST,
	full_resource_descriptor = REG_FULL_RESOURCE_DESCRIPTOR,
	resource_requirements_list = REG_RESOURCE_REQUIREMENTS_LIST,
	qword = REG_QWORD,
	qword_little_endian = REG_QWORD_LITTLE_ENDIAN
};

class c_registry
{
private:
public:
	c_registry()
	{
	}

	~c_registry()
	{
	}

	std::string QueryValue(DMA_Connection* Conn, const char* path, e_registry_type type);
};

class c_keys
{
private:
	static inline uint64_t gafAsyncKeyStateExport = 0;
	static inline uint8_t state_bitmap[64]{ };
	static inline uint8_t previous_state_bitmap[256 / 8]{ };
	static inline uint64_t win32kbase = 0;

	static inline int win_logon_pid = 0;

	static inline c_registry registry;
	static inline std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	static inline bool m_bInitialized = false;
public:
	static bool InitKeyboard(DMA_Connection* Conn);
	const static bool IsInitialized()
	{
		return m_bInitialized;
	}
	static void UpdateKeys(DMA_Connection* Conn);
	static bool IsKeyDown(DMA_Connection* Conn, uint32_t virtual_key_code);
};