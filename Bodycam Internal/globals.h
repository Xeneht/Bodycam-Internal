#pragma once
#include "includes.h"
#include "SDK.hpp"

inline bool showmenu = true;
inline bool alive = true;
inline ImDrawList* draw_list{};
inline std::vector<SDK::AActor*> PlayerList{};
inline std::vector<SDK::AActor*> ActorList{};
inline SDK::UEngine* Engine{};
inline SDK::UWorld* World{};
inline SDK::TArray<SDK::AActor*> actors{};
inline SDK::AActor* actor_list{};
inline SDK::APlayerController* MyController{};
inline SDK::AActor* LocalActor{};
inline float aimbot_distance{};
inline std::vector<SDK::AActor*> WorldActors{};

inline const char* snapline_positions[] = { "Top", "Middle", "Bottom" };
inline int selected_position = 0;

namespace Colors
{
	inline ImColor White(255.f, 255.f, 255.f);
	inline ImColor Black(0.f, 0.f, 0.f);
	inline ImColor Red(255.f, 0.f, 0.f);
	inline ImColor DarkRed(0.700f, 0.f, 0.f);
	inline ImColor Green(0.f, 255.f, 0.f);
	inline ImColor DarkGreen(0.f, 0.600f, 0.f);
	inline ImColor Blue(0.f, 0.f, 255.f);
	inline ImColor DarkBlue(0.f, 0.f, 0.700f);
	inline ImColor Pink(255.f, 0.f, 255.f);
	inline ImColor Cian(0.f, 255.f, 255.f);
	inline ImColor Yellow(255.f, 255.f, 0.f);
	inline ImColor Grey(0.600f, 0.600f, 0.600f);
	inline ImColor Purple(0.200f, 0.f, 0.600f);
}

namespace gl
{
	namespace ESP
	{
		inline bool ESP = true;
		inline bool ESP_Visible = true;
		inline bool ESP_NotVisible = true;
		inline bool SnapLines = false;
		inline bool Nicknames = false;
		inline bool HealthBar = false;
		inline bool TeamCheck = false;

		inline bool NoFlash = false;
	}

	namespace World
	{
		inline bool Drones = false;
		inline bool Bomb = false;
	}
	
	namespace Aimbot
	{
		inline bool Aimbot = false;
		inline bool ShowFov = false;
		inline float Fov = 0.f;
		inline float Pitch = 0.f;
		inline bool AimLine = false;

		inline bool InstantKill = false;
		inline bool InstantAiming = false;
		inline bool NoAimingRestrictions = false;
	}
	
	namespace Exploits
	{
		inline bool Gravity = false;
		inline bool Fov = false;
		inline float GravityValue = 1.f;
		inline float FovValue = 120.f;

		inline bool NoRecoil = false;
		inline bool RapidFire = false;
		inline bool FullAuto = false;
		inline bool UnlimitedAmmo = false;
		inline bool GodMode = false;


		inline bool DisarmEnemies = false;
		inline bool StealEnemyBullets = false;

		inline float RapidFireValue = 0.11f;
		inline int xp = 0;
		inline int killsQuantity = 0;
		inline bool xpApply = false;
		inline bool addKills = false;
	}

	namespace HostOptions
	{
		inline bool Gravity = false;
		inline float GravityValue = 1.f;

		inline bool NoRecoil = false;
		inline bool RapidFire = false;
		inline bool UnlimitedAmmo = false;
		inline bool FullAuto = false;

		inline bool NoDamageBullets = false;
		inline bool RemoveBullets = false;

		inline bool CrazyHeads = false;
		inline bool KillPlayers = false;
		inline bool KillPlayersSilent = false;

		inline float RapidFireValue = 0.11f;
		inline bool finishGame = false;
	}

	namespace Misc
	{
		inline bool ShowMouse = true;
		inline bool Suicide = false;
		inline bool HideSteamId = false;
	}

	namespace esp_Colors
	{
		inline ImColor Box3DColor{ 255.f, 0.f , 0.f };
		inline ImColor SnaplineColor{ 0.f, 255.f, 255.f };
		inline ImColor SkeletonColor{ 255.f, 0.f, 0.f };
		inline ImColor FovColor{ 255.f, 255.f, 255.f };
		inline ImColor CrosshairColor{ 255.f, 0.f, 0.f };
		inline ImColor VisibleColor{ 255.f, 0.f, 0.f };
		inline ImColor NotVisibleColor{ 0.f, 255.f, 255.f };

		inline ImColor Drones = Colors::Yellow;
		inline ImColor Bomb = Colors::Red;

		inline ImColor AimLine = Colors::Red;
		inline ImColor Nickname = Colors::White;

		inline ImColor White = Colors::White;

		inline float visible[3] = { 1.0f, 0.0f, 0.0f };
		inline float notVisible[3] = { 0.0f, 0.0f, 0.0f };
	}
}