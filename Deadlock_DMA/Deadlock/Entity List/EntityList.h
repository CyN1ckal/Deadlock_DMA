#pragma once

#include "Deadlock/Classes/CBaseEntity.h"

inline constexpr size_t MAX_ENTITIES = 512;

class EntityList
{
public:
	static void UpdateCrucialInformation(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntitySystemAddress(DMA_Connection* Conn, Process* Proc);
	static void UpdateEntityListAddr(DMA_Connection* Conn, Process* Proc);

	static void UpdateEntityMap(DMA_Connection* Conn, Process* Proc);
	static void SortEntityMap();

	static void PrintPlayerControllers();

private:
	static inline std::vector<uintptr_t> m_PlayerControllers{};

	static inline uintptr_t m_EntitySystem_Address = 0;
	static inline uintptr_t m_EntityList_Address = 0;
};