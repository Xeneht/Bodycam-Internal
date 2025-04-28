#pragma once
#include "functions.h"
#include <algorithm>


inline bool firstDraw = true;

inline void DrawMenu()
{
	if (firstDraw)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 300));
		firstDraw = false;
	}
	ImGui::Begin("Bodycam Cheat Made By Xeneht (Unknowncheats.me) V0.1.4.0");

	if (ImGui::BeginTabBar("tabs"))
	{
		if (ImGui::BeginTabItem("Aimbot"))
		{
			ImGui::Checkbox("Aimbot", &gl::aimbot::aimbot);
			ImGui::SameLine();
			ImGui::Combo("##HitboxBone", &gl::aimbot::hitbox, hitboxes, IM_ARRAYSIZE(hitboxes));
			
			if (gl::aimbot::aimbot)
			{
				ImGui::Separator();

				ImGui::Checkbox("Show Fov", &gl::aimbot::showFov);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Fov color", (float*)&gl::espColors::fovColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::SliderFloat("##Fov Value", &gl::aimbot::fov, 0.f, 1000.f);

				ImGui::Checkbox("Aim Line", &gl::aimbot::aimLine);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Aimline color", (float*)&gl::espColors::aimLine, ImGuiColorEditFlags_NoInputs);

				ImGui::Spacing();
			}
			ImGui::Separator();

			ImGui::Checkbox("Instant Kill", &gl::aimbot::instantKill);
			ImGui::Checkbox("No Aiming Restrictions", &gl::aimbot::noAimingRestrictions);
			ImGui::Checkbox("Instant Aiming", &gl::aimbot::instantAiming);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Visuals"))
		{
			ImGui::Checkbox("ESP", &gl::esp::esp);
			if (gl::esp::esp)
			{
				ImGui::Separator();

				ImGui::Checkbox("Visible", &gl::esp::espVisible);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Visible color", (float*)&gl::espColors::visibleColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Not Visible", &gl::esp::espNotVisible);
				ImGui::SameLine();
				ImGui::ColorEdit3("##NotVisible color", (float*)&gl::espColors::notVisibleColor, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Snaplines", &gl::esp::snapLines);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Snaplines color", (float*)&gl::espColors::snaplineColor, ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine();
				ImGui::Combo("##SnaplinePosition", &selectedSnaplinePos, snaplinePositions, IM_ARRAYSIZE(snaplinePositions));


				if (ImGui::Checkbox("Players Chams", &gl::esp::playersChams))
					updateChams(ChamsType::Players);
				ImGui::SameLine();
				if (ImGui::Combo("##PlayerChams Type", &selectedPlayerChams, chamsTypes, IM_ARRAYSIZE(chamsTypes)))
					updateChams(ChamsType::Players);

				if (ImGui::Checkbox("Local Chams", &gl::esp::localChams))
					updateChams(ChamsType::Local);
				ImGui::SameLine();
				if (ImGui::Combo("##LocalChams Type", &selectedLocalChams, chamsTypes, IM_ARRAYSIZE(chamsTypes)))
					updateChams(ChamsType::Local);

				if (ImGui::Checkbox("Weapon Chams", &gl::esp::weaponChams))
					updateChams(ChamsType::Weapon);
				ImGui::SameLine();
				if (ImGui::Combo("##WeaponChams Type", &selectedWeaponChams, chamsTypes, IM_ARRAYSIZE(chamsTypes)))
					updateChams(ChamsType::Weapon);


				ImGui::Checkbox("Nicknames", &gl::esp::nicknames);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Nicknames color", (float*)&gl::espColors::nickname, ImGuiColorEditFlags_NoInputs);

				ImGui::Checkbox("Health Bar", &gl::esp::healthBar);
				ImGui::Checkbox("Skeleton", &gl::esp::skeleton);
				ImGui::Checkbox("Distance", &gl::esp::distance);
				ImGui::SameLine();
				ImGui::ColorEdit3("##Distance color", (float*)&gl::espColors::distanceColor, ImGuiColorEditFlags_NoInputs);
				ImGui::Spacing();
			}
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Team Check", &gl::esp::teamCheck);
			ImGui::Checkbox("No Flash", &gl::esp::noFlash);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("World");

			ImGui::Checkbox("Drones", &gl::world::drones);
			ImGui::SameLine();
			ImGui::ColorEdit3("##Drones color", (float*)&gl::espColors::drones, ImGuiColorEditFlags_NoInputs);

			ImGui::Checkbox("Bomb", &gl::world::bomb);
			ImGui::SameLine();
			ImGui::ColorEdit3("##Bomb color", (float*)&gl::espColors::bomb, ImGuiColorEditFlags_NoInputs);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Exploits"))
		{
			ImGui::Checkbox("View Fov", &gl::exploits::fov);
			ImGui::SameLine();
			ImGui::SliderFloat("##View Fov Value", &gl::exploits::fovValue, 90.f, 160.f);

			ImGui::Checkbox("Gravity Multiplier*", &gl::exploits::gravity);
			ImGui::SameLine();
			ImGui::SliderFloat("##Gravity Multiplier Value", &gl::exploits::gravityValue, -1.f, 3.f);

			ImGui::Checkbox("No Recoil", &gl::exploits::noRecoil);

			ImGui::Checkbox("Rapid Fire", &gl::exploits::rapidFire);
			ImGui::SameLine();
			ImGui::SliderFloat("##Rapid Fire Value", &gl::exploits::rapidFireValue, 0.f, 1.f);

			ImGui::Checkbox("Full Auto", &gl::exploits::fullAuto);
			ImGui::Checkbox("Unlimited Ammo", &gl::exploits::unlimitedAmmo);
			ImGui::Checkbox("GodMode*", &gl::exploits::godMode);

			ImGui::Checkbox("Teleport Enemies Infront", &gl::exploits::teleportEnemies);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::TextWrapped("Features marked with (*) only work if you are the host.");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("dev"))
		{
			static int SelectedMaterialIndex = -1;
			static std::string SelectedMaterialName = "";
			static char MaterialSearch[128] = "";

			if (ImGui::Button("Refresh materials"))
			{
				refreshMaterials();
				SelectedMaterialIndex = -1;
				SelectedMaterialName.clear();
			}

			std::lock_guard<std::mutex> lock(materialsMtx);
			if (!availableMaterials.empty())
			{
				ImGui::InputText("Search", MaterialSearch, IM_ARRAYSIZE(MaterialSearch));

				std::vector<std::string> FilteredMaterials;
				for (const auto& mat : availableMaterials)
				{
					std::string lowerMat = mat;
					std::string lowerSearch = MaterialSearch;

					std::transform(lowerMat.begin(), lowerMat.end(), lowerMat.begin(), ::tolower);
					std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

					if (lowerMat.find(lowerSearch) != std::string::npos)
					{
						FilteredMaterials.push_back(mat);
					}
				}

				std::vector<const char*> Items;
				for (const auto& mat : FilteredMaterials)
					Items.push_back(mat.c_str());

				if (ImGui::ListBox("Available Materials", &SelectedMaterialIndex, Items.data(), (int)Items.size(), 10))
				{
					if (SelectedMaterialIndex >= 0)
						SelectedMaterialName = FilteredMaterials[SelectedMaterialIndex];
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && SelectedMaterialIndex >= 0)
				{
					if (!SelectedMaterialName.empty()) {
						createChamsMaterials(SelectedMaterialName, ChamsType::Players);
						createChamsMaterials(SelectedMaterialName, ChamsType::Local);
						createChamsMaterials(SelectedMaterialName, ChamsType::Weapon);
					}
				}

				if (ImGui::Button("Apply material") && !SelectedMaterialName.empty())
				{
					createChamsMaterials(SelectedMaterialName, ChamsType::Players);
					createChamsMaterials(SelectedMaterialName, ChamsType::Local);
					createChamsMaterials(SelectedMaterialName, ChamsType::Weapon);
				}
			}


			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Misc"))
		{
			ImGui::Checkbox("Show mouse", &gl::misc::showMouse);
			if (ImGui::Button("Suicide"))
				gl::misc::suicide = true;


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

	ImGuiStyle& style = ImGui::GetStyle();
}
