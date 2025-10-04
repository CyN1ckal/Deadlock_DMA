#pragma once
#include "Deadlock/Classes/CEntityListEntry.h"
#include "Deadlock/Classes/Classes.h"

class EntityList
{
public: /* Interface methods */
	static void FullUpdate(DMA_Connection* Conn, Process* Proc);

	static void GetEntityListAddresses(DMA_Connection* Conn, Process* Proc);
	static void GetPlayerControllerAddresses();
	static void GetPlayerPawnAddresses();
	static void GetEntitySystemAddress(DMA_Connection* Conn, Process* Proc);

	static void UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityMap(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerControllers(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerPawns(DMA_Connection* Conn, Process* Proc);
	static void UpdateBosses(DMA_Connection* Conn, Process* Proc);
	static void UpdateTroopers(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityClassMap(DMA_Connection* Conn, Process* Proc);

	static void SortEntityList();

	static uintptr_t GetEntityAddressFromHandle(CHandle Handle);


public: /* Interface variables */
	static inline std::mutex m_PawnMutex{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerPawn> m_PlayerPawns{};

	static inline std::mutex m_ControllerMutex{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerController> m_PlayerControllers{};

	static inline std::mutex m_TrooperMutex{};
	static inline std::unordered_map<uintptr_t, CBaseEntity> m_Troopers{};

	static inline std::mutex m_MonsterCampMutex{};
	static inline std::unordered_map<uintptr_t, CBaseEntity> m_MonsterCamps{};

	static inline std::mutex m_ClassMapMutex{};
	static inline std::unordered_map<std::string, uintptr_t> m_EntityClassMap{};

private: /* Internal variables */
	static inline uintptr_t m_EntitySystem_Address = 0;
	static inline std::array<uintptr_t, MAX_ENTITY_LISTS> m_EntityList_Addresses{};
	static inline std::array<std::array<CEntityListEntry, MAX_ENTITIES>, MAX_ENTITY_LISTS> m_CompleteEntityList{};

	static inline std::vector<uintptr_t> m_PlayerController_Addresses{};
	static inline std::vector<uintptr_t> m_PlayerPawn_Addresses{};
	static inline std::vector<uintptr_t> m_TrooperAddresses{};
	static inline std::vector<uintptr_t> m_MonsterCampAddresses{};

public: /* Debug features */
	static void PrintPlayerControllerAddresses();
	static void PrintPlayerControllers();
	static void PrintPlayerPawns();
	static void PrintClassMap();
};