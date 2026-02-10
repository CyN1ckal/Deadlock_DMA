#include "pch.h"

#include "EntityList.h"
#include "GUI/Fuser/ESP/ESP.h"

#include "GUI/Query.h"

void EntityList::FullUpdate(DMA_Connection* Conn, Process* Proc)
{
	ZoneScoped;

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
	uintptr_t LatestAddr = Proc->ReadMem<uintptr_t>(Conn, EntitySystemPointer);

	if (LatestAddr == m_EntitySystem_Address) return;

	m_EntitySystem_Address = LatestAddr;

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
	std::scoped_lock Lock(m_PawnMutex, m_ControllerMutex);

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

	DbgPrintln("Entity Map Updated.");
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
	if (m_EntityClassMap.find("npc_neutral_sinners_sacrifice") != m_EntityClassMap.end())
		SinnerClassPtr = m_EntityClassMap["npc_neutral_sinners_sacrifice"];

	for (auto& List : m_CompleteEntityList)
	{
		for (auto& Entry : List)
		{
			if (TrooperClassPtr && Entry.pName == TrooperClassPtr) m_TrooperAddresses.push_back(Entry.pEnt);
			else if (Entry.pName == PlayerControllerClassPtr) m_PlayerController_Addresses.push_back(Entry.pEnt);
			else if (Entry.pName == PlayerPawnClassPtr) m_PlayerPawn_Addresses.push_back(Entry.pEnt);
			else if (MonsterCampClassPtr && Entry.pName == MonsterCampClassPtr) m_MonsterCampAddresses.push_back(Entry.pEnt);
			else if (SinnerClassPtr && Entry.pName == SinnerClassPtr) m_SinnersAddresses.push_back(Entry.pEnt);
			else continue;
		}
	}
}

void EntityList::FullControllerRefresh_lk(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingPlayers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_ControllerMutex);

	FullControllerRefresh(Conn, Proc);
}

void EntityList::FullControllerRefresh(DMA_Connection* Conn, Process* Proc)
{
	m_PlayerControllers.clear();

	for (auto& addr : m_PlayerController_Addresses)
		m_PlayerControllers.emplace_back(CCitadelPlayerController(addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& PC : m_PlayerControllers)
		PC.PrepareRead_1(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& PC : m_PlayerControllers)
		PC.PrepareRead_2(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (int i = 0; i < m_PlayerControllers.size(); i++)
	{
		if (m_PlayerControllers[i].IsLocalPlayer())
		{
			m_LocalControllerIndex = i;

			break;
		}
		else if (i == m_PlayerControllers.size() - 1)
		{
			m_LocalControllerIndex = -1;
		}
	}
}

void EntityList::QuickControllerRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingPlayers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_ControllerMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& PC : m_PlayerControllers)
		PC.QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::FullPawnRefresh_lk(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingPlayers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_PawnMutex);

	FullPawnRefresh(Conn, Proc);
}

void EntityList::FullPawnRefresh(DMA_Connection* Conn, Process* Proc)
{
	ZoneScoped;

	m_PlayerPawns.clear();

	for (auto& addr : m_PlayerPawn_Addresses)
		m_PlayerPawns.emplace_back(CCitadelPlayerPawn(addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Pawn : m_PlayerPawns)
		Pawn.PrepareRead_1(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Pawn : m_PlayerPawns)
		Pawn.PrepareRead_2(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Pawn : m_PlayerPawns)
		Pawn.PrepareRead_3(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	for (int i = 0; i < m_PlayerPawns.size(); i++)
	{
		if (m_PlayerPawns[i].IsLocalPlayer())
		{
			m_LocalPawnIndex = i;
			break;
		}
		else if (i == m_PlayerPawns.size() - 1)
		{
			m_LocalPawnIndex = -1;
		}
	}
}

void EntityList::QuickPawnRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingPlayers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_PawnMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Pawn : m_PlayerPawns)
		Pawn.QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);
}

void EntityList::FullMonsterCampRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingCamps() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_MonsterCampMutex);

	m_MonsterCamps.clear();

	for (auto& addr : m_MonsterCampAddresses)
		m_MonsterCamps.emplace_back(CBaseEntity(addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Camp : m_MonsterCamps)
		Camp.PrepareRead_1(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Camp : m_MonsterCamps)
		Camp.PrepareRead_2(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::QuickMonsterCampRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingCamps() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_MonsterCampMutex);

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Camp : m_MonsterCamps)
		Camp.QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);
}

void EntityList::FullTrooperRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingTroopers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_TrooperMutex);

	m_Troopers.clear();

	for (auto& addr : m_TrooperAddresses)
		m_Troopers.emplace_back(CTrooperEntity(addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Trooper : m_Troopers)
		Trooper.PrepareRead_1(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Trooper : m_Troopers)
		Trooper.PrepareRead_2(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	DbgPrintln("Trooper List Refreshed. Count: {}", m_Troopers.size());
}

void EntityList::QuickTrooperRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingTroopers() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_TrooperMutex);

	if (m_Troopers.empty()) return;

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Trooper : m_Troopers)
		Trooper.QuickRead(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

void EntityList::FullSinnerRefresh(DMA_Connection* Conn, Process* Proc)
{
	if (Query::IsUsingSinners() == false) return;

	ZoneScoped;

	std::scoped_lock Lock(m_SinnerMutex);

	m_Sinners.clear();

	for (auto& addr : m_SinnersAddresses)
		m_Sinners.emplace_back(CBaseEntity(addr));

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Sinner : m_Sinners)
		Sinner.PrepareRead_1(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, Proc->GetPID(), VMMDLL_FLAG_NOCACHE);

	for (auto& Sinner : m_Sinners)
		Sinner.PrepareRead_2(vmsh);

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);
}

uintptr_t EntityList::GetEntityAddressFromHandle(CHandle Handle)
{
	if (!Handle.IsValid()) return 0;

	auto ListIndex = Handle.GetEntityListIndex();
	auto EntityIndex = Handle.GetEntityEntryIndex();

	return m_CompleteEntityList[ListIndex][EntityIndex].pEnt;
}

void EntityList::UpdateEntityClassMap(DMA_Connection* Conn, Process* Proc)
{
	std::vector<uintptr_t> UniqueClassNames{};

	for (auto& List : m_CompleteEntityList)
	{
		for (auto& Entry : List)
		{
			if (Entry.pEnt == 0 || Entry.pName == 0) continue;

			if (std::find(UniqueClassNames.begin(), UniqueClassNames.end(), Entry.pName) != UniqueClassNames.end())
				continue;

			UniqueClassNames.push_back(Entry.pName);
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

	DbgPrintln("Entity Class Map Updated.");
}

void EntityList::PrintPlayerControllerAddresses()
{
	for (auto& Addr : m_PlayerController_Addresses)
		std::println("PlayerController: 0x{:X}", Addr);
}

void EntityList::PrintPlayerControllers()
{
	if (m_PlayerControllers.empty())
	{
		std::println("No PlayerControllers found.");
		return;
	}


	for (auto& PC : m_PlayerControllers)
	{
		if (PC.IsInvalid()) continue;

		auto PawnIt = std::find(m_PlayerPawns.begin(), m_PlayerPawns.end(), EntityList::GetEntityAddressFromHandle(PC.m_hPawn));

		if (PawnIt == std::end(m_PlayerPawns)) continue;

		if (PawnIt->IsInvalid()) continue;

		std::println("Player found! hPawn: {0:d}, hController {1:d}\n  Pawn @ {2:.0f} with {3:d} hp", PC.m_hPawn.GetEntityEntryIndex(), PawnIt->m_hController.GetEntityEntryIndex(), PawnIt->m_BonePositions[0].z, PC.m_CurrentHealth);
	}
}

void EntityList::PrintPlayerPawns()
{
	for (auto& Pawn : m_PlayerPawns)
		std::println("PlayerPawn: 0x{0:X} | GameSceneNode: {1:X}", Pawn.m_EntityAddress, Pawn.m_GameSceneNodeAddress);
}

void EntityList::PrintClassMap()
{
	for (auto& [name, addr] : m_EntityClassMap)
		std::println("Class: {} | Address: 0x{:X}", name, addr);
}

ETeam EntityList::GetLocalPlayerTeam()
{
	auto Return = ETeam::UNKNOWN;

	std::scoped_lock Lock(m_PawnMutex);

	if (m_LocalPawnIndex == -1)
		return Return;

	return m_PlayerPawns[m_LocalPawnIndex].m_TeamNum;
}

Vector3 EntityList::GetLocalPawnPosition()
{
	auto Return = Vector3();

	std::scoped_lock Lock(m_PawnMutex);

	if (m_LocalPawnIndex == -1)
		return Return;

	return m_PlayerPawns[m_LocalPawnIndex].m_Position;
}

/* m_ControllerMutex must be locked! */
CCitadelPlayerController* EntityList::GetAssociatedPC(const CCitadelPlayerPawn& Pawn)
{
	auto PCAddress = EntityList::GetEntityAddressFromHandle(Pawn.m_hController);

	for (auto& PC : m_PlayerControllers)
	{
		if (PC.m_EntityAddress == PCAddress)
			return &PC;
	}

	return nullptr;
}