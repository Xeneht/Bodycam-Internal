#pragma once
#include <algorithm>
#include <sstream>
#include <functional>
#include "sdk.hpp"
#include "ESP.h"
#include "globals.h"

template<typename T>
inline bool isBadPoint(T* ptr)
{
	if (!ptr) return true;
    std::uintptr_t Pointer = reinterpret_cast<std::uintptr_t>(ptr);

    if ((Pointer < 0xFFFFFFFFFFULL) || (Pointer > 0x2FFFFFFFFFFULL))
        return true;
    else
        return false;
}

inline bool onScreen(const SDK::FVector2D& pos)
{
    int margin = 250;
    return pos.X >= -margin && pos.X <= (screenWidth + margin)
        && pos.Y >= -margin && pos.Y <= (screenHeight + margin);
}


inline bool IsValidMesh(SDK::USkeletalMeshComponent* mesh)
{
    if (!mesh) return false;

    uintptr_t address = reinterpret_cast<uintptr_t>(mesh);
    if (address < 0x10000) return false;

    uintptr_t vtable = *reinterpret_cast<uintptr_t*>(address);
    if (vtable < 0x10000 || vtable > 0x7FFFFFFFFFFF) return false;

    return true;
}


inline SDK::FName StrToName(std::string str)
{
    std::wstring wstr(str.begin(), str.end());
    const wchar_t* wstr_cstr = wstr.c_str();

    return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(TEXT(wstr_cstr)));
}

inline float GetDistance(SDK::FVector firstLocation, SDK::FVector secondLocation)
{
    if (firstLocation.IsZero() || secondLocation.IsZero()) return 0.0f;

    float distance = sqrt(
        pow(secondLocation.X - firstLocation.X, 2) +
        pow(secondLocation.Y - firstLocation.Y, 2) +
        pow(secondLocation.Z - firstLocation.Z, 2)
    );

    return distance;
}


inline void DrawDistance(SDK::USkeletalMeshComponent* mesh, SDK::APlayerController* controller, ImColor color, SDK::FVector2D Bottom)
{
	if (!mesh) return;
    if (isBadPoint(mesh)) return;

    SDK::TArray<class SDK::FName> BoneSocketNames = mesh->GetAllSocketNames();
    if (BoneSocketNames.Num() == 0) return;
    SDK::FName HeadSocket = BoneSocketNames[0];
	SDK::FVector HeadLocation = mesh->GetSocketLocation(HeadSocket);
	SDK::FVector2D HeadLocation2D{};
	if (controller->ProjectWorldLocationToScreen(HeadLocation, &HeadLocation2D, false))
	{
		Vec2 BottomVec = { Bottom.X, Bottom.Y };
		SDK::FVector myLocation = myController->PlayerCameraManager->GetCameraLocation();
		float distance = GetDistance(myLocation, HeadLocation);
		auto r = 10000 / distance;
        int roundedDistance = static_cast<int>(distance / 100);
		ESP::DrawText2(BottomVec, gl::espColors::distanceColor, (std::to_string(roundedDistance) + "m").c_str());
	}
}


inline SDK::FName StrToName(const wchar_t* str)
{
    return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(TEXT(str)));
}
inline SDK::FLinearColor FloatToColor(float color[3])
{
    return SDK::FLinearColor{ color[0], color[1], color[2] };
}


inline static bool materials_created = false;

inline void applyChams(SDK::USkeletalMeshComponent* mesh, SDK::UMaterialInstanceDynamic* dynMaterial)
{
    if (!mesh || !dynMaterial) return;

    auto Mats = mesh->GetMaterials();
    if (Mats.Num() == 0) return;

    mesh->SetRenderCustomDepth(true);

    for (int i = 0; i < Mats.Num(); i++)
    {
        if (!Mats[i]) continue;
        mesh->SetMaterial(i, dynMaterial);
    }
}


// refreshes the materials from the dev tab
inline void refreshMaterials()
{
    std::thread([]()
        {
            std::vector<std::string> tempMaterials;

            for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
            {
                SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);

                if (!Obj || Obj->IsDefaultObject())
                    continue;

                if (Obj->IsA(SDK::UMaterialInstanceConstant::StaticClass()))
                {
                    auto* MaterialInstance = (SDK::UMaterialInstanceConstant*)Obj;

                    if (MaterialInstance->ScalarParameterValues.Num() > 0 || MaterialInstance->VectorParameterValues.Num() > 0)
                    {
                        tempMaterials.push_back(Obj->GetFullName());
                    }
                }
            }

            {
                std::lock_guard<std::mutex> lock(materialsMtx);
                availableMaterials = std::move(tempMaterials);
            }

            //std::cout << "[INFO] Refresh finished. Total: " << availableMaterials.size() << "\n";

        }).detach();
}

enum class ChamsType
{
    Players,
    Local,
    Weapon
};

inline void createChamsMaterials(const std::string& MatName, ChamsType type)
{
    std::thread([MatName, type]()
        {
            // we use the provided material name to use it as the chams
            SDK::UMaterial* baseMaterial = SDK::UObject::FindObject<SDK::UMaterial>(MatName.c_str());
            if (!baseMaterial) return;

            // shitty flags idk
            baseMaterial->bDisableDepthTest = true;
            baseMaterial->Wireframe = true;
            baseMaterial->BlendMode = SDK::EBlendMode::BLEND_Additive;
            baseMaterial->MaterialDomain = SDK::EMaterialDomain::MD_Surface;
            baseMaterial->AllowTranslucentCustomDepthWrites = true;
            baseMaterial->bIsBlendable = true;
            baseMaterial->LightmassSettings.EmissiveBoost = 2;
            baseMaterial->LightmassSettings.DiffuseBoost = 0;

			// create the dynamic material instance
            SDK::UMaterialInstanceDynamic* dynMaterial = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance(world, baseMaterial, StrToName(L"ChamsMaterial"), SDK::EMIDCreationFlags::None);
            if (!dynMaterial) return;

            switch (type)
            {
            case ChamsType::Players:
                playersChamsMat = dynMaterial;
                break;
            case ChamsType::Local:
                localChamsMat = dynMaterial;
                break;
            case ChamsType::Weapon:
                weaponChamsMat = dynMaterial;
                break;
            }
        }).detach();
}


inline void updateChams(ChamsType type)
{
    switch (type)
    {
    case ChamsType::Players:
        createChamsMaterials(std::string(chamsNameList[selectedPlayerChams]), ChamsType::Players);
        break;
    case ChamsType::Local:
        createChamsMaterials(std::string(chamsNameList[selectedLocalChams]), ChamsType::Local);
        break;
    case ChamsType::Weapon:
        createChamsMaterials(std::string(chamsNameList[selectedWeaponChams]), ChamsType::Weapon);
        break;
    }
}
