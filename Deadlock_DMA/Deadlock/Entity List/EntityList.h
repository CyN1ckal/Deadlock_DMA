#pragma once
#include "Deadlock/Classes/CEntityListEntry.h"
#include "Deadlock/Classes/Classes.h"

class EntityList
{
public: /* Interface methods */
	static void FullUpdate(DMA_Connection* Conn, Process* Proc);

	static void GetEntityListAddresses(DMA_Connection* Conn, Process* Proc);
	static void GetEntitySystemAddress(DMA_Connection* Conn, Process* Proc);

	static void UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityMap(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityClassMap(DMA_Connection* Conn, Process* Proc);
	static void SortEntityList();
	static uintptr_t GetEntityAddressFromHandle(CHandle Handle);


	static void FullControllerRefresh_lk(DMA_Connection* Conn, Process* Proc);
	static void FullControllerRefresh(DMA_Connection* Conn, Process* Proc);
	static void QuickControllerRefresh(DMA_Connection* Conn, Process* Proc);

	static void FullSinnerRefresh(DMA_Connection* Conn, Process* Proc);

	static void FullPawnRefresh_lk(DMA_Connection* Conn, Process* Proc);
	static void FullPawnRefresh(DMA_Connection* Conn, Process* Proc);
	static void QuickPawnRefresh(DMA_Connection* Conn, Process* Proc);

	static void FullTrooperRefresh(DMA_Connection* Conn, Process* Proc);
	static void QuickTrooperRefresh(DMA_Connection* Conn, Process* Proc);

	static void FullMonsterCampRefresh(DMA_Connection* Conn, Process* Proc);
	static void QuickMonsterCampRefresh(DMA_Connection* Conn, Process* Proc);

public: /* Interface variables */
	static inline std::mutex m_PawnMutex{};
	static inline int32_t m_LocalPawnIndex = -1;
	static inline std::vector<CCitadelPlayerPawn> m_PlayerPawns{};

	static inline std::mutex m_ControllerMutex{};
	static inline int32_t m_LocalControllerIndex = -1;
	static inline std::vector<CCitadelPlayerController> m_PlayerControllers{};

	static inline std::mutex m_TrooperMutex{};
	static inline std::vector<CTrooperEntity> m_Troopers{};

	static inline std::mutex m_MonsterCampMutex{};
	static inline std::vector<CBaseEntity> m_MonsterCamps{};

	static inline std::mutex m_SinnerMutex{};
	static inline std::vector<CBaseEntity> m_Sinners{};

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
	static inline std::vector<uintptr_t> m_SinnersAddresses{};

public: /* Debug features */
	static void PrintPlayerControllerAddresses();
	static void PrintPlayerControllers();
	static void PrintPlayerPawns();
	static void PrintClassMap();

public:
	static ETeam GetLocalPlayerTeam();
	static Vector3 GetLocalPawnPosition();
	static 	CCitadelPlayerController* GetAssociatedPC(const CCitadelPlayerPawn& Pawn);
};