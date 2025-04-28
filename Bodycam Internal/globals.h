#pragma once
#include "SDK.hpp"

inline bool showMenu = true;
inline bool alive = true;
inline ImDrawList* drawList{};
inline SDK::UWorld* world{};
inline SDK::TArray<SDK::AActor*> actors{};
inline SDK::APlayerController* myController{};
inline SDK::AALS_AnimMan_CharacterBP_C* localCharacter{};
inline SDK::AActor* localActor{};
inline float aimbotDistance{};

// aimbot
inline SDK::FRotator targetRotation{};
inline SDK::AActor* targetActor{};
inline Vec2 target2D{};
inline float targetDistance{};

inline float screenWidth = GetSystemMetrics(SM_CXSCREEN);
inline float screenHeight = GetSystemMetrics(SM_CYSCREEN);

inline std::vector<SDK::AActor*> playerList{};
inline std::vector<SDK::AActor*> botsList{};
inline std::vector<SDK::AActor*> worldActors{};

inline static std::vector<std::string> availableMaterials;
inline static int selectedMaterialIndex = -1;

inline SDK::UMaterialInstanceDynamic* chamsMat{};
inline SDK::UMaterialInstanceDynamic* playersChamsMat{};
inline SDK::UMaterialInstanceDynamic* localChamsMat{};
inline SDK::UMaterialInstanceDynamic* weaponChamsMat{};

inline std::mutex mtx;
inline std::mutex materialsMtx;

inline const char* snaplinePositions[] = { "Top", "Middle", "Bottom" };
inline int selectedSnaplinePos = 0;

inline const char* chamsTypes[] = { "White", "Ironman", "Black Glass", "Red Glass", "Transparent"};
inline int selectedPlayerChams = 0;
inline int selectedLocalChams = 0;
inline int selectedWeaponChams = 0;
inline const char* chamsNameList[] ={
    "MaterialInstanceConstant Mi_PlasticWhite.Mi_PlasticWhite",
    "MaterialInstanceConstant M_Ammo_12gauge.M_Ammo_12gauge",
    "MaterialInstanceConstant Mi_Helmet_Glasses_01.Mi_Helmet_Glasses_01",
    "MaterialInstanceConstant MI_GlassTumblers01.MI_GlassTumblers01"
    "MaterialInstanceConstant MI_Glass_C.MI_Glass_C",
};


inline const char* hitboxes[] = { "Head", "Chest" };
inline const char* hitboxesBones[] = { "head", "spine_03" };

namespace colors
{
    inline ImColor white(255.f, 255.f, 255.f);
    inline ImColor black(0.f, 0.f, 0.f);
    inline ImColor red(255.f, 0.f, 0.f);
    inline ImColor darkRed(0.700f, 0.f, 0.f);
    inline ImColor green(0.f, 255.f, 0.f);
    inline ImColor darkGreen(0.f, 0.600f, 0.f);
    inline ImColor blue(0.f, 0.f, 255.f);
    inline ImColor darkBlue(0.f, 0.f, 0.700f);
    inline ImColor pink(255.f, 0.f, 255.f);
    inline ImColor cyan(0.f, 255.f, 255.f);
    inline ImColor yellow(255.f, 255.f, 0.f);
    inline ImColor grey(0.600f, 0.600f, 0.600f);
    inline ImColor purple(0.200f, 0.f, 0.600f);
}

namespace gl
{
    namespace esp
    {
        inline bool esp = true;
        inline bool espVisible = true;
        inline bool espNotVisible = true;
        inline bool snapLines = false;

        inline bool playersChams = false;
        inline bool localChams = false;
        inline bool weaponChams = false;

        inline bool nicknames = false;
        inline bool healthBar = false;
        inline bool skeleton = false;
        inline bool distance = false;
        inline bool teamCheck = false;

        inline bool noFlash = false;
    }

    namespace world
    {
        inline bool drones = false;
        inline bool bomb = false;
    }

    namespace aimbot
    {
        inline bool aimbot = false;
        inline bool showFov = false;
        inline float fov = 0.f;
        inline bool aimLine = false;
        inline int hitbox = 0;

        inline bool instantKill = false;
        inline bool instantAiming = false;
        inline bool noAimingRestrictions = false;
    }

    namespace exploits
    {
        inline bool gravity = false;
        inline bool fov = false;
        inline float gravityValue = 1.f;
        inline float fovValue = 120.f;

        inline bool noRecoil = false;
        inline bool rapidFire = false;
        inline bool fullAuto = false;
        inline bool unlimitedAmmo = false;
        inline bool godMode = false;

        inline float rapidFireValue = 0.11f;
        inline int xp = 0;
        inline int killsQuantity = 0;
        inline bool xpApply = false;
        inline bool addKills = false;

        inline bool teleportEnemies = false;

        inline bool firstFrozenKills = false;
        inline bool frozenKills = false;
        inline int kills = 0;

        inline int zombieTime = 0;
        inline bool addZombieTime = false;
    }

    namespace hostOptions
    {
        inline bool gravity = false;
        inline float gravityValue = 1.f;

        inline bool noRecoil = false;
        inline bool rapidFire = false;
        inline bool unlimitedAmmo = false;
        inline bool fullAuto = false;

        inline bool noDamageBullets = false;
        inline bool removeBullets = false;

        inline bool crazyHeads = false;
        inline bool killPlayers = false;
        inline bool killPlayersSilent = false;

        inline float rapidFireValue = 0.11f;
        inline bool finishGame = false;
    }

    namespace misc
    {
        inline bool showMouse = true;
        inline bool suicide = false;
        inline bool safeMode = true;
        inline bool hideSteamId = false;
    }

    namespace espColors
    {
        inline ImColor box3DColor{ 255.f, 0.f , 0.f };
        inline ImColor snaplineColor{ 0.f, 255.f, 255.f };
        inline ImColor distanceColor{ 255.f, 255.f, 255.f };
        inline ImColor fovColor{ 255.f, 255.f, 255.f };
        inline ImColor crosshairColor{ 255.f, 0.f, 0.f };
        inline ImColor visibleColor{ 255.f, 0.f, 0.f };
        inline ImColor notVisibleColor{ 0.f, 255.f, 255.f };

        inline ImColor drones = colors::yellow;
        inline ImColor bomb = colors::red;

        inline ImColor aimLine = colors::red;
        inline ImColor nickname{ 255.f, 255.f, 255.f };

        inline ImColor white = colors::white;

        inline float visibleArr[3] = { 0.0f, 255.0f, 0.0f };
        inline float notVisibleArr[3] = { 255.0f, 0.0f, 0.0f };
    }
}
