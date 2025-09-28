#pragma once
#include "Deadlock/Classes/CEntityListEntry.h"
#include "Deadlock/Classes/CBaseEntity.h"
#include "Deadlock/Classes/CCitadelPlayerController.h"
#include "Deadlock/Classes/CCitadelPlayerPawn.h"

inline constexpr size_t MAX_ENTITIES = 512;

class EntityList
{
public:
	static void UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntitySystemAddress(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityListAddr(DMA_Connection* Conn, Process* Proc);

	static void UpdateEntityMap(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerControllerAddresses();
	static void UpdatePlayerPawnAddresses();

	static void UpdatePlayerControllers(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerPawns(DMA_Connection* Conn, Process* Proc);

private:
	static inline uintptr_t m_EntitySystem_Address = 0;
	static inline uintptr_t m_EntityList_Address = 0;
	static inline std::array<CEntityListEntry, MAX_ENTITIES> m_CompleteEntityList{};

	static inline std::vector<uintptr_t> m_PlayerController_Addresses{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerController> m_PlayerControllers{};

	static inline std::vector<uintptr_t> m_PlayerPawn_Addresses{};

public:
	static inline std::mutex PlayerPawnsMutex{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerPawn> m_PlayerPawns{};

public: /* Debug features */
	static void PrintPlayerControllerAddresses();
	static void PrintPlayerControllers();
	static void PrintPlayerPawns();
};