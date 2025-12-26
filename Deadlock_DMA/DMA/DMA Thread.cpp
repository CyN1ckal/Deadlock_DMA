#include "pch.h"

#include "DMA Thread.h"
#include "Deadlock/Deadlock.h"
#include "Deadlock/Entity List/EntityList.h"

#include "GUI/Keybinds/Keybinds.h"

template <typename T, typename F>
class CTimer
{
public:
	CTimer(T _Interval, F _Function) : m_Interval(_Interval), m_Function(_Function) {}

	void Tick(std::chrono::steady_clock::time_point CurrentTime)
	{
		if (std::chrono::duration_cast<T>(CurrentTime - m_LastExecutionTime) >= m_Interval)
		{
			m_Function();
			m_LastExecutionTime = CurrentTime;
		}
	}
private:
	T m_Interval{};
	F m_Function{};
	std::chrono::steady_clock::time_point m_LastExecutionTime{};
};

extern std::atomic<bool> bRunning;

void DMAThread(DMA_Connection* Conn, Process* Deadlock)
{
#ifdef TRACY_ENABLE
	tracy::SetThreadName("DMA Thread");
#endif

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

	CTimer HotkeyTimer(std::chrono::milliseconds(10), [&Conn] { Keybinds::OnFrame(Conn); });

	while (bRunning)
	{
		auto CurrentTime = std::chrono::steady_clock::now();

		ViewMatrixTimer.Tick(CurrentTime);
		YawTimer.Tick(CurrentTime);
		ServerTimeTimer.Tick(CurrentTime);
		LocalControllerAddressTime.Tick(CurrentTime);
		FullTrooperTimer.Tick(CurrentTime);
		QuickTrooperTimer.Tick(CurrentTime);
		FullPawnTimer.Tick(CurrentTime);
		QuickPawnTimer.Tick(CurrentTime);
		FullMonsterCampTimer.Tick(CurrentTime);
		QuickMonsterCampTimer.Tick(CurrentTime);
		FullControllerTimer.Tick(CurrentTime);
		FullSinnerTimer.Tick(CurrentTime);
		FullUpdateTimer.Tick(CurrentTime);
		HotkeyTimer.Tick(CurrentTime);

		auto ElapsedTime = std::chrono::steady_clock::now() - CurrentTime;
		auto TargetTime = std::chrono::milliseconds(2);

		if (ElapsedTime < TargetTime)
			std::this_thread::sleep_for(TargetTime - ElapsedTime);
	}
}