#include "pch.h"

#include "Offsets.h"

#include "Deadlock.h"

bool Offsets::ResolveOffsets(DMA_Connection* Conn)
{
	MultiScan ms;

	ms.Initialize(Conn->GetHandle(), Deadlock::Proc().GetPID(), "client.dll");

	ResolveLocalController(ms, Conn);

	ResolveGameEntitySystem(ms, Conn);

	ResolveViewMatrix(ms, Conn);

	ResolvePredictionPtr(ms, Conn);

	ResolveRotation(ms, Conn);

	ms.Close();

	return true;
}

bool Offsets::ResolveGameEntitySystem(MultiScan& ms, DMA_Connection* Conn)
{
	PatternInfo pi;
	pi.Mask = "xxx????xx";
	pi.Pattern = "\x48\x8B\x0D????\x8B\xFD";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 3);

	uint32_t Final = PatternOffset + 7 + Displacement;

	Offsets::GameEntitySystem = Final;

	std::println("[+] GameEntitySystem Offset: 0x{:X}", Offsets::GameEntitySystem);

	return false;
}

bool Offsets::ResolveLocalController(MultiScan& ms, DMA_Connection* Conn)
{
	PatternInfo pi;
	pi.Mask = "xxx";
	pi.Pattern = "\x48\x3B\x35";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 3);

	uint32_t Final = PatternOffset + 7 + Displacement;

	Offsets::LocalController = Final;

	std::println("[+] LocalController Offset: 0x{:X}", Offsets::LocalController);

	return true;
}

bool Offsets::ResolveViewMatrix(MultiScan& ms, DMA_Connection* Conn)
{
	PatternInfo pi;
	pi.Mask = "xxx????xxx";
	pi.Pattern = "\x49\x8D\x87????\x48\xC1\xE2";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 3);

	Offsets::ViewMatrix = Displacement;

	std::println("[+] ViewMatrix Offset: 0x{:X}", Offsets::ViewMatrix);

	return true;
}

bool Offsets::ResolvePredictionPtr(MultiScan& ms, DMA_Connection* Conn)
{
	PatternInfo pi;
	pi.Mask = "xxx????xx";
	pi.Pattern = "\x48\x8B\x05????\x38\x58";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 3);

	uint32_t Final = PatternOffset + 7 + Displacement;

	Offsets::PredictionPtr = Final;

	std::println("[+] PredictionPtr Offset: 0x{:X}", Offsets::PredictionPtr);

	return true;
}

bool Offsets::ResolveRotation(MultiScan& ms, DMA_Connection* Conn)
{
	PatternInfo pi;
	pi.Mask = "xxxx????xx????xxxx";
	pi.Pattern = "\xF2\x0F\x11\x05????\x89\x05????\xF2\x0F\x10\x06";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 4);

	Offsets::Rotation = PatternOffset + 8 + Displacement;

	std::println("[+] Rotation Offset: 0x{:X}", Offsets::Rotation);

	return true;
}
