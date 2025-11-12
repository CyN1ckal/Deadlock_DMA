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

	s.every("FullUpdate", std::chrono::seconds(5), EntityList::FullUpdate, Conn, &Deadlock::Proc());
	s.every("FullPawnRefresh", std::chrono::milliseconds(10), EntityList::FullPawnRefresh, Conn, &Deadlock::Proc());
	s.every("FullControllerRefresh", std::chrono::milliseconds(50), EntityList::FullControllerRefresh, Conn, &Deadlock::Proc());
	s.every("FullTrooperRefresh", std::chrono::milliseconds(100), EntityList::FullTrooperRefresh, Conn, &Deadlock::Proc());
	s.every("FullMonsterCampRefresh", std::chrono::milliseconds(100), EntityList::FullMonsterCampRefresh, Conn, &Deadlock::Proc());
	s.every("FullSinnerRefresh", std::chrono::milliseconds(1000), EntityList::FullSinnerRefresh, Conn, &Deadlock::Proc());

	s.every("UpdateLocalPlayerYaw", std::chrono::milliseconds(10), Deadlock::UpdateClientYaw, Conn);
	s.every("UpdateServerTime", std::chrono::seconds(1), Deadlock::UpdateServerTime, Conn);
	s.every("UpdateLocalPlayerControllerAddress", std::chrono::seconds(15), Deadlock::UpdateLocalPlayerControllerAddress, Conn);

	while (!GetAsyncKeyState(VK_END))
	{
		MainWindow::OnFrame();
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}