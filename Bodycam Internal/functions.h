#pragma once


static const std::vector<std::pair<int, int>> BonePairs_type1 = {
    {0, 15}, {15, 14}, {14, 13}, {13, 12},
    {15, 16}, {16, 17}, {17, 18}, {18, 19},
    {15, 44}, {44, 45}, {45, 46}, {46, 47},
    {12, 75}, {75, 76}, {76, 79}, {12, 84},
    {84, 85}, {85, 88}
};

static const std::vector<std::pair<int, int>> BonePairs_type2 = {
    {0, 15}, {15, 14}, {14, 13}, {13, 12},
    {15, 16}, {16, 17}, {17, 18}, {18, 19},
    {15, 37}, {37, 38}, {38, 39}, {39, 40},
    {12, 60}, {60, 61}, {61, 63}, {12, 66},
    {66, 67}, {67, 69}
};

enum Bones : uint32_t {
    head = 62,
    hand_l = 8,
    hand_r = 36,
    spine_01 = 2,
    pelvis = 1,
    Root = 0,
    spine_02 = 3,
    spine_03 = 4,
    clavicle_l = 5,
    upperarm_l = 6,
    lowerarm_l = 7,
    index_01_l = 9,
    index_02_l = 10,
    index_03_l = 11,
    index_03_l_end = 12,
    middle_01_l = 13,
    middle_02_l = 14,
    middle_03_l = 15,
    middle_03_l_end = 16,
    pinky_01_l = 17,
    pinky_02_l = 18,
    pinky_03_l = 19,
    pinky_03_l_end = 20,
    ring_01_l = 21,
    ring_02_l = 22,
    ring_03_l = 23,
    ring_03_l_end = 24,
    thumb_01_l = 25,
    thumb_02_l = 26,
    thumb_03_l = 27,
    thumb_03_l_end = 28,
    lowerarm_twist_01_l = 29,
    lowerarm_twist_01_l_end = 30,
    upperarm_twist_01_l = 31,
    upperarm_twist_01_l_end = 32,
    clavicle_r = 33,
    upperarm_r = 34,
    lowerarm_r = 35,
    index_01_r = 37,
    index_02_r = 38,
    index_03_r = 39,
    index_03_r_end = 40,
    middle_01_r = 41,
    middle_02_r = 42,
    middle_03_r = 43,
    middle_03_r_end = 44,
    pinky_01_r = 45,
    pinky_02_r = 46,
    pinky_03_r = 47,
    pinky_03_r_end = 48,
    ring_01_r = 49,
    ring_02_r = 50,
    ring_03_r = 51,
    ring_03_r_end = 52,
    thumb_01_r = 53,
    thumb_02_r = 54,
    thumb_03_r = 55,
    thumb_03_r_end = 56,
    lowerarm_twist_01_r = 57,
    lowerarm_twist_01_r_end = 58,
    upperarm_twist_01_r = 59,
    upperarm_twist_01_r_end = 60,
    neck_01 = 61,
    head_end = 63,
    thigh_l = 64,
    calf_l = 65,
    calf_twist_01_l = 66,
    calf_twist_01_l_end = 67,
    foot_l = 68,
    ball_l = 69,
    ball_l_end = 70,
    thigh_twist_01_l = 71,
    thigh_twist_01_l_end = 72,
    thigh_r = 73,
    calf_r = 74,
    calf_twist_01_r = 75,
    calf_twist_01_r_end = 76,
    foot_r = 77,
    ball_r = 78,
    ball_r_end = 79,
    thigh_twist_01_r = 80,
    thigh_twist_01_r_end = 81,
    ik_foot_root = 82,
    ik_foot_l = 83,
    ik_foot_l_end = 84,
    ik_foot_r = 85,
    ik_foot_r_end = 86,
    ik_hand_root = 87,
    ik_hand_gun = 88,
    ik_hand_l = 89,
    ik_hand_l_end = 90,
    ik_hand_r = 91,
    ik_hand_r_end = 92,
};

template<typename T>
inline bool IsBadPoint(T* ptr)
{
    std::uintptr_t Pointer = reinterpret_cast<std::uintptr_t>(ptr);

    if ((Pointer < 0xFFFFFFFFFFULL) || (Pointer > 0x2FFFFFFFFFFULL))
        return true;
    else
        return false;
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

inline void DrawBones(SDK::USkeletalMeshComponent* mesh, SDK::APlayerController* controller, ImColor color)
{
    if (!mesh) return;
    if (IsBadPoint(mesh)) return;

    SDK::TArray<class SDK::FName> BoneSocketNames = mesh->GetAllSocketNames();

	if (BoneSocketNames.Num() == 0) return;
    SDK::FName HeadSocket = BoneSocketNames[0];
	SDK::FVector HeadLocation = mesh->GetSocketLocation(HeadSocket);
	SDK::FVector2D HeadLocation2D{};
	if (controller->ProjectWorldLocationToScreen(HeadLocation, &HeadLocation2D, false))
	{
		Vec2 HeadLocationVec2 = { HeadLocation2D.X, HeadLocation2D.Y };
        SDK::FVector myLocation = MyController->PlayerCameraManager->GetCameraLocation();
		float distance = GetDistance(myLocation, HeadLocation);
        auto r = 10000 / distance;
		ESP::DrawCircle(HeadLocationVec2, r, color);
	}

    std::vector<std::pair<int, int>> BonePairs = {};
    if (BoneSocketNames.Num() > 100)
		BonePairs = BonePairs_type1;
    else
        BonePairs = BonePairs_type2;

    for (const auto& bonePair : BonePairs)
    {
        int boneIndex1 = bonePair.first;
        int boneIndex2 = bonePair.second;

        if (boneIndex1 >= 0 && boneIndex2 >= 0 && boneIndex1 < BoneSocketNames.Num() && boneIndex2 < BoneSocketNames.Num())
        {
            SDK::FName boneName1 = mesh->GetSocketBoneName(BoneSocketNames[boneIndex1]);
            SDK::FName boneName2 = mesh->GetSocketBoneName(BoneSocketNames[boneIndex2]);

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
}

inline void DrawDistance(SDK::USkeletalMeshComponent* mesh, SDK::APlayerController* controller, ImColor color, SDK::FVector2D Bottom)
{
	if (!mesh) return;
    if (IsBadPoint(mesh)) return;

    SDK::TArray<class SDK::FName> BoneSocketNames = mesh->GetAllSocketNames();
    if (BoneSocketNames.Num() == 0) return;
    SDK::FName HeadSocket = BoneSocketNames[0];
	SDK::FVector HeadLocation = mesh->GetSocketLocation(HeadSocket);
	SDK::FVector2D HeadLocation2D{};
	if (controller->ProjectWorldLocationToScreen(HeadLocation, &HeadLocation2D, false))
	{
		Vec2 BottomVec = { Bottom.X, Bottom.Y };
		SDK::FVector myLocation = MyController->PlayerCameraManager->GetCameraLocation();
		float distance = GetDistance(myLocation, HeadLocation);
		auto r = 10000 / distance;
        int roundedDistance = static_cast<int>(distance / 100);
		ESP::DrawText2(BottomVec, gl::esp_Colors::DistanceColor, (std::to_string(roundedDistance) + "m").c_str());
	}
}