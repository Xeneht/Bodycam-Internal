#include "pch.h"
#include "../ESP.h"
#include "../functions.h"
#include "bones.h"


void drawBones(SDK::USkeletalMeshComponent* mesh, SDK::APlayerController* controller, ImColor color)
{
    if (!IsValidMesh(mesh)) return;

    SDK::TArray<class SDK::FName> boneSocketNames = mesh->GetAllSocketNames();
    if (boneSocketNames.Num() == 0) return;

    std::unordered_map<std::string, int> nameToIndex;
    for (int i = 0; i < boneSocketNames.Num(); ++i) {
        nameToIndex[boneSocketNames[i].ToString()] = i;
    }

    auto it = nameToIndex.find("head");
    if (it == nameToIndex.end()) return;

    SDK::FName HeadSocket = boneSocketNames[it->second];
    SDK::FVector HeadLocation = mesh->GetSocketLocation(HeadSocket);
    SDK::FVector2D HeadLocation2D{};
    if (controller->ProjectWorldLocationToScreen(HeadLocation, &HeadLocation2D, false))
    {
        Vec2 HeadLocationVec2 = { HeadLocation2D.X, HeadLocation2D.Y };
        SDK::FVector myLocation = myController->PlayerCameraManager->GetCameraLocation();
        float distance = GetDistance(myLocation, HeadLocation);
        float r = 10000.f / distance;
        ESP::DrawCircle(HeadLocationVec2, r, color);
    }

    for (const auto& bonePair : BonePairsNames)
    {
        auto it1 = nameToIndex.find(bonePair.first);
        auto it2 = nameToIndex.find(bonePair.second);

        if (it1 == nameToIndex.end() || it2 == nameToIndex.end()) continue;

        int boneIndex1 = it1->second;
        int boneIndex2 = it2->second;

        if (boneIndex1 < 0 || boneIndex2 < 0 || boneIndex1 >= boneSocketNames.Num() || boneIndex2 >= boneSocketNames.Num()) continue;

        SDK::FName boneName1 = boneSocketNames[boneIndex1];
        SDK::FName boneName2 = boneSocketNames[boneIndex2];

        SDK::FVector bone_location1 = mesh->GetSocketLocation(boneName1);
        SDK::FVector bone_location2 = mesh->GetSocketLocation(boneName2);

        SDK::FVector2D bone1_w2s_2D{};
        SDK::FVector2D bone2_w2s_2D{};

        if (controller->ProjectWorldLocationToScreen(bone_location1, &bone1_w2s_2D, false) &&
            controller->ProjectWorldLocationToScreen(bone_location2, &bone2_w2s_2D, false))
        {
            Vec2 bone1_w2s = { bone1_w2s_2D.X, bone1_w2s_2D.Y };
            Vec2 bone2_w2s = { bone2_w2s_2D.X, bone2_w2s_2D.Y };

            ESP::DrawLine(bone1_w2s, bone2_w2s, color);
        }
    }
}

SDK::FVector getBonePos(SDK::USkeletalMeshComponent* mesh, std::string boneName)
{
    SDK::FVector boneLocation = SDK::FVector(0, 0, 0);
    if (!IsValidMesh(mesh)) return boneLocation;

    SDK::TArray<class SDK::FName> boneSocketNames = mesh->GetAllSocketNames();
    if (boneSocketNames.Num() == 0) return boneLocation;

    for (int i = 0; i < boneSocketNames.Num(); ++i)
    {
        if (boneSocketNames[i].ToString() == boneName)
        {
            boneLocation = mesh->GetSocketLocation(boneSocketNames[i]);
            break;
        }
    }

    return boneLocation;
}

SDK::FVector getHeadPos(SDK::USkeletalMeshComponent* mesh)
{
    return getBonePos(mesh, "head");
}