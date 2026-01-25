#include "pch.h"

#include "DMA Thread.h"
#include "Input Manager.h"
#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Keybinds/Keybinds.h"

extern std::atomic<bool> bRunning;

void DMA_Thread_Main()
{
	std::println("[DMA Thread] DMA Thread started.");

	DMA_Connection* Conn = DMA_Connection::GetInstance();

	c_keys::InitKeyboard(Conn);

#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif

	if (!Deadlock::Initialize(Conn))
	{
		std::println("[DMA Thread] Deadlock Initialization failed, requesting exit.");
		bRunning = false;
		return;
	}

	auto Deadlock = &Deadlock::Proc();

	CTimer ViewMatrixTimer(std::chrono::milliseconds(1), [&Conn] {Deadlock::UpdateViewMatrix(Conn); });
	CTimer YawTimer(std::chrono::milliseconds(10), [&Conn] {Deadlock::UpdateClientYaw(Conn); });
	CTimer ServerTimeTimer(std::chrono::milliseconds(1000), [&Conn] {Deadlock::UpdateServerTime(Conn); });
	CTimer LocalControllerAddressTime(std::chrono::seconds(15), [&Conn] {Deadlock::UpdateLocalPlayerAddresses(Conn); });

	CTimer FullTrooperTimer(std::chrono::seconds(3), [&Conn, &Deadlock] {EntityList::FullTrooperRefresh(Conn, Deadlock); });
	CTimer QuickTrooperTimer(std::chrono::milliseconds(100), [&Conn, &Deadlock] {EntityList::QuickTrooperRefresh(Conn, Deadlock); });

	CTimer FullPawnTimer(std::chrono::seconds(2), [&Conn, &Deadlock] {EntityList::FullPawnRefresh_lk(Conn, Deadlock); });
	CTimer QuickPawnTimer(std::chrono::milliseconds(10), [&Conn, &Deadlock] {EntityList::QuickPawnRefresh(Conn, Deadlock); });

	CTimer FullMonsterCampTimer(std::chrono::seconds(2), [&Conn, &Deadlock] {EntityList::FullMonsterCampRefresh(Conn, Deadlock); });
	CTimer QuickMonsterCampTimer(std::chrono::milliseconds(100), [&Conn, &Deadlock] {EntityList::QuickMonsterCampRefresh(Conn, Deadlock); });

	CTimer FullControllerTimer(std::chrono::milliseconds(50), [&Conn, &Deadlock] {EntityList::FullControllerRefresh_lk(Conn, Deadlock); });
	CTimer FullSinnerTimer(std::chrono::milliseconds(1000), [&Conn, &Deadlock] {EntityList::FullSinnerRefresh(Conn, Deadlock); });
	CTimer FullUpdateTimer(std::chrono::seconds(5), [&Conn, &Deadlock] {EntityList::FullUpdate(Conn, Deadlock); });

	CTimer Keybinds(std::chrono::milliseconds(50), [&Conn]() { Keybinds::OnDMAFrame(Conn); });

	while (bRunning)
	{
		auto TimeNow = std::chrono::high_resolution_clock::now();
		ViewMatrixTimer.Tick(TimeNow);
		YawTimer.Tick(TimeNow);
		ServerTimeTimer.Tick(TimeNow);
		LocalControllerAddressTime.Tick(TimeNow);
		FullTrooperTimer.Tick(TimeNow);
		QuickTrooperTimer.Tick(TimeNow);
		FullPawnTimer.Tick(TimeNow);
		QuickPawnTimer.Tick(TimeNow);
		FullMonsterCampTimer.Tick(TimeNow);
		QuickMonsterCampTimer.Tick(TimeNow);
		FullControllerTimer.Tick(TimeNow);
		FullSinnerTimer.Tick(TimeNow);
		FullUpdateTimer.Tick(TimeNow);
		Keybinds.Tick(TimeNow);
	}

	Conn->EndConnection();
}