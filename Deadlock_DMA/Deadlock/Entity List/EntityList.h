#pragma once
#include "Deadlock/Classes/CEntityListEntry.h"
#include "Deadlock/Classes/Classes.h"

class EntityList
{
public:
	static void FullUpdate(DMA_Connection* Conn, Process* Proc);

	static void UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntitySystemAddress(DMA_Connection* Conn, Process* Proc);
	static void GetEntityListAddresses(DMA_Connection* Conn, Process* Proc);

	static void UpdateEntityMap(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerControllerAddresses();
	static void UpdatePlayerPawnAddresses();

	static void UpdatePlayerControllers(DMA_Connection* Conn, Process* Proc);
	static void UpdatePlayerPawns(DMA_Connection* Conn, Process* Proc);

	static uintptr_t GetEntityAddressFromHandle(CHandle Handle);

private:
	static inline uintptr_t m_EntitySystem_Address = 0;
	static inline std::array<uintptr_t, MAX_ENTITY_LISTS> m_EntityList_Addresses{};
	static inline std::array<std::array<CEntityListEntry, MAX_ENTITIES>, MAX_ENTITY_LISTS> m_CompleteEntityList{};

	static inline std::vector<uintptr_t> m_PlayerController_Addresses{};
	static inline std::vector<uintptr_t> m_PlayerPawn_Addresses{};

public:
	static inline std::mutex PlayerPawnsMutex{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerPawn> m_PlayerPawns{};

	static inline std::mutex PlayerControllerMutex{};
	static inline std::unordered_map<uintptr_t, CCitadelPlayerController> m_PlayerControllers{};

public: /* Debug features */
	static void PrintPlayerControllerAddresses();
	static void PrintPlayerControllers();
	static void PrintPlayerPawns();
};