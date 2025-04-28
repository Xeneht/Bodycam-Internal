#include "pch.h"
#include "gameLoop.h"
#include <unordered_set>

std::unordered_set<SDK::AActor*> validActors;

void gameLoop()
{
	if (!updateActors()) return; // gets all actors in the world
	if (!updateLocalplayer()) return; // updates local player

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	targetRotation = {};
	targetDistance = 0.f;
	target2D = {};

	_worldActors(); // drones and bomb
	_players(); // players
	_bots(); // bots
	_local(); // local player
}

// we get all the actors to be able to check if the cached ones are still valid
bool updateActors()
{
	world = SDK::UWorld::GetWorld();
	if (isBadPoint(world)) return false;
	if (isBadPoint(world->PersistentLevel)) return false;

	auto& actors = world->PersistentLevel->Actors;
	if (actors.Num() <= 0) return false;

	validActors.clear();

	for (int i = 0; i < actors.Num(); i++)
	{
		auto* actor = actors[i];
		if (!isBadPoint(actor))
			validActors.insert(actor);
	}

	return true;
}

// validates the actor pointer
bool validActor(SDK::AActor* actor)
{
	return !isBadPoint(actor) && validActors.contains(actor);
}

// updates the local player
bool updateLocalplayer() {
	if (isBadPoint(myController)) return false;

	// get the local player character
	localCharacter = reinterpret_cast<SDK::AALS_AnimMan_CharacterBP_C*>(myController->Character);
	if (isBadPoint(localCharacter)) return false;
	double localHealth = 0.0;

	// get the local player survivor and check if is alive
	auto survivorStatus = localCharacter->WW_SurvivorStatus;
	if (survivorStatus)
		localHealth = survivorStatus->Health;
	if (localHealth <= 0) return false; // if dead we skip
	
	return true;
}

// drones and bomb
void _worldActors()
{
	if (worldActors.empty()) return;

	// we copy the world actors to avoid reading the vector while its being modified by the cache
	std::vector<SDK::AActor*> cWorldActors;
	{
		std::lock_guard<std::mutex> lock(mtx);
		cWorldActors = worldActors;
	}

	for (SDK::AActor* actor : cWorldActors)
	{
		if (!validActor(actor)) continue;
		if (isBadPoint(actor->RootComponent)) continue;

		// get the actor location and check if is valid
		SDK::FVector* locationPtr = &(actor->RootComponent->RelativeLocation);
		if (isBadPoint(locationPtr)) continue;
		SDK::FVector location = *locationPtr;
		SDK::FVector2D pos{};

		// draw the text esp
		if (myController->ProjectWorldLocationToScreen(location, &pos, false) && onScreen(pos)) {
			if (gl::world::drones && actor->IsA(SDK::ABP_Drone_C::StaticClass()))
			    ESP::DrawText2({ static_cast<float>(pos.X), static_cast<float>(pos.Y) }, gl::espColors::drones, "Drone");

			if (gl::world::bomb && actor->IsA(SDK::ABombe_C::StaticClass()))
			    ESP::DrawText2({ static_cast<float>(pos.X), static_cast<float>(pos.Y) }, gl::espColors::bomb, "Bomb");

			// draw any name
			//ESP::DrawText2({ static_cast<float>(pos.X), static_cast<float>(pos.Y) }, gl::espColors::white, actor->GetFullName().c_str());
		}
	}
}


void _players() {
	if (playerList.empty()) return;

	// same as world actors, copy to avoid crashes :)
	std::vector<SDK::AActor*> cPlayerList;
	{
		std::lock_guard<std::mutex> lock(mtx);
		cPlayerList = playerList;
	}

	for (SDK::AActor* actor : cPlayerList)
	{
		if (!validActor(actor)) continue;
		if (isBadPoint(actor->RootComponent)) continue;

		if (actor == localActor) continue; // skip local player

		// shitty checks
		if (isBadPoint(world)) continue;
		if (isBadPoint(world->OwningGameInstance)) continue;
		if (!world->OwningGameInstance->LocalPlayers) continue;
		if (isBadPoint(world->OwningGameInstance->LocalPlayers[0])) continue;
		if (isBadPoint(world->OwningGameInstance->LocalPlayers[0]->PlayerController)) continue;

		// actor character
		auto character = reinterpret_cast<SDK::AALS_AnimMan_CharacterBP_C*>(actor);
		if (!character) continue;


		////////////////////////////////--------------- ESP Checks -------------------////////////////////////////////////////

		// visibility check
		bool isActorVisible = false;
		if (!myController->PlayerCameraManager) continue;
		if (isBadPoint(myController->PlayerCameraManager)) continue;
		if (myController->PlayerCameraManager->CameraCachePrivate.POV.Location.IsZero()) continue;
		if (myController->LineOfSightTo(actor, myController->PlayerCameraManager->CameraCachePrivate.POV.Location, false))
			isActorVisible = true;

		// get the actor bounds and center
		SDK::FVector center{};
		SDK::FVector bounds{};
		SDK::FVector2D top{};
		SDK::FVector2D bottom{};

		actor->GetActorBounds(true, &center, &bounds, false);

		SDK::FVector top3D = center + SDK::FVector(0, 0, bounds.Z);
		SDK::FVector bottom3D = center - SDK::FVector(0, 0, bounds.Z);
		if (!myController->ProjectWorldLocationToScreen(top3D, &top, false)) continue;
		if (!myController->ProjectWorldLocationToScreen(bottom3D, &bottom, false)) continue;

		if (bounds.X > 100 || bounds.Y > 100) continue; // little fix to buggy entities
		if (!onScreen(top) && !onScreen(bottom)) continue; // avoid offscreen entities

		// team check
		bool differentTeam = true;
		if (gl::esp::teamCheck) {
			if (isBadPoint(&localCharacter->Team)) continue;
			int localPlayerTeam = static_cast<int>(localCharacter->Team);
			if (isBadPoint(&character->Team)) continue;
			int actorTeam = static_cast<int>(character->Team);
			if (localPlayerTeam == actorTeam)
				differentTeam = false;
		}

		// dead check and health
		bool isDead = false;
		character->IsDead(&isDead);
		double health = 0.0;
		if (character->WW_SurvivorStatus)
			health = character->WW_SurvivorStatus->Health;

		if (isDead) continue; // skip dead players

		if (!myController->Character->Mesh) continue;
		auto mesh = reinterpret_cast<SDK::USkeletalMeshComponent*>(character->Mesh);

		// Teleport enemies
		if (gl::exploits::teleportEnemies && differentTeam) {
			SDK::FVector localLocation = localActor->K2_GetActorLocation();
			SDK::FVector actorRotation = myController->GetActorForwardVector();
			SDK::FVector newLocation = localLocation + (actorRotation * 200.0f);
			actor->K2_SetActorRelativeLocation(newLocation, false, nullptr, true);
		}


		////////////////////////////////--------------- ESP Draw -------------------////////////////////////////////////////

		// Check if it's alive, esp is enabled and if (player is an enemy or team check is disabled)
		if (gl::esp::esp && (differentTeam || !gl::esp::teamCheck)) {

			// Chams
			if (gl::esp::playersChams)
				applyChams(mesh, playersChamsMat);

			// Not visible esp
			if (!isActorVisible)
			{
				if (gl::esp::espNotVisible) {
					ESP::Draw3DBox(center, bounds, myController, gl::espColors::notVisibleColor);
				}
				if (gl::esp::skeleton)
					drawBones(mesh, myController, gl::espColors::notVisibleColor);
			}

			// Visible esp
			else if (isActorVisible)
			{
				if (gl::esp::espVisible)
					ESP::Draw3DBox(center, bounds, myController, gl::espColors::visibleColor);
				if (gl::esp::skeleton)
					drawBones(mesh, myController, gl::espColors::visibleColor);
			}

			// Distance esp
			if (gl::esp::distance) {
				DrawDistance(mesh, myController, gl::espColors::distanceColor, bottom);
			}
			
			// Snaplines
			if (gl::esp::snapLines)
			{
				Vec2 snaplineOrigin2D = {};
				switch (selectedSnaplinePos)
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

				ESP::DrawLine(snaplineOrigin2D, Vec2(top.X, top.Y), gl::espColors::snaplineColor, 1);
			}
			// Health bar
			if (gl::esp::healthBar)
			{
				const float h = std::abs(top.Y - bottom.Y);
				const float w = h * 0.2f;
				ESP::DrawBoxFilled(Vec2(top.X - w - 6, top.Y - 1), Vec2(top.X - w - 3, bottom.Y + 1), colors::black);
				ESP::DrawBoxFilled(Vec2(top.X - w - 5, bottom.Y - (h * (health / 100.0f))), Vec2(top.X - w - 4, bottom.Y), colors::green);
			}
			// Nicknames
			if (gl::esp::nicknames)
			{
				if (!isBadPoint(actor->Instigator->PlayerState) && actor->Instigator->PlayerState->PlayerNamePrivate) {
					SDK::FString playername = actor->Instigator->PlayerState->PlayerNamePrivate;
					float estimatedTextHeight = 20.0f;
					float marginLeft = 15.0f;
					if (playername.ToString().length() > 0) {
						ESP::DrawText2({ static_cast<float>(top.X - marginLeft), static_cast<float>(top.Y - estimatedTextHeight) }, colors::white, playername.ToString().c_str());
					}
				}
			}
		}

		// Aimbot targeting

		if (gl::aimbot::aimbot) {
			if (!differentTeam) continue; // skip teammates
			if (!isActorVisible) continue; // skip not visible players

			_target(mesh);
		}
	}
}

// bots
// wont comment this one because its a copy paste of the players function
void _bots() {
	if (botsList.empty()) return;

	std::vector<SDK::AActor*> cBotsList;
	{
		std::lock_guard<std::mutex> lock(mtx);
		cBotsList = botsList;
	}

	world = SDK::UWorld::GetWorld();
	if (isBadPoint(world)) return;

	for (int i = 0; i < cBotsList.size(); i++)
	{
		SDK::AActor* actor = cBotsList[i];
		if (!validActor(actor)) continue;

		if (isBadPoint(actor->RootComponent)) continue;
		if (isBadPoint(localActor)) continue;

		auto character = reinterpret_cast<SDK::AAI_Humanoid_C*>(actor);
		if (isBadPoint(character)) continue;

		// Visibility check
		bool isActorVisible = false;
		if (!myController->PlayerCameraManager) continue;
		if (isBadPoint(myController->PlayerCameraManager)) continue;
		if (myController->PlayerCameraManager->CameraCachePrivate.POV.Location.IsZero()) continue;
		if (myController->LineOfSightTo(actor, myController->PlayerCameraManager->CameraCachePrivate.POV.Location, false))
			isActorVisible = true;

		bool isDead = false;

		double health = 0.0;
		if (character->WW_SurvivorStatus)
			health = character->WW_SurvivorStatus->Health;

		character->IsDead(&isDead);
		if (health <= 0) isDead = true;
		if (isDead) continue;

		auto mesh = reinterpret_cast<SDK::USkeletalMeshComponent*>(character->Mesh);

		SDK::FVector center{};
		SDK::FVector bounds{};
		SDK::FVector2D top{};
		SDK::FVector2D bottom{};

		actor->GetActorBounds(true, &center, &bounds, false);

		SDK::FVector top3D = center + SDK::FVector(0, 0, bounds.Z);
		SDK::FVector bottom3D = center - SDK::FVector(0, 0, bounds.Z);
		if (!myController->ProjectWorldLocationToScreen(top3D, &top, false)) continue;
		if (!myController->ProjectWorldLocationToScreen(bottom3D, &bottom, false)) continue;

		if (bounds.X > 100 || bounds.Y > 100) continue;
		if (!onScreen(top) && !onScreen(bottom)) continue;

		// Team check
		bool differentTeam = true;
		if (gl::esp::teamCheck) {
			if (isBadPoint(&localCharacter->Team)) continue;
			int localPlayerTeam = static_cast<int>(localCharacter->Team);
			if (isBadPoint(&character->Team)) continue;
			int actorTeam = static_cast<int>(character->Team);
			if (localPlayerTeam == actorTeam)
				differentTeam = false;
		}

		// Teleport enemies
		if (gl::exploits::teleportEnemies && differentTeam) {
			SDK::FVector localLocation = localActor->K2_GetActorLocation();
			SDK::FVector actorRotation = myController->GetActorForwardVector();
			SDK::FVector newLocation = localLocation + (actorRotation * 200.0f);
			actor->K2_SetActorRelativeLocation(newLocation, false, nullptr, true);
		}

		SDK::FVector* locationPtr = &(actor->RootComponent->RelativeLocation);
		if (isBadPoint(locationPtr)) continue;
		SDK::FVector location = *locationPtr;



		////////////////////////////////--------------- ESP Draw -------------------////////////////////////////////////////

		if (gl::esp::playersChams)
			applyChams(mesh, playersChamsMat);


		// Check if it's alive, esp is enabled and if (player is an enemy or team check is disabled)
		if (gl::esp::esp && (differentTeam || !gl::esp::teamCheck)) {
			// Chams

			// Not visible esp
			if (!isActorVisible)
			{
				if (gl::esp::espNotVisible) {
					ESP::Draw3DBox(center, bounds, myController, gl::espColors::notVisibleColor);
				}
				if (gl::esp::skeleton)
					drawBones(mesh, myController, gl::espColors::notVisibleColor);
			}
			// Visible esp
			else if (isActorVisible)
			{
				if (gl::esp::espVisible)
					ESP::Draw3DBox(center, bounds, myController, gl::espColors::visibleColor);
				if (gl::esp::skeleton)
					drawBones(mesh, myController, gl::espColors::visibleColor);
			}
			if (gl::esp::distance) {
				DrawDistance(mesh, myController, gl::espColors::distanceColor, bottom);
			}
			if (gl::esp::snapLines)
			{
				// Snaplines
				Vec2 snaplineOrigin2D = {};
				switch (selectedSnaplinePos)
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

				ESP::DrawLine(snaplineOrigin2D, Vec2(top.X, top.Y), gl::espColors::snaplineColor, 1);
			}
			// Health bar
			if (gl::esp::healthBar)
			{
				const float h = std::abs(top.Y - bottom.Y);
				const float w = h * 0.2f;
				ESP::DrawBoxFilled(Vec2(top.X - w - 6, top.Y - 1), Vec2(top.X - w - 3, bottom.Y + 1), colors::black);
				ESP::DrawBoxFilled(Vec2(top.X - w - 5, bottom.Y - (h * (health / 100.0f))), Vec2(top.X - w - 4, bottom.Y), colors::green);
			}
			// Nicknames
			if (gl::esp::nicknames)
			{
				if (actor->Instigator->PlayerState && !isBadPoint(actor->Instigator->PlayerState) && actor->Instigator->PlayerState->PlayerNamePrivate) {
					SDK::FString playername = actor->Instigator->PlayerState->PlayerNamePrivate;
					float estimatedTextHeight = 20.0f;
					float marginLeft = 15.0f;
					if (playername.ToString().length() > 0) {
						ESP::DrawText2({ static_cast<float>(top.X - marginLeft), static_cast<float>(top.Y - estimatedTextHeight) }, colors::white, playername.ToString().c_str());
					}
				}
			}
		}


		if (gl::aimbot::aimbot) {
			if (!differentTeam) continue; // skip teammates
			if (!isActorVisible) continue; // skip not visible players

			_target(mesh);
		}
	}
}


// local player
void _local() {
	if (isBadPoint(localActor)) return;
	if (isBadPoint(localCharacter)) return;
	if (isBadPoint(myController)) return;
	if (isBadPoint(localCharacter->WEP)) return;
	if (isBadPoint(localCharacter->WW_SurvivorStatus)) return;

	SDK::AWEP_C* pWeapon = localCharacter->WEP;
	SDK::UWW_SurvivorStatus_C* survivorStatus = localCharacter->WW_SurvivorStatus;

	// Local chams
	if (gl::esp::localChams && !isBadPoint(localCharacter->Mesh)) {
		auto mesh = localCharacter->Mesh;
		if (mesh) applyChams(mesh, localChamsMat);
	}

	if (isBadPoint(pWeapon)) return; // skip if no weapon
	
	// No recoil
	if (gl::exploits::noRecoil) {
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
	if (gl::exploits::unlimitedAmmo && !isBadPoint(pWeapon)) {
		pWeapon->BulletInChamber = 1;
	}
	// Rapid fire
	if (gl::exploits::rapidFire) {
		pWeapon->Wep_Info.RecoilInfo_14_2F550DD548C99F85FB2D1C911E29083C.FireRate_11_A93DD4B84C6696E6E0EC3BA044512B73 = static_cast<double>(gl::exploits::rapidFireValue);
	}
	// Full auto
	if (gl::exploits::fullAuto) {
		pWeapon->Wep_Info.FireMode_39_E7AEF4D24EEC5420D75555A92E651905 = SDK::E_FireMode::NewEnumerator1;
	}
	// Instant kill
	if (gl::aimbot::instantKill) {
		pWeapon->Wep_Info.Damage_17_7F60A88D435DBD7B36C96893C2118EAC = 999;
	}
	// Insta aim
	if (gl::aimbot::instantAiming) {
		pWeapon->Wep_Info.Animation_23_574905EC43F77C89F6A965AFE119106B.ADSInTIme_39_E74D36F9478596C143C5C0A4B3A694F9 = 0.03;
		pWeapon->Wep_Info.Animation_23_574905EC43F77C89F6A965AFE119106B.ADSOutTime_41_63DF5BF04A00BD2A761C11861660F122 = 0.03;
	}
	// No Aiming restrictions
	if (gl::aimbot::noAimingRestrictions) {
		localCharacter->AimingBloked = false;
	}

	// Weapon chams
	if (gl::esp::weaponChams && !isBadPoint(pWeapon) && !isBadPoint(pWeapon->Wep_Body)) {
		auto mesh = pWeapon->Wep_Body;
		if (!isBadPoint(mesh))
			applyChams(mesh, weaponChamsMat);
	}

	// No flash
	if (gl::esp::noFlash) {
		survivorStatus->FlashBangEffect = 0;

		if (!isBadPoint(localCharacter->FlashBangEar))
			localCharacter->FlashBangEar->SetVolumeMultiplier(0);
	}

	// God mode
	if (gl::exploits::godMode) {
		auto survivorStatus = localCharacter->WW_SurvivorStatus;
		survivorStatus->Health = 999;
	}

	// Fov
	if (gl::exploits::fov && localCharacter->Camera) {
		auto camera = reinterpret_cast<SDK::UCameraComponent*>(localCharacter->Camera);
		if (!isBadPoint(camera)) {
			camera->FieldOfView = gl::exploits::fovValue;
			camera->PostProcessBlendWeight = 0.0f;
		}
	}

	// Gravity
	if (gl::exploits::gravity)
		if (localCharacter->CharacterMovement)
			localCharacter->CharacterMovement->GravityScale = gl::exploits::gravityValue;


	// Aimbot
	if (gl::aimbot::aimbot) {
		pWeapon->Spread_Multiplier = 0; // fixes aiming height issue
		_aimbot();
	}
}
