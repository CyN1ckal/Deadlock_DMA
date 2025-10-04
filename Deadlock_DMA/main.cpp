#include "pch.h"

#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"
#include "GUI/Main Window/Main Window.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	MainWindow::Initialize();

	Cppsched::Scheduler s(1);
	s.every("UpdateViewMatrix", std::chrono::milliseconds(1), Deadlock::UpdateViewMatrix, Conn);
	s.every("UpdatePlayerPawns", std::chrono::milliseconds(10), EntityList::UpdatePlayerPawns, Conn, &Deadlock::Proc());
	s.every("UpdateTroopers", std::chrono::milliseconds(100), EntityList::UpdateTroopers, Conn, &Deadlock::Proc());
	s.every("UpdateBosses", std::chrono::milliseconds(100), EntityList::UpdateBosses, Conn, &Deadlock::Proc());
	s.every("UpdatePlayerControllers", std::chrono::milliseconds(50), EntityList::UpdatePlayerControllers, Conn, &Deadlock::Proc());
	s.every("FullUpdate", std::chrono::seconds(5), EntityList::FullUpdate, Conn, &Deadlock::Proc());
	s.every("UpdateLocalPlayerControllerAddress", std::chrono::seconds(15), Deadlock::UpdateLocalPlayerControllerAddress, Conn);

	while (!GetAsyncKeyState(VK_END))
	{
		MainWindow::OnFrame();
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}