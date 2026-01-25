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

	ms.Close();

	DbgPrintln("All offsets resolved.");

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
	pi.Mask = "xx????xx????xx?????xx????xxxxxxx?xx?????x????xxxxxxxxxx????xxx????xxxx?????xxxxx?????xx????xxx?????xxx????xxxx????xxx";
	pi.Pattern = "\x39\x0D????\x0F\x8F????\x80\x3D?????\x0F\x85????\x48\x8B\xCB\x48\x89\x7C\x24?\xC6\x05?????\xE8????\x48\x8B\xD0\x45\x33\xC0\x48\x8B\xCE\xE8????\x48\x8D\x3D????\x48\xC7\x44\x24?????\x48\x8B\xD7\xC6\x05?????\x41\xB9????\xC7\x44\x24?????\x4C\x8D\x05????\x48\x8B\xCE\xE8????\x48\x8D\x05";
	uint32_t PatternOffset = ms.ScanOffset(pi);

	uint32_t Displacement = Deadlock::Proc().ReadMem<uint32_t>(Conn, Deadlock::Proc().GetClientBase() + PatternOffset + 2);

	Offsets::ViewMatrix = PatternOffset + 6 + Displacement - ms.GetSectionBase();

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