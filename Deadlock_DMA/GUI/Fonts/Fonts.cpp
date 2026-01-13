#include "pch.h"
#include "Fonts.h"
#include "Data/IBMPlexMono.h"

void Fonts::Initialize(ImGuiIO& io)
{
	m_IBMPlexMonoSemiBold = io.Fonts->AddFontFromMemoryTTF(IBMPlexMonoSemiBoldData, sizeof(IBMPlexMonoSemiBoldData), 16.0f);
	IM_ASSERT(m_IBMPlexMonoSemiBold != nullptr);
}