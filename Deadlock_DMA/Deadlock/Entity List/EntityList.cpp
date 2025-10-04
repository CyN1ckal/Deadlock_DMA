#include "pch.h"

#include "EntityList.h"

void EntityList::FullUpdate(DMA_Connection* Conn, Process* Proc)
{
	UpdateCrucialInformation(Conn, Proc);
	UpdateEntityMap(Conn, Proc);
	UpdateEntityClassMap(Conn, Proc);
	SortEntityList();
}

void EntityList::UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc)
{
	GetEntitySystemAddress(Conn, Proc);

	GetEntityListAddresses(Conn, Proc);
}

void EntityList::GetEntitySystemAddress(DMA_Connection* Conn, Process* Proc)
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

	for (int i = 0; i < MAX_ENTITY_LISTS; i++)
	{
		auto& Addr = m_EntityList_Addresses[i];
		auto& WriteAddr = m_CompleteEntityList[i][0];

		if (Addr == 0) continue;

		VMMDLL_Scatter_PrepareEx(vmsh, Addr, EntityListSize, reinterpret_cast<BYTE*>(&WriteAddr), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::SortEntityList()
{
	m_TrooperAddresses.clear();
	m_PlayerPawn_Addresses.clear();
	m_PlayerController_Addresses.clear();
	m_MonsterCampAddresses.clear();
	m_SinnersAddresses.clear();

	uintptr_t PlayerControllerClassPtr = m_EntityClassMap["citadel_player_controller"];
	uintptr_t PlayerPawnClassPtr = m_EntityClassMap["player"];

	uintptr_t TrooperClassPtr = 0;
	if (m_EntityClassMap.find("npc_trooper") != m_EntityClassMap.end())
		TrooperClassPtr = m_EntityClassMap["npc_trooper"];

	uintptr_t MonsterCampClassPtr = 0;
	if (m_EntityClassMap.find("npc_trooper_neutral") != m_EntityClassMap.end())
		MonsterCampClassPtr = m_EntityClassMap["npc_trooper_neutral"];

	uintptr_t SinnerClassPtr = 0;
	if (m_EntityClassMap.find("npc_neutral_sinners_sacrifce") != m_EntityClassMap.end())
		SinnerClassPtr = m_EntityClassMap["npc_neutral_sinners_sacrifce"];

	for (auto& List : m_CompleteEntityList)
	{
		for (auto& Entry : List)
		{
			if (TrooperClassPtr && Entry.NamePtr == TrooperClassPtr) m_TrooperAddresses.push_back(Entry.pEntity);
			else if (Entry.NamePtr == PlayerControllerClassPtr) m_PlayerController_Addresses.push_back(Entry.pEntity);
			else if (Entry.NamePtr == PlayerPawnClassPtr) m_PlayerPawn_Addresses.push_back(Entry.pEntity);
			else if (MonsterCampClassPtr && Entry.NamePtr == MonsterCampClassPtr) m_MonsterCampAddresses.push_back(Entry.pEntity);
			else if (SinnerClassPtr && Entry.NamePtr == SinnerClassPtr) m_SinnersAddresses.push_back(Entry.pEntity);
			else continue;
		}
	}
}

void EntityList::UpdatePlayerControllers(DMA_Connection* Conn, Process* Proc)
{
	std::scoped_lock Lock(m_ControllerMutex);

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
	std::scoped_lock Lock(m_PawnMutex);

	m_PlayerPawns.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerPawn_Addresses)
	{
		auto&& Pawn = m_PlayerPawns[pcAddr];
		CBaseEntity::Read_1(vmsh, Pawn, pcAddr);
		CCitadelPlayerPawn::Read_1(vmsh, Pawn, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerPawn_Addresses)
	{
		auto&& Pawn = m_PlayerPawns[pcAddr];
		CBaseEntity::Read_2(vmsh, Pawn, pcAddr);
		CCitadelPlayerPawn::Read_2(vmsh, Pawn, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& pcAddr : m_PlayerPawn_Addresses)
	{
		auto&& Pawn = m_PlayerPawns[pcAddr];
		CCitadelPlayerPawn::Read_3(vmsh, Pawn, pcAddr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdateBosses(DMA_Connection* Conn, Process* Proc)
{
	std::scoped_lock Lock(m_MonsterCampMutex);

	m_MonsterCamps.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_MonsterCampAddresses)
	{
		auto& Boss = m_MonsterCamps[Addr];
		CBaseEntity::Read_1(vmsh, Boss, Addr, true);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_MonsterCampAddresses)
	{
		auto& Boss = m_MonsterCamps[Addr];
		CBaseEntity::Read_2(vmsh, Boss, Addr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdateTroopers(DMA_Connection* Conn, Process* Proc)
{
	std::scoped_lock Lock(m_TrooperMutex);

	m_Troopers.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_TrooperAddresses)
	{
		auto& Trooper = m_Troopers[Addr];
		CBaseEntity::Read_1(vmsh, Trooper, Addr, true);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_TrooperAddresses)
	{
		auto& Trooper = m_Troopers[Addr];
		CBaseEntity::Read_2(vmsh, Trooper, Addr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::UpdateSinners(DMA_Connection* Conn, Process* Proc)
{
	std::scoped_lock Lock(m_SinnerMutex);
	m_Sinners.clear();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_SinnersAddresses)
	{
		auto& Sinner = m_Sinners[Addr];
		CBaseEntity::Read_1(vmsh, Sinner, Addr, true);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Addr : m_SinnersAddresses)
	{
		auto& Sinner = m_Sinners[Addr];
		CBaseEntity::Read_2(vmsh, Sinner, Addr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

uintptr_t EntityList::GetEntityAddressFromHandle(CHandle Handle)
{
	if (!Handle.IsValid()) return 0;

	auto ListIndex = Handle.GetEntityListIndex();
	auto EntityIndex = Handle.GetEntityEntryIndex();

	return m_CompleteEntityList[ListIndex][EntityIndex].pEntity;
}

void EntityList::UpdateEntityClassMap(DMA_Connection* Conn, Process* Proc)
{
	std::println("Updating Entity Class Types...");

	std::vector<uintptr_t> UniqueClassNames{};

	for (auto& List : m_CompleteEntityList)
	{
		for (auto& Entry : List)
		{
			if (Entry.pEntity == 0 || Entry.NamePtr == 0) continue;

			if (std::find(UniqueClassNames.begin(), UniqueClassNames.end(), Entry.NamePtr) != UniqueClassNames.end())
				continue;

			UniqueClassNames.push_back(Entry.NamePtr);
		}
	}

	struct NameBuffer
	{
		char Name[64]{ 0 };
	};

	auto Buffer = std::make_unique<NameBuffer[]>(UniqueClassNames.size());

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (int i = 0; i < UniqueClassNames.size(); i++)
	{
		auto& NamePtr = UniqueClassNames[i];
		VMMDLL_Scatter_PrepareEx(vmsh, NamePtr, sizeof(NameBuffer), reinterpret_cast<BYTE*>(&Buffer.get()[i]), nullptr);
	}

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::scoped_lock Lock(m_ClassMapMutex);

	for (int i = 0; i < UniqueClassNames.size(); i++)
	{
		auto& NamePtr = UniqueClassNames[i];

		std::string Name = Buffer.get()[i].Name;

		if (Name.empty()) continue;

		m_EntityClassMap[Name] = NamePtr;
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
		std::println("PlayerController: 0x{0:X} | hPawn: {1:X} {2:X}", addr, pc.m_hPawn.GetEntityEntryIndex(), pc.GameSceneNodeAddress);
}

void EntityList::PrintPlayerPawns()
{
	for (auto& [addr, pawn] : m_PlayerPawns)
		std::println("PlayerPawn: 0x{0:X} | GameSceneNode: {1:X}", addr, pawn.GameSceneNodeAddress);
}

void EntityList::PrintClassMap()
{
	for (auto& [name, addr] : m_EntityClassMap)
		std::println("Class: {} | Address: 0x{:X}", name, addr);
}
