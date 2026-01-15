#pragma once
#include "makcu/makcu.h"

class MyMakcu
{
public:
	static bool Initialize();
	static inline makcu::Device m_Device{};
};