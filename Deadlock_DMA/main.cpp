#include "pch.h"

#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	Cppsched::Scheduler s(1);
	s.every("UpdateViewMatrix", std::chrono::milliseconds(3), Deadlock::UpdateViewMatrix, Conn);
	s.every("UpdatePlayerPawns", std::chrono::milliseconds(50), EntityList::UpdatePlayerPawns, Conn, &Deadlock::Proc());

	while (!GetAsyncKeyState(VK_END))
	{
		{
			std::scoped_lock PawnLock(EntityList::PlayerPawnsMutex);
			for (auto& [addr, pawn] : EntityList::m_PlayerPawns)
			{
				Vector2 ScreenPos;
				if (Deadlock::WorldToScreen(pawn.Position, ScreenPos))
					std::println("Pawn on screen! @ 0x{0:X} | {3} | Screen Position: X: {1}, Y: {2}", addr, ScreenPos.x, ScreenPos.y, pawn.Health);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}