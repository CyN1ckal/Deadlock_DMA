#include "pch.h"

#include "Deadlock/Deadlock.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	while (!GetAsyncKeyState(VK_END))
	{
		Deadlock::UpdateViewMatrix(Conn);

		Vector2 ScreenPos;

		if (Deadlock::WorldToScreen(Vector3{ 0.0f,0.0f,0.0f }, ScreenPos))
			std::println("Screen Position: X: {}, Y: {}", ScreenPos.x, ScreenPos.y);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}