#include "pch.h"

#include "Deadlock/Deadlock.h"
#include "GUI/Main Window/Main Window.h"

#include "DMA/DMA Thread.h"

#ifdef CATCH_ENABLE
#include "Tests/All Tests.h"
#endif

std::atomic<bool> bRunning{ true };
#ifndef CATCH_ENABLE
int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	MainWindow::Initialize();

	std::thread DMAThreadHandle(DMAThread, Conn, &Deadlock::Proc());

	while (bRunning)
	{
		if (GetAsyncKeyState(VK_END) & 0x1) bRunning = false;
		MainWindow::OnFrame();
		FrameMark;
	}

	DMAThreadHandle.join();

	Conn->EndConnection();

	system("pause");

	return 0;
}
#endif