#include "pch.h"

#include "Deadlock/Deadlock.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	auto GameProc = Deadlock::Proc();

	try
	{
		auto MemRead = GameProc.ReadMem<uint32_t>(Conn, GameProc.GetBaseAddress());

		std::println("Memory Read: 0x{:X}", MemRead);
	}
	catch (std::runtime_error& e)
	{
		std::println("Error: {}", e.what());
	}
	catch (...)
	{
		std::println("Unknown error occurred");
	}

	Conn->EndConnection();

	system("pause");

	return 0;
}