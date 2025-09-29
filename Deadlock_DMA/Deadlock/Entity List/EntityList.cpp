#include "pch.h"
#include "EntityList.h"

void EntityList::FullUpdate(DMA_Connection* Conn, Process* Proc)
{
	UpdateCrucialInformation(Conn, Proc);
	UpdateEntityMap(Conn, Proc);
	UpdatePlayerControllerAddresses();
	UpdatePlayerControllers(Conn, Proc);

	UpdatePlayerPawnAddresses();	
}

void EntityList::UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc)
{
	UpdateEntitySystemAddress(Conn, Proc);

	GetEntityListAddresses(Conn, Proc);
}

void EntityList::UpdateEntitySystemAddress(DMA_Connection* Conn, Process* Proc)
{
	uintptr_t EntitySystemPointer = Proc->GetModuleAddress("client.dll") + Offsets::GameEntitySystem;
	m_EntitySystem_Address = Proc->ReadMem<uintptr_t>(Conn, EntitySystemPointer);
	std::println("Entity System Address: 0x{:X}", m_EntitySystem_Address);
}

void EntityList::GetEntityListAddresses(DMA_Connection* Conn, Process* Proc)
{
	m_EntityList_Addresses.fill({});

	uintptr_t StartEntityListArray = m_EntitySystem_Address + 0x10;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < MAX_ENTITY_LISTS; i++)
	{
		auto& WriteAddr = m_EntityList_Addresses[i];
		auto Addr = StartEntityListArray + (i * sizeof(uintptr_t));
		VMMDLL_Scatter_PrepareEx(vmsh, Addr, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&WriteAddr), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdateEntityMap(DMA_Connection* Conn, Process* Proc)
{
	for (auto& Arr : m_CompleteEntityList)
		Arr.fill({});

	size_t EntityListSize = sizeof(CEntityListEntry) * MAX_ENTITIES;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	DWORD BytesRead[MAX_ENTITY_LISTS]{ 0 };

	for (int i = 0; i < MAX_ENTITY_LISTS; i++)
	{
		auto& Addr = m_EntityList_Addresses[i];
		auto& WriteAddr = m_CompleteEntityList[i][0];

		if (Addr == 0) continue;

		VMMDLL_Scatter_PrepareEx(vmsh, Addr, EntityListSize, reinterpret_cast<BYTE*>(&WriteAddr), &BytesRead[i]);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdatePlayerControllerAddresses()
{
	m_PlayerController_Addresses.clear();

	for (int i = 1; i < 32; i++)
	{
		auto& Entry = m_CompleteEntityList[0][i];

		if (Entry.NamePtr == 0)	continue;

		m_PlayerController_Addresses.push_back(Entry.pEntity);
	}
}

void EntityList::UpdatePlayerPawnAddresses()
{
	m_PlayerPawn_Addresses.clear();

	for (auto& [addr, pc] : m_PlayerControllers)
	{
		if (!pc.hPawn || pc.hPawn == 0x7FFF) continue;

		auto PawnEntityIndex = pc.GetPawnEntityIndex();
		auto ListIndex = PawnEntityIndex / MAX_ENTITIES;
		auto EntityIndex = PawnEntityIndex % MAX_ENTITIES;

		if (PawnEntityIndex >= MAX_ENTITIES  * MAX_ENTITY_LISTS) continue;

		auto& Entry = m_CompleteEntityList[ListIndex][EntityIndex];

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
