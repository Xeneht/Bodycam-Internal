#include "GameLoop.h"
#include "functions.h"

void GameLoop()
{
	if (WorldActors.size() > 0)
	{
		for (int i = 0; i < WorldActors.size(); i++)
		{
			// Check if any of the world esp options are enabled
			if (!gl::World::Drones && !gl::World::Bomb) break;

			if (i >= WorldActors.size()) break;
			if (!WorldActors[i]) continue;
			World = SDK::UWorld::GetWorld();
			if (!World) continue;
			if (IsBadPoint(World)) continue;
			SDK::AActor* actor = WorldActors[i];
			if (!actor) continue;
			if (IsBadPoint(actor)) continue;
			if (!MyController) continue;
			if (IsBadPoint(MyController)) continue;

			// Get the local player character
			auto LocalCharacter = reinterpret_cast<SDK::AALS_AnimMan_CharacterBP_C*>(MyController->Character);
			if (IsBadPoint(LocalCharacter)) continue;
			double localHealth = 0.0;

			// Get the local player survivor and check if is alive
			auto SurvivorStatus = LocalCharacter->WW_SurvivorStatus;
			if (SurvivorStatus)
				localHealth = SurvivorStatus->Health;
			if (localHealth <= 0) continue;

			if (IsBadPoint(actor->RootComponent)) continue;
			if (!actor->RootComponent) continue;
			
			// Get the actor location and check if is valid
			SDK::FVector* locationPtr = &(actor->RootComponent->RelativeLocation);
			if (IsBadPoint(locationPtr)) continue;
			SDK::FVector location = *locationPtr;

			SDK::FVector2D Pos{};
			SDK::FVector Center{};
			SDK::FVector Bounds{};

			// Get the actor bounds and center
			actor->GetActorBounds(true, &Center, &Bounds, false);

			// Draw the text esp
			if (MyController->ProjectWorldLocationToScreen(location, &Pos, false)) {
				if (gl::World::Drones && actor->GetName().find("BP_Drone") != std::string::npos)
					ESP::DrawText2({ static_cast<float>(Pos.X), static_cast<float>(Pos.Y) }, gl::esp_Colors::Drones, "Drone");
				
				if (gl::World::Bomb && actor->GetName().find("Bomb") != std::string::npos)
					ESP::DrawText2({ static_cast<float>(Pos.X), static_cast<float>(Pos.Y) }, gl::esp_Colors::Bomb, "Bomb");
			}
		}
	}

	if (PlayerList.size() > 0)
	{
		SDK::UALS_AnimBP_C* AnimBP = nullptr;
		SDK::AActor* target_actor{};
		SDK::FRotator target_rotation{};
		float target_distance{};

		Vec2 screen_middle2D{};
		Vec2 target_middle2D{};

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (i >= PlayerList.size()) break;
			if (!PlayerList[i]) continue;
			SDK::AActor* actor = PlayerList[i];
			if (!actor) continue;
			if (IsBadPoint(actor)) continue;
			if (!LocalActor) continue;
			if (IsBadPoint(LocalActor)) continue;
			if (!actor->RootComponent) continue;


			// Shitty checks
			if (IsBadPoint(actor->RootComponent)) continue;
			if (i >= PlayerList.size()) break;
			if (!World) continue;
			if(IsBadPoint(World->OwningGameInstance)) continue;
			if (!World->OwningGameInstance) continue;
			if (!World->OwningGameInstance->LocalPlayers[0]) continue;
			if (!World->OwningGameInstance->LocalPlayers[0]->PlayerController) continue;

			// Get the local player controller and character
			MyController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
			auto LocalController = reinterpret_cast<SDK::APlayerController*>(MyController);
			auto LocalCharacter = reinterpret_cast<SDK::AALS_AnimMan_CharacterBP_C*>(MyController->Character);
			if (!LocalCharacter) continue;

			// Hide Steam Id and Name on the VHS overlay
			if (gl::Misc::HideSteamId) {
				SDK::FString steamIdString = SDK::FString(L"[REDACTED]");
				SDK::FText steamIdText = SDK::UKismetTextLibrary::Conv_StringToText(steamIdString);
			
				SDK::FString nickString = SDK::FString(L"Unknowncheats.me");
				SDK::FText nickText = SDK::UKismetTextLibrary::Conv_StringToText(nickString);

				if (LocalCharacter->PC_Bodycam)
					if (LocalCharacter->PC_Bodycam->HUD_Character)
						if (LocalCharacter->PC_Bodycam->HUD_Character->WBP_Vhs) {
							auto vhs = LocalCharacter->PC_Bodycam->HUD_Character->WBP_Vhs;
							if (vhs->SteamID)
								vhs->SteamID->SetText(steamIdText);
							if (vhs->Nickname)
								vhs->Nickname->SetText(nickText);
						}
			}

			// Add Kills
			if (gl::Exploits::addKills) {
				auto pcb = LocalCharacter->PC_Bodycam;
				if (pcb)
					if (!IsBadPoint(pcb)) {
						for (int i = 0; i < gl::Exploits::killsQuantity; i++)
							pcb->Update_Kill(1);
						gl::Exploits::addKills = false;
					}
			}
			
			// Add Xp
			if (gl::Exploits::xpApply) {
				auto pcb = LocalCharacter->PC_Bodycam;
				if (pcb)
					if (!IsBadPoint(pcb)) {
						pcb->UpdateXp(gl::Exploits::xp);
						gl::Exploits::xpApply = false;
					}
			}

			if (!MyController) continue;
			if (!MyController->Character) continue;
			if (!MyController->Character->Mesh) continue;

			// Get the local player health
			double localHealth = 0.0;
			auto SurvivorStatus = LocalCharacter->WW_SurvivorStatus;

			if (SurvivorStatus)
				localHealth = SurvivorStatus->Health;
			if (localHealth <= 0) continue;

			auto ActorState = actor->Instigator->PlayerState;
			if (!ActorState) continue;
			if (IsBadPoint(ActorState)) continue;
			if (!MyController->PlayerState) continue;
			if (IsBadPoint(MyController->PlayerState)) continue;

			// Get the local player actor
			bool isLocalPlayer = false;
			if (ActorState == MyController->PlayerState) {
				LocalActor = actor;
				isLocalPlayer = true;
			}

			////////////////////////////////--------------- WEAPON OPTIONS -------------------////////////////////////////////////////

			// Get the current weapon
			SDK::AWEP_C* pWeapon = LocalCharacter->WEP;
			if (pWeapon && !IsBadPoint(pWeapon))
			{
				// No recoil
				if (gl::Exploits::NoRecoil) {
					pWeapon->RecoilRot = SDK::FVector(0, 0, 0);
					pWeapon->RecoilLoc = SDK::FVector(0, 0, 0);
					pWeapon->RecoilRandomloc = SDK::FVector(0, 0, 0);
					pWeapon->RecoilRandomRot = SDK::FVector(0, 0, 0);
					pWeapon->Recoil_WepRotation = SDK::FVector(0, 0, 0);
					pWeapon->Aim_Rot = SDK::FVector(0, 0, 0);
					pWeapon->Aim_Loc = SDK::FVector(0, 0, 0);
					pWeapon->LeanSwitchLoc = SDK::FVector(0, 0, 0);
					pWeapon->LeanSwitchRot = SDK::FVector(0, 0, 0);
					pWeapon->ShakeCurrent = SDK::FVector(0, 0, 0);
					pWeapon->SmoothRotIdle = 0;
					pWeapon->Wep_Info.RecoilInfo_14_2F550DD548C99F85FB2D1C911E29083C.CameraShake_13_B0DD31C04CD0D1983602ADB8F281516E = 0;
					pWeapon->Spread_Multiplier = 0;
				}
				// Unlimited ammo
				if (gl::Exploits::UnlimitedAmmo && !IsBadPoint(pWeapon)) {
					pWeapon->BulletInChamber = 1;
				}
				// Rapid fire
				if (gl::Exploits::RapidFire) {
					pWeapon->Wep_Info.RecoilInfo_14_2F550DD548C99F85FB2D1C911E29083C.FireRate_11_A93DD4B84C6696E6E0EC3BA044512B73 = static_cast<double>(gl::Exploits::RapidFireValue);
				}
				// Full auto
				if (gl::Exploits::FullAuto) {
					pWeapon->Wep_Info.FireMode_39_E7AEF4D24EEC5420D75555A92E651905 = SDK::E_FireMode::NewEnumerator1;
				}
				// Instant kill
				if (gl::Aimbot::InstantKill) {
					pWeapon->Wep_Info.Damage_17_7F60A88D435DBD7B36C96893C2118EAC = 999;
				}
				// Insta aim
				if (gl::Aimbot::InstantAiming) {
					pWeapon->Wep_Info.Animation_23_574905EC43F77C89F6A965AFE119106B.ADSInTIme_39_E74D36F9478596C143C5C0A4B3A694F9 = 0.03;
					pWeapon->Wep_Info.Animation_23_574905EC43F77C89F6A965AFE119106B.ADSOutTime_41_63DF5BF04A00BD2A761C11861660F122 = 0.03;
				}
				// No Aiming restrictions
				if (gl::Aimbot::NoAimingRestrictions) {
					LocalCharacter->AimingBloked = false;
				}
			}
			// No flash
			if (gl::ESP::NoFlash) {
				if (LocalCharacter->FlashBangEffect)
					if (LocalCharacter->FlashBangEffect < 0)
						if (LocalCharacter->FlashBangEar)
							LocalCharacter->FlashBangEar->SetVolumeMultiplier(0);
				LocalCharacter->FlashBangEffect = 0;

			}
			// God mode
			if (gl::Exploits::GodMode) {
				SurvivorStatus->Health = 999;
			}
			// Fov
			if (LocalCharacter->Camera && gl::Exploits::Fov) {
				auto Camera = reinterpret_cast<SDK::UCameraComponent*>(LocalCharacter->Camera);
				if (Camera)
					if (!IsBadPoint(Camera))
						Camera->FieldOfView = gl::Exploits::FovValue;
			}
			// Gravity
			if (gl::Exploits::Gravity)
				if (LocalCharacter->CharacterMovement)
					LocalCharacter->CharacterMovement->GravityScale = gl::Exploits::GravityValue;


			// Get the actor character
			auto Character = reinterpret_cast<SDK::AALS_AnimMan_CharacterBP_C*>(actor);
			if (!Character) continue;


			////////////////////////////////--------------- HOST OPTIONS -------------------////////////////////////////////////////

			SDK::AWEP_C* aWeapon = Character->WEP;
			if (pWeapon && !IsBadPoint(aWeapon) && !isLocalPlayer)
			{
				// No recoil
				if (gl::HostOptions::NoRecoil) {
					aWeapon->RecoilRot = SDK::FVector(0, 0, 0);
					aWeapon->RecoilLoc = SDK::FVector(0, 0, 0);
					aWeapon->RecoilRandomloc = SDK::FVector(0, 0, 0);
					aWeapon->RecoilRandomRot = SDK::FVector(0, 0, 0);
					aWeapon->Recoil_WepRotation = SDK::FVector(0, 0, 0);
					aWeapon->Aim_Rot = SDK::FVector(0, 0, 0);
					aWeapon->Aim_Loc = SDK::FVector(0, 0, 0);
					aWeapon->LeanSwitchLoc = SDK::FVector(0, 0, 0);
					aWeapon->LeanSwitchRot = SDK::FVector(0, 0, 0);
					aWeapon->ShakeCurrent = SDK::FVector(0, 0, 0);
					aWeapon->SmoothRotIdle = 0;
					aWeapon->Wep_Info.RecoilInfo_14_2F550DD548C99F85FB2D1C911E29083C.CameraShake_13_B0DD31C04CD0D1983602ADB8F281516E = 0;
					aWeapon->Spread_Multiplier = 0;
				}
				// Unlimited ammo
				if (gl::HostOptions::UnlimitedAmmo && !IsBadPoint(aWeapon)) {
					aWeapon->BulletInChamber = 1;
				}
				// Rapid fire
				if (gl::HostOptions::RapidFire) {
					aWeapon->Wep_Info.RecoilInfo_14_2F550DD548C99F85FB2D1C911E29083C.FireRate_11_A93DD4B84C6696E6E0EC3BA044512B73 = static_cast<double>(gl::Exploits::RapidFireValue);
				}
				// Full auto
				if (gl::HostOptions::FullAuto) {
					aWeapon->Wep_Info.FireMode_39_E7AEF4D24EEC5420D75555A92E651905 = SDK::E_FireMode::NewEnumerator1;
				}
				if (gl::HostOptions::NoDamageBullets)
					if (Character->WEP && !IsBadPoint(Character->WEP))
						Character->WEP->Wep_Info.Damage_17_7F60A88D435DBD7B36C96893C2118EAC = 0;

				if (gl::HostOptions::RemoveBullets)
					if (Character->WEP && !IsBadPoint(Character->WEP))
						Character->WEP->BulletInChamber = 0;
			}
			// Gravity
			if (gl::HostOptions::Gravity && !isLocalPlayer)
				if (Character->CharacterMovement)
					Character->CharacterMovement->GravityScale = gl::HostOptions::GravityValue;

			if (gl::HostOptions::CrazyHeads && !isLocalPlayer) {
				if (actor->Instigator)
					if (actor->Instigator->Controller)
						actor->Instigator->Controller->SetControlRotation(SDK::FRotator(rand() % 360 - 180, rand() % 360 - 180, rand() % 360 - 180));
			}

			// Kill All Players
			if (gl::HostOptions::KillPlayers && !isLocalPlayer) {
				SDK::FString headString = SDK::FString(L"Head");
				SDK::FName head = SDK::UKismetStringLibrary::Conv_StringToName(headString);
				Character->ApplyPointDamage(101, 100, SDK::FName(head), SDK::FVector(0, 0, 0), SDK::FVector(0, 0, 0), MyController, true);
			}

			// Kill All Players Silent
			if (gl::HostOptions::KillPlayersSilent && !isLocalPlayer) {
				if (Character->WW_SurvivorStatus)
					if (Character->WW_SurvivorStatus->Health)
						Character->WW_SurvivorStatus->Remove_Health(101);
			}

			// Suicide
			if (gl::Misc::Suicide) {
				if (LocalCharacter->WW_SurvivorStatus) {
					LocalCharacter->WW_SurvivorStatus->Remove_Health(101);
					gl::Misc::Suicide = false;
				}
			}

			if (gl::HostOptions::finishGame && !isLocalPlayer) {
				auto pcb = Character->PC_Bodycam;
				if (pcb)
					if (!IsBadPoint(pcb)) {
						SDK::FSTR_Score score = SDK::FSTR_Score();
						score.MilitaryScore_2_FC1428E340B9C22D2885F78FFDB25C9F = 10;
						score.TerroristScore_4_8EB299E64F8EA15DF9366CBE042FD9C7 = 10;
						pcb->UpdateScore(score);
						gl::HostOptions::finishGame = false;
					}
			}



			////////////////////////////////--------------- ESP -------------------////////////////////////////////////////



			// Visibility check
			bool IsActorVisible = false;
			if (!MyController->PlayerCameraManager) continue;
			if (IsBadPoint(MyController->PlayerCameraManager)) continue;
			if (MyController->PlayerCameraManager->CameraCachePrivate.POV.Location.IsZero()) continue;
			if (MyController->LineOfSightTo(actor, MyController->PlayerCameraManager->CameraCachePrivate.POV.Location, false))
				IsActorVisible = true;

			// Get the actor bounds and center
			SDK::FVector Center{};
			SDK::FVector Bounds{};
			SDK::FVector2D Top{};
			SDK::FVector2D Bottom{};
			SDK::FVector2D Pos{};

			actor->GetActorBounds(true, &Center, &Bounds, false);

			// Team check
			bool differentTeam = true;
			if (gl::ESP::TeamCheck) {
				if (IsBadPoint(&(LocalCharacter->Team))) continue;
				int localPlayerTeam = static_cast<int>(LocalCharacter->Team);
				if (IsBadPoint(&(Character->Team))) continue;
				int actorTeam = static_cast<int>(Character->Team);
				if (localPlayerTeam == actorTeam)
					differentTeam = false;
			}

			SDK::FVector Top3D = Center + SDK::FVector(0, 0, Bounds.Z);
			SDK::FVector Bottom3D = Center - SDK::FVector(0, 0, Bounds.Z);
			if (!MyController->ProjectWorldLocationToScreen(Top3D, &Top, false)) continue;
			if (!MyController->ProjectWorldLocationToScreen(Bottom3D, &Bottom, false)) continue;

			// Dead check and health
			bool isDead = false;
			Character->IsDead(&isDead);
			double health = 0.0;
			if (Character->WW_SurvivorStatus)
				health = Character->WW_SurvivorStatus->Health;


			// Check if it's alive, esp is enabled and if (player is an enemie or team check is disabled)
			if (!isDead && gl::ESP::ESP && (differentTeam || !gl::ESP::TeamCheck)) {
				// Not visible esp
				if (!IsActorVisible && gl::ESP::ESP_NotVisible)
				{
					ESP::Draw3DBox(Center, Bounds, MyController, gl::esp_Colors::NotVisibleColor);
				}
				// Visible esp
				else if (IsActorVisible && gl::ESP::ESP_Visible)
				{
					ESP::Draw3DBox(Center, Bounds, MyController, gl::esp_Colors::VisibleColor);
				}
				if (gl::ESP::SnapLines)
				{
					// Snaplines
					Vec2 snaplineOrigin2D = {};
					switch (selected_position)
					{
					case 0:
						snaplineOrigin2D = { static_cast<float>(screenWidth) / 2, 0 };
						break;
					case 1:
						snaplineOrigin2D = { static_cast<float>(screenWidth) / 2, static_cast<float>(screenHeight) / 2 };
						break;
					case 2:
						snaplineOrigin2D = { static_cast<float>(screenWidth) / 2, static_cast<float>(screenHeight) };
						break;
					}

					auto playerTop = SDK::FVector(Center.X, Center.Y, Center.Z + Bounds.Z);
					
					SDK::FVector2D snaplineEnd2D{};
					if (!MyController->ProjectWorldLocationToScreen(playerTop, &snaplineEnd2D, false)) continue;
					Vec2 snaplineEnd = { snaplineEnd2D.X, snaplineEnd2D.Y };

					ESP::DrawLine(snaplineOrigin2D, snaplineEnd, gl::esp_Colors::SnaplineColor, 1);
				}
				// Health bar
				if (gl::ESP::HealthBar)
				{
					const float h = std::abs(Top.Y - Bottom.Y);
					const float w = h * 0.2f;
					ESP::DrawBoxFilled(Vec2(Top.X - w - 6, Top.Y - 1), Vec2(Top.X - w - 3, Bottom.Y + 1), Colors::Black);
					ESP::DrawBoxFilled(Vec2(Top.X - w - 5, Bottom.Y - (h * (health / 100.0f))), Vec2(Top.X - w - 4, Bottom.Y), Colors::Green);
				}
				// Nicknames
				if (gl::ESP::Nicknames)
				{
					if (actor->Instigator->PlayerState && !IsBadPoint(actor->Instigator->PlayerState) && actor->Instigator->PlayerState->PlayerNamePrivate) {
						SDK::FString playername = actor->Instigator->PlayerState->PlayerNamePrivate;
						float estimatedTextHeight = 20.0f;
						float marginLeft = 15.0f;
						if (playername.ToString().length() > 0)
							ESP::DrawText2({ static_cast<float>(Top.X - marginLeft), static_cast<float>(Top.Y - estimatedTextHeight) }, Colors::White, playername.ToString().c_str());
					}
				}
			}

			////////////////////////////////--------------- AIMBOT TARGETING -------------------////////////////////////////////////////

			if (gl::Aimbot::Aimbot) {

				if (isDead) continue;
				if (!differentTeam) continue;
				if (!IsActorVisible) continue;

				auto CameraLocation = MyController->PlayerCameraManager->GetCameraLocation();
				
				auto playerMiddle3D = SDK::FVector(Center.X, Center.Y, Center.Z - (Bounds.Z * gl::Aimbot::Pitch));
				auto rot = SDK::UKismetMathLibrary::FindLookAtRotation(CameraLocation, playerMiddle3D);

				SDK::FVector2D screen_middle = { screenWidth / 2, screenHeight / 2 };
				SDK::FVector2D playerMiddle2D{};
				if (!MyController->ProjectWorldLocationToScreen(playerMiddle3D, &playerMiddle2D, false)) continue;

				aimbot_distance = SDK::UKismetMathLibrary::Distance2D(playerMiddle2D, screen_middle);

				// Aimbot
				// Check inside fov and closest target
				if (aimbot_distance <= gl::Aimbot::Fov) {
					if (target_distance > 0 && target_distance < aimbot_distance) continue;

					target_actor = actor;
					target_rotation = rot;
					target_distance = aimbot_distance;

					screen_middle2D = { static_cast<float>(screen_middle.X), static_cast<float>(screen_middle.Y) };
					target_middle2D = { static_cast<float>(playerMiddle2D.X), static_cast<float>(playerMiddle2D.Y) };
				}
			}
		}
		
		////////////////////////////////--------------- AIMBOT -------------------////////////////////////////////////////

		if (gl::Aimbot::Aimbot) {
			if (gl::Aimbot::ShowFov)
				ESP::DrawCircle(Vec2(screenWidth / 2, screenHeight / 2), gl::Aimbot::Fov, gl::esp_Colors::FovColor, 80);
			if (target_actor && !IsBadPoint(target_actor)) {
				// Aimline
				if (gl::Aimbot::AimLine)
					ESP::DrawLine(screen_middle2D, target_middle2D, gl::esp_Colors::AimLine, 1);
				// On right click
				if (GetAsyncKeyState(VK_RBUTTON))
					MyController->SetControlRotation(target_rotation);
			}
		}
	}
}