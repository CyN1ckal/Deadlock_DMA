#include "pch.h"

#include "Class List.h"

#include "Deadlock/Entity List/EntityList.h"

void ClassList::Render()
{
	std::scoped_lock Lock(EntityList::m_ClassMapMutex);

	ImGui::Begin("Class List");

	for (auto&& [ClassName, Address] : EntityList::m_EntityClassMap)
	{
		std::string DisplayString = std::format("{0:s} @ {1:X}", ClassName, Address);
		ImGui::Text(DisplayString.c_str());
	}

	ImGui::End();
}