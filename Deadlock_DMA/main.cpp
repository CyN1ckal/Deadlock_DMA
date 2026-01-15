#include "pch.h"

#include "Deadlock/Deadlock.h"
#include "GUI/Main Window/Main Window.h"

#include "DMA/DMA Thread.h"

#include "GUI/Config/Config.h"

#include "Makcu/MyMakcu.h"

#ifdef CATCH_ENABLE
#include "Tests/All Tests.h"
#endif

std::atomic<bool> bRunning{ true };
#ifndef CATCH_ENABLE
int main()
{
	std::println("Hello, DEADLOCK_DMA!");

	Config::LoadConfig("default");


#ifndef DEADLOCK_DLL
	MainWindow::Initialize();
#endif

	MyMakcu::Initialize();

	std::thread DMAThread(DMA_Thread_Main);

	std::println("Press END to exit...");

	while (bRunning)
	{
		if (GetAsyncKeyState(VK_END) & 0x1) bRunning = false;
#ifndef DEADLOCK_DLL
		MainWindow::OnFrame();
#endif
#ifdef DEADLOCK_DLL
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
		FrameMark;
	}

	DMAThread.join();

	system("pause");

	return 0;
}
#endif

#ifdef DEADLOCK_DLL
extern "C" __declspec(dllexport) void Render(ImGuiContext* ctx)
{
	ImGui::SetCurrentContext(ctx);
	MainWindow::RenderOnly();
}
DWORD WINAPI StartingThread(HMODULE hMod)
{
	return main();
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ReasonForCall, LPVOID lpReserved)
{
	switch (ReasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartingThread, hModule, 0, 0);
		break;
	}

	return TRUE;
}
#endif