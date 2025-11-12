#include "pch.h"

#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"
#include "GUI/Main Window/Main Window.h"

#ifdef CATCH_ENABLE
#include "Tests/All Tests.h"
#endif

#ifndef CATCH_ENABLE
int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	MainWindow::Initialize();

	Cppsched::Scheduler s(1);

#ifdef TRACY_ENABLE
	tracy::SetThreadName("Main Thread");
	s.in("InitialDelay", std::chrono::microseconds(1), []() { tracy::SetThreadName("DMA Thread"); });
#endif

	s.every("UpdateViewMatrix", std::chrono::microseconds(200), Deadlock::UpdateViewMatrix, Conn);

	s.every("FullTrooperRefresh", std::chrono::seconds(3), EntityList::FullTrooperRefresh, Conn, &Deadlock::Proc());
	s.every("QuickTrooperRefresh", std::chrono::milliseconds(100), EntityList::QuickTrooperRefresh, Conn, &Deadlock::Proc());

	s.every("FullPawnRefresh", std::chrono::seconds(2), EntityList::FullPawnRefresh, Conn, &Deadlock::Proc());
	s.every("QuickPawnRefresh", std::chrono::milliseconds(10), EntityList::QuickPawnRefresh, Conn, &Deadlock::Proc());

	s.every("FullMonsterCampRefresh", std::chrono::seconds(2), EntityList::FullMonsterCampRefresh, Conn, &Deadlock::Proc());
	s.every("QuickMonsterCampRefresh", std::chrono::milliseconds(100), EntityList::QuickMonsterCampRefresh, Conn, &Deadlock::Proc());

	s.every("FullUpdate", std::chrono::seconds(5), EntityList::FullUpdate, Conn, &Deadlock::Proc());
	s.every("FullControllerRefresh", std::chrono::milliseconds(50), EntityList::FullControllerRefresh, Conn, &Deadlock::Proc());
	s.every("FullSinnerRefresh", std::chrono::milliseconds(1000), EntityList::FullSinnerRefresh, Conn, &Deadlock::Proc());

	s.every("UpdateLocalPlayerYaw", std::chrono::milliseconds(10), Deadlock::UpdateClientYaw, Conn);
	s.every("UpdateServerTime", std::chrono::seconds(1), Deadlock::UpdateServerTime, Conn);
	s.every("UpdateLocalPlayerControllerAddress", std::chrono::seconds(15), Deadlock::UpdateLocalPlayerControllerAddress, Conn);

	while (!GetAsyncKeyState(VK_END))
	{
		MainWindow::OnFrame();
		FrameMark;
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}
#endif