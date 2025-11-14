#include "pch.h"
#include "DMA.h"
#include "Input Manager.h"
#include "Deadlock/Deadlock.h"

std::vector<int> GetPidListFromName(DMA_Connection* Conn, std::string name)
{
	PVMMDLL_PROCESS_INFORMATION process_info = NULL;
	DWORD total_processes = 0;
	std::vector<int> list = { };

	if (!VMMDLL_ProcessGetInformationAll(Conn->GetHandle(), &process_info, &total_processes))
	{
		std::println("[!] Failed to get process list");
		return list;
	}

	for (size_t i = 0; i < total_processes; i++)
	{
		auto process = process_info[i];
		if (strstr(process.szNameLong, name.c_str()))
			list.push_back(process.dwPID);
	}

	return list;
}

static const char* hexdigits =
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\001\002\003\004\005\006\007\010\011\000\000\000\000\000\000"
"\000\012\013\014\015\016\017\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\012\013\014\015\016\017\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
"\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000";

static uint8_t GetByte(const char* hex)
{
	return static_cast<uint8_t>((hexdigits[hex[0]] << 4) | (hexdigits[hex[1]]));
}

uint64_t FindSignature(DMA_Connection* Conn, const char* signature, uint64_t range_start, uint64_t range_end, int PID)
{
	if (!signature || signature[0] == '\0' || range_start >= range_end)
		return 0;

	if (PID == 0)
		PID = Deadlock::Proc().GetPID();

	std::vector<uint8_t> buffer(range_end - range_start);
	if (!VMMDLL_MemReadEx(Conn->GetHandle(), PID, range_start, buffer.data(), buffer.size(), 0, VMMDLL_FLAG_NOCACHE))
		return 0;

	const char* pat = signature;
	uint64_t first_match = 0;
	for (uint64_t i = range_start; i < range_end; i++)
	{
		if (*pat == '?' || buffer[i - range_start] == GetByte(pat))
		{
			if (!first_match)
				first_match = i;

			if (!pat[2])
				break;

			pat += (*pat == '?') ? 2 : 3;
		}
		else
		{
			pat = signature;
			first_match = 0;
		}
	}

	return first_match;
}

template <typename T> T ReadFromPID(DMA_Connection* Conn, uintptr_t Address, DWORD PID)
{
	T Value;

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), (BYTE*)&Value, &BytesRead);

	VMMDLL_Scatter_ExecuteRead(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (BytesRead != sizeof(T))
		return T();

	return Value;
};

//TODO: Restart winlogon.exe when it doesn't exist.
bool c_keys::InitKeyboard(DMA_Connection* Conn)
{
	std::string win = registry.QueryValue(Conn, "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;
	std::string ubr = registry.QueryValue(Conn, "HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\UBR", e_registry_type::dword);
	int Ubr = 0;
	if (!ubr.empty())
		Ubr = std::stoi(ubr);
	else
		return false;

	DWORD PID = 0x0;
	VMMDLL_PidGetFromName(Conn->GetHandle(), "winlogon.exe", &PID);
	win_logon_pid = PID;

	if (Winver > 22000)
	{
		auto pids = GetPidListFromName(Conn, "csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];

			PVMMDLL_MAP_MODULEENTRY win32k_module_info;
			if (!VMMDLL_Map_GetModuleFromNameW(Conn->GetHandle(), pid, const_cast<LPWSTR>(L"win32ksgd.sys"), &win32k_module_info, VMMDLL_MODULE_FLAG_NORMAL))
			{
				if (!VMMDLL_Map_GetModuleFromNameW(Conn->GetHandle(), pid, const_cast<LPWSTR>(L"win32k.sys"), &win32k_module_info, VMMDLL_MODULE_FLAG_NORMAL))
				{
					std::println("failed to get module win32k info");
					return false;
				}
			}
			uintptr_t win32k_base = win32k_module_info->vaBase;
			size_t win32k_size = win32k_module_info->cbImageSize;
			//win32ksgd
			auto g_session_ptr = FindSignature(Conn, "48 8B 05 ? ? ? ? 48 8B 04 C8", win32k_base, win32k_base + win32k_size, pid);
			if (!g_session_ptr)
			{
				//win32k
				g_session_ptr = FindSignature(Conn, "48 8B 05 ? ? ? ? FF C9", win32k_base, win32k_base + win32k_size, pid);
				if (!g_session_ptr)
				{
					std::println("failed to find g_session_global_slots");
					return false;
				}
			}
			uintptr_t ReadAddr = g_session_ptr + 3;
			int _relative = ReadFromPID<int>(Conn, ReadAddr, pid);

			int relative = _relative;
			uintptr_t g_session_global_slots = g_session_ptr + 7 + relative;
			uintptr_t user_session_state = 0;
			for (int i = 0; i < 4; i++)
			{
				uintptr_t Deref1 = ReadFromPID<uintptr_t>(Conn, g_session_global_slots, pid);

				uintptr_t Deref2 = ReadFromPID<uintptr_t>(Conn, Deref1 + 8 * i, pid);

				uintptr_t Deref3 = ReadFromPID<uintptr_t>(Conn, Deref2, pid);

				user_session_state = Deref3;

				if (user_session_state > 0x7FFFFFFFFFFF)
					break;
			}

			PVMMDLL_MAP_MODULEENTRY win32kbase_module_info;
			if (!VMMDLL_Map_GetModuleFromNameW(Conn->GetHandle(), pid, const_cast<LPWSTR>(L"win32kbase.sys"), &win32kbase_module_info, VMMDLL_MODULE_FLAG_NORMAL))
			{
				std::println("failed to get module win32kbase info");
				return false;
			}
			uintptr_t win32kbase_base = win32kbase_module_info->vaBase;
			size_t win32kbase_size = win32kbase_module_info->cbImageSize;

			//Unsure if this sig will work on all versions. (sig is from PostUpdateKeyStateEvent function. seems to exist in both older version and the new version of win32kbase that I have checked)
			uintptr_t ptr = FindSignature(Conn, "48 8D 90 ? ? ? ? E8 ? ? ? ? 0F 57 C0", win32kbase_base, win32kbase_base + win32kbase_size, pid);
			uint32_t session_offset = 0x0;
			if (ptr)
			{
				uintptr_t Deref1 = ReadFromPID<uintptr_t>(Conn, ptr + 3, pid);

				session_offset = Deref1;

				gafAsyncKeyStateExport = user_session_state + session_offset;
			}
			else
			{
				std::println("failed to find offset for gafAyncKeyStateExport");
				return false;
			}

			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF) break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
		{
			m_bInitialized = true;
			return true;
		}

		return false;
	}
	else
	{
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		DWORD PID = 0x0;
		VMMDLL_PidGetFromName(Conn->GetHandle(), "winlogon.exe", &PID);

		bool result = VMMDLL_Map_GetEATU(Conn->GetHandle(), PID | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, const_cast<LPSTR>("win32kbase.sys"), &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}

		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		{
			PVMMDLL_MAP_MODULEENTRY module_info;
			auto result = VMMDLL_Map_GetModuleFromNameW(Conn->GetHandle(), PID | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, static_cast<LPCWSTR>(L"win32kbase.sys"), &module_info, VMMDLL_MODULE_FLAG_NORMAL);
			if (!result)
			{
				std::println("failed to get module info");
				return false;
			}

			char str[261];
			if (!VMMDLL_PdbLoad(Conn->GetHandle(), PID | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, module_info->vaBase, str))
			{
				std::println("failed to load pdb");
				return false;
			}

			uintptr_t gafAsyncKeyState;
			if (!VMMDLL_PdbSymbolAddress(Conn->GetHandle(), str, const_cast<LPSTR>("gafAsyncKeyState"), &gafAsyncKeyState))
			{
				std::println("failed to find gafAsyncKeyState");
				return false;
			}
			std::println("found gafAsyncKeyState at: 0x%p\n", gafAsyncKeyState);
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
		{
			m_bInitialized = true;
			return true;
		}

		return false;
	}

}

void c_keys::UpdateKeys(DMA_Connection* Conn)
{
	uint8_t previous_key_state_bitmap[64] = { 0 };
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	VMMDLL_MemReadEx(Conn->GetHandle(), win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, reinterpret_cast<PBYTE>(&state_bitmap), 64, NULL, VMMDLL_FLAG_NOCACHE);
	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

bool c_keys::IsKeyDown(DMA_Connection* Conn, uint32_t virtual_key_code)
{
	if (c_keys::IsInitialized() == false)
		return false;

	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;

	if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(100))
	{
		UpdateKeys(Conn);
		start = std::chrono::system_clock::now();
	}

	return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}

std::string c_registry::QueryValue(DMA_Connection* Conn, const char* path, e_registry_type type)
{
	if (!Conn->GetHandle())
		return "";

	BYTE buffer[0x128];
	DWORD _type = static_cast<DWORD>(type);
	DWORD size = sizeof(buffer);

	if (!VMMDLL_WinReg_QueryValueExU(Conn->GetHandle(), const_cast<LPSTR>(path), &_type, buffer, &size))
	{
		std::println("[!] failed QueryValueExU call");
		return "";
	}
	//TODO: implement something nicer & better than this.
	if (type == e_registry_type::dword)
	{
		DWORD dwordValue = *reinterpret_cast<DWORD*>(buffer);
		return std::to_string(dwordValue);
	}
	std::wstring wstr = std::wstring(reinterpret_cast<wchar_t*>(buffer));
	return std::string(wstr.begin(), wstr.end());
}