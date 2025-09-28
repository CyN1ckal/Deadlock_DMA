#include "pch.h"

#include "EntityList.h"

void EntityList::UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc)
{
	UpdateEntitySystemAddress(Conn, Proc);

	UpdateEntityListAddr(Conn, Proc);
}

void EntityList::UpdateEntitySystemAddress(DMA_Connection* Conn, Process* Proc)
{
	uintptr_t EntitySystemPointer = Proc->GetModuleAddress("client.dll") + Offsets::GameEntitySystem;
	m_EntitySystem_Address = Proc->ReadMem<uintptr_t>(Conn, EntitySystemPointer);
	std::println("Entity System Address: 0x{:X}", m_EntitySystem_Address);
}

void EntityList::UpdateEntityListAddr(DMA_Connection* Conn, Process* Proc)
{
	uintptr_t EntityListPointer = m_EntitySystem_Address + 0x10;
	m_EntityList_Address = Proc->ReadMem<uintptr_t>(Conn, EntityListPointer);
	std::println("Entity List Address: 0x{:X}", m_EntityList_Address);
}

class CEntityListEntry
{
public:
	uintptr_t pEntity; //0x0000
	char pad_0008[24]; //0x0008
	uintptr_t NamePtr; //0x0020
	char pad_0028[80]; //0x0028
}; //Size: 0x0078
static_assert(sizeof(CEntityListEntry) == 0x78);

std::array<CEntityListEntry, MAX_ENTITIES> EntArray;
void EntityList::UpdateEntityMap(DMA_Connection* Conn, Process* Proc)
{
	EntArray.fill({});

	size_t TotalSize = sizeof(CEntityListEntry) * MAX_ENTITIES;
	DWORD BytesRead = 0;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityList_Address, TotalSize, reinterpret_cast<BYTE*>(EntArray.data()), &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (BytesRead != TotalSize)
		throw std::runtime_error("Failed to read memory");

	SortEntityMap();
}

void EntityList::SortEntityMap()
{
	m_PlayerControllers.clear();

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		auto& Entry = EntArray[i];

		if (Entry.NamePtr == 0)
			continue;

		if (i > 0 && i < 32)
		{
			m_PlayerControllers.push_back(Entry.pEntity);
			continue;
		}
	}
}

void EntityList::PrintPlayerControllers()
{
	for (auto& pc : m_PlayerControllers)
		std::println("PlayerController: 0x{:X}", pc);
}
