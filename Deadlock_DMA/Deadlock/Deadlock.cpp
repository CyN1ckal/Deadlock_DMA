#include "pch.h"

#include "Deadlock.h"

#include "Entity List/EntityList.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	Proc().GetProcessInfo("deadlock.exe", Conn);

	EntityList::UpdateCrucialInformation(Conn, &Proc());

	EntityList::UpdateEntityMap(Conn, &Proc());

	EntityList::PrintPlayerControllers();

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}