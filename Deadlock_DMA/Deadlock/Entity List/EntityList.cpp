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

void EntityList::UpdateEntityMap(DMA_Connection* Conn, Process* Proc)
{
	m_CompleteEntityList.fill({});

	size_t TotalSize = sizeof(CEntityListEntry) * MAX_ENTITIES;
	DWORD BytesRead = 0;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityList_Address, TotalSize, reinterpret_cast<BYTE*>(m_CompleteEntityList.data()), &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	if (BytesRead != TotalSize)
		std::println("Incomplete entity list read: {}/{}", BytesRead, TotalSize);
}

void EntityList::UpdatePlayerControllerAddresses()
{
	m_PlayerController_Addresses.clear();

	for (int i = 1; i < 32; i++)
	{
		auto& Entry = m_CompleteEntityList[i];

		if (Entry.NamePtr == 0)	continue;

		if (i > 0 && i < 32)
		{
			m_PlayerController_Addresses.push_back(Entry.pEntity);
			continue;
		}
	}
}

void EntityList::UpdatePlayerPawnAddresses()
{
	m_PlayerPawn_Addresses.clear();

	for (auto& [addr, pc] : m_PlayerControllers)
	{
		if (!pc.hPawn)	continue;

		auto PawnEntityIndex = pc.GetPawnEntityIndex();

		if (PawnEntityIndex >= MAX_ENTITIES) continue;

		auto& Entry = m_CompleteEntityList[PawnEntityIndex];

		if (Entry.pEntity == 0)	continue;

		m_PlayerPawn_Addresses.push_back(Entry.pEntity);
	}
}

void EntityList::PrintPlayerControllerAddresses()
{
	for (auto& Addr : m_PlayerController_Addresses)
		std::println("PlayerController: 0x{:X}", Addr);
}

void EntityList::PrintPlayerControllers()
{
	for (auto& [addr, pc] : m_PlayerControllers)
		std::println("PlayerController: 0x{0:X} | hPawn: {1:X} {2:X}", addr, pc.GetPawnEntityIndex(), pc.GameSceneNodeAddress);
}

void EntityList::PrintPlayerPawns()
{
	for (auto& [addr, pawn] : m_PlayerPawns)
		std::println("PlayerPawn: 0x{0:X} | GameSceneNode: {1:X} | Health: {2}\n   Position {3:.0f} {4:.0f} {5:.0f}", addr, pawn.GameSceneNodeAddress, pawn.Health, pawn.Position.x, pawn.Position.y, pawn.Position.z);
}

void EntityList::UpdatePlayerControllers(DMA_Connection* Conn, Process* Proc)
{
	m_PlayerControllers.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerController_Addresses)
	{
		auto&& PC = m_PlayerControllers[pcAddr];
		CBaseEntity::Read_1(vmsh, PC, pcAddr);
		CCitadelPlayerController::Read_1(vmsh, PC, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdatePlayerPawns(DMA_Connection* Conn, Process* Proc)
{
	std::scoped_lock Lock(PlayerPawnsMutex);

	m_PlayerPawns.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerPawn_Addresses)
	{
		auto&& Pawn = m_PlayerPawns[pcAddr];
		CBaseEntity::Read_1(vmsh, Pawn, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerPawn_Addresses)
	{
		auto&& Pawn = m_PlayerPawns[pcAddr];
		CBaseEntity::Read_2(vmsh, Pawn, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}
