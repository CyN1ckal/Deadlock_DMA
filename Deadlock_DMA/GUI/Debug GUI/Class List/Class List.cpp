#include "pch.h"
#include "Class List.h"
#include "Deadlock/Entity List/EntityList.h"

void ClassList::Render()
{
	if (!bSettings) return;

	ImGui::Begin("Class List", &bSettings);

	std::scoped_lock Lock(EntityList::m_ClassMapMutex);

	// Display total count
	ImGui::Text("Total Classes: %zu", EntityList::m_EntityClassMap.size());

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Search filter
	static char searchBuffer[256] = "";
	ImGui::SetNextItemWidth(-1);
	ImGui::InputTextWithHint("##search", "Search class name...", searchBuffer, sizeof(searchBuffer));

	ImGui::Spacing();

	if (ImGui::BeginTable("Classes Table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
	{
		ImGui::TableSetupColumn("Class Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 120.0f);
		ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableHeadersRow();

		uint32_t ClassNum = 0;
		std::string searchStr = searchBuffer;
		std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);

		for (auto&& [ClassName, Address] : EntityList::m_EntityClassMap)
		{
			// Filter by search
			if (!searchStr.empty())
			{
				std::string lowerClassName = ClassName;
				std::transform(lowerClassName.begin(), lowerClassName.end(), lowerClassName.begin(), ::tolower);
				if (lowerClassName.find(searchStr) == std::string::npos)
					continue;
			}

			ImGui::TableNextRow();

			// Class Name Column
			ImGui::TableNextColumn();
			ImGui::Text("%s", ClassName.c_str());

			// Address Column
			ImGui::TableNextColumn();
			ImGui::Text("0x%llX", Address);

			// Actions Column
			ImGui::TableNextColumn();
			if (ImGui::Button(std::format("Copy##{}", ClassNum).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", Address).c_str());

			ClassNum++;
		}

		ImGui::EndTable();
	}

	ImGui::End();
}