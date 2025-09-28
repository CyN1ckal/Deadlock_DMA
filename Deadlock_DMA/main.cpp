#include "pch.h"

#include "Deadlock/Deadlock.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	std::println("Press END to exit...");

	while(!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	Conn->EndConnection();

	system("pause");

	return 0;
}