#include "pch.h"

#include "Deadlock.h"

#include "Entity List/EntityList.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	auto Process = Deadlock::Proc();

	Process.GetProcessInfo("deadlock.exe", Conn);

	EntityList::UpdateCrucialInformation(Conn, &Process);

	EntityList::UpdateEntityMap(Conn, &Process);

	EntityList::UpdatePlayerControllerAddresses();

	EntityList::UpdatePlayerControllers(Conn, &Process);

	EntityList::UpdatePlayerPawnAddresses();

	EntityList::UpdatePlayerPawns(Conn, &Process);

	EntityList::PrintPlayerPawns();	

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}