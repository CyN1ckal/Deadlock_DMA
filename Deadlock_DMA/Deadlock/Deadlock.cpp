#include "pch.h"

#include "Deadlock.h"

bool Deadlock::Initialize(DMA_Connection* Conn)
{
	Proc().GetProcessInfo("deadlock.exe", Conn);

	return false;
}

Process& Deadlock::Proc()
{
	return m_DeadlockProc;
}