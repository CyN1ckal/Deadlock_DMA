#pragma once

class Deadlock
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static Process& Proc();

private:
	static inline Process m_DeadlockProc{};
};