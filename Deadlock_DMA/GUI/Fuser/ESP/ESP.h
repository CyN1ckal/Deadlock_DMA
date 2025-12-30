#pragma once

#include "Deadlock/Classes/Classes.h"

class ESP
{
public:
	static inline bool bMasterToggle{ true };

public:
	static void OnFrame();
	static void RenderSettings();
};