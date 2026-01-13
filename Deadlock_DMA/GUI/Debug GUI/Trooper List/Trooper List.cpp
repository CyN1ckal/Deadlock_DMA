#include "pch.h"
#include "Trooper List.h"
#include "Deadlock/Entity List/EntityList.h"

void TrooperList::Render()
{
	if (!bSettings) return;

	ImGui::Begin("Trooper List", &bSettings);

	/* m_ControllerMutex is required for friendly check */
	std::scoped_lock Lock(EntityList::m_TrooperMutex, EntityList::m_ControllerMutex);

	static bool bHideFriendly = true;
	static bool bHideDormant = false;
	ImGui::Checkbox("Hide Friendly", &bHideFriendly);
	ImGui::SameLine();
	ImGui::Checkbox("Hide Dormant", &bHideDormant);

	ImGui::Spacing();

	// Display total count
	size_t visibleCount = 0;
	for (auto& Trooper : EntityList::m_Troopers)
	{
		if (Trooper.IsInvalid()) continue;
		if (bHideFriendly && Trooper.IsFriendly()) continue;
		if (bHideDormant && Trooper.IsDormant()) continue;
		visibleCount++;
	}
	ImGui::Text("Troopers: %zu", visibleCount);

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::BeginTable("Troopers Table", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
	{
		ImGui::TableSetupColumn("Team", ImGuiTableColumnFlags_WidthFixed, 60.0f);
		ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableSetupColumn("Distance", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed, 150.0f);
		ImGui::TableSetupColumn("Flags", ImGuiTableColumnFlags_WidthFixed, 50.0f);
		ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed, 70.0f);
		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 80.0f);
		ImGui::TableHeadersRow();

		uint32_t TrooperNum = 0;
		for (auto& Trooper : EntityList::m_Troopers)
		{
			if (Trooper.IsInvalid()) continue;
			if (bHideFriendly && Trooper.IsFriendly()) continue;
			if (bHideDormant && Trooper.IsDormant()) continue;

			ImGui::TableNextRow();

			// Team Column
			ImGui::TableNextColumn();
			bool isFriendly = Trooper.IsFriendly();
			if (isFriendly)
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Team %d", Trooper.m_TeamNum);
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Team %d", Trooper.m_TeamNum);

			// Health Column
			ImGui::TableNextColumn();
			float healthPercent = (float)Trooper.m_CurrentHealth / 100.0f;
			ImVec4 healthColor;
			if (healthPercent > 0.6f)
				healthColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
			else if (healthPercent > 0.3f)
				healthColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
			else
				healthColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			ImGui::TextColored(healthColor, "%d", Trooper.m_CurrentHealth);

			// Distance Column
			ImGui::TableNextColumn();
			float distance = Trooper.DistanceFromLocalPlayer(true);
			ImGui::Text("%.1f m", distance);

			// Position Column
			ImGui::TableNextColumn();
			ImGui::Text("%.0f, %.0f, %.0f", Trooper.m_Position.x, Trooper.m_Position.y, Trooper.m_Position.z);

			// Flags Column
			ImGui::TableNextColumn();
			ImGui::Text("0x%02X", Trooper.m_Flags);

			// State Column
			ImGui::TableNextColumn();
			if (Trooper.IsDormant())
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Dormant");
			else
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Active");

			// Address Column
			ImGui::TableNextColumn();
			if (ImGui::Button(std::format("Copy##{}", TrooperNum).c_str()))
				ImGui::SetClipboardText(std::format("0x{:X}", Trooper.m_EntityAddress).c_str());

			TrooperNum++;
		}

		ImGui::EndTable();
	}

	ImGui::End();
}