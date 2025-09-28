#include "pch.h"

#include "Deadlock/Deadlock.h"

int main()
{
	DMA_Connection* Conn = DMA_Connection::GetInstance();

	Deadlock::Initialize(Conn);

	Conn->EndConnection();

	system("pause");

	return 0;
}