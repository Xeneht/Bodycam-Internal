#pragma once
#include "includes.h"

inline void DrawMenu()
{
	ImGui::Begin("Bodycam Cheat Made By Xeneht (Unknowncheats.me) V1.7");

	if (ImGui::BeginTabBar("tabs"))
	{
		if (ImGui::BeginTabItem("Aimbot"))
		{
			ImGui::Checkbox("Aimbot", &gl::Aimbot::Aimbot);
			
			if (gl::Aimbot::Aimbot)
			{
				ImGui::Separator();

				ImGui::Checkbox("Show Fov", &gl::Aimbot::ShowFov);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Fov color", (float*)&gl::esp_Colors::FovColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::SliderFloat("##Fov Value", &gl::Aimbot::Fov, 0.f, 1000.f);

				ImGui::Text("Pitch Value");
				ImGui::SliderFloat("##Pitch Value", &gl::Aimbot::Pitch, -0.5f, 0.5f);
				ImGui::Checkbox("Aim Line", &gl::Aimbot::AimLine);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Aimline color", (float*)&gl::esp_Colors::AimLine, ImGuiColorEditFlags_NoInputs);
				
				ImGui::Spacing();
			}
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Instant Kill", &gl::Aimbot::InstantKill);
			ImGui::Checkbox("No Aiming Restrictions", &gl::Aimbot::AimLine);
			ImGui::Checkbox("Instant Aiming", &gl::Aimbot::InstantAiming);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visuals"))
		{
			ImGui::Checkbox("ESP", &gl::ESP::ESP);
			if (gl::ESP::ESP)
			{
				ImGui::Separator();

				ImGui::Checkbox("Visible", &gl::ESP::ESP_Visible);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Visible color", (float*)&gl::esp_Colors::VisibleColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Not Visible", &gl::ESP::ESP_NotVisible);
				ImGui::SameLine();
				ImGui::ColorEdit3("##NotVisible color", (float*)&gl::esp_Colors::NotVisibleColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Snaplines", &gl::ESP::SnapLines);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Snaplines color", (float*)&gl::esp_Colors::SnaplineColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::Combo("##SnaplinePosition", &selected_position, snapline_positions, IM_ARRAYSIZE(snapline_positions));

				ImGui::Checkbox("Nickname", &gl::ESP::Nicknames);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Nicknames color", (float*)&gl::esp_Colors::Nickname, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Health Bar", &gl::ESP::HealthBar);
				ImGui::Checkbox("Team Check", &gl::ESP::TeamCheck);
				ImGui::Spacing();
			}
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("No Flash", &gl::ESP::NoFlash);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("World");

			ImGui::Checkbox("Drones", &gl::World::Drones);
			ImGui::SameLine();
			ImGui::ColorEdit3("##Drones color", (float*)&gl::esp_Colors::Drones, ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox("Bomb", &gl::World::Bomb);
			ImGui::SameLine();
			ImGui::ColorEdit3("##Bomb color", (float*)&gl::esp_Colors::Bomb, ImGuiColorEditFlags_NoInputs);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Exploits"))
		{
			ImGui::Checkbox("View Fov", &gl::Exploits::Fov);
			ImGui::SameLine();
			ImGui::SliderFloat("##View Fov Value", &gl::Exploits::FovValue, 90.f, 160.f);

			ImGui::Checkbox("Gravity Multiplier", &gl::Exploits::Gravity);
			ImGui::SameLine();
			ImGui::SliderFloat("##Gravity Multiplier Value", &gl::Exploits::GravityValue, -1.f, 3.f);

			ImGui::Checkbox("No Recoil", &gl::Exploits::NoRecoil);

			ImGui::Checkbox("Rapid Fire", &gl::Exploits::RapidFire);
			ImGui::SameLine();
			ImGui::SliderFloat("##Rapid Fire Value", &gl::Exploits::RapidFireValue, 0.f, 1.f);

			ImGui::Checkbox("Full Auto", &gl::Exploits::FullAuto);
			ImGui::Checkbox("Unlimited Ammo", &gl::Exploits::UnlimitedAmmo);
			ImGui::Checkbox("GodMode", &gl::Exploits::GodMode);
			ImGui::SliderInt("##XP Value", &gl::Exploits::xp, 0, 50000);
			ImGui::SameLine();
			if (ImGui::Button("Apply XP"))
				gl::Exploits::xpApply = true;

			ImGui::SliderInt("##Rank Value", &gl::Exploits::killsQuantity, 0, 100);
			ImGui::SameLine();
			if (ImGui::Button("Add Kills"))
				gl::Exploits::addKills = true;

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Host Options"))
		{
			ImGui::Checkbox("Gravity Multiplier", &gl::HostOptions::Gravity);
			ImGui::SameLine();
			ImGui::SliderFloat("##Gravity Multiplier Value", &gl::HostOptions::GravityValue, -1.f, 3.f);

			ImGui::Checkbox("No Recoil", &gl::HostOptions::NoRecoil);

			ImGui::Checkbox("Rapid Fire", &gl::HostOptions::RapidFire);
			ImGui::SameLine();
			ImGui::SliderFloat("##Rapid Fire Value", &gl::HostOptions::RapidFireValue, 0.f, 1.f);

			ImGui::Checkbox("Full Auto", &gl::HostOptions::FullAuto);
			ImGui::Checkbox("No Damage Bullets", &gl::HostOptions::NoDamageBullets);
			ImGui::Checkbox("Remove Ammo", &gl::HostOptions::RemoveBullets);
			ImGui::Checkbox("Unlimited Ammo", &gl::HostOptions::UnlimitedAmmo);

			ImGui::Checkbox("Crazy Heads", &gl::HostOptions::CrazyHeads);
			ImGui::Checkbox("Kill All players", &gl::HostOptions::KillPlayers);
			ImGui::Checkbox("Kill All players (silent)", &gl::HostOptions::KillPlayersSilent);

			if (ImGui::Button("Finish Game"))
				gl::HostOptions::finishGame = true;

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc"))
		{
			ImGui::Checkbox("Show mouse", &gl::Misc::ShowMouse);
			if (ImGui::Button("Suicide"))
				gl::Misc::Suicide = true;

			ImGui::Checkbox("Hide SteamID In-Game", &gl::Misc::HideSteamId);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
			ImGui::Text("Credits");
			ImGui::PopFont();
			ImGui::Spacing();

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
			ImGui::Text("Developed by Xeneht");
			ImGui::PopFont();

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
			ImGui::TextWrapped("Valgar1 - Cheat was developed using his work as a reference, with his support.");
			ImGui::TextWrapped("Kiero Hook - The foundational base for this project.");
			ImGui::TextWrapped("Dumper-7 - The tool that makes you wonder if the game was ever closed source.");

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::TextWrapped("This cheat is freely available on UnknownCheats.me. If you have paid for it, you have been scammed.");
			
			ImGui::PopFont();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

inline void SetColorsFlags()
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	/*colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.88f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.88f, 0.88f, 0.90f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.53f, 0.53f, 0.53f, 0.69f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.83f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.65f, 0.65f, 0.65f, 0.87f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.47f, 0.47f, 0.20f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.80f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.71f, 0.71f, 0.30f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.57f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.79f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 0.45f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	colors[ImGuiCol_Tab] = ImVec4(0.42f, 0.42f, 0.42f, 0.79f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.84f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.28f, 0.28f, 0.57f, 0.82f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.35f, 0.35f, 0.65f, 0.84f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);*/



	ImGuiStyle& style = ImGui::GetStyle();

	/*style.FrameBorderSize = 1.f;
	style.TabBorderSize = 1.f;
	style.WindowTitleAlign.x = 0.50f;
	style.WindowPadding = ImVec2(5, 5);
	style.WindowRounding = 12.0f;
	style.FramePadding = ImVec2(6, 6);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 8.f;*/
}
