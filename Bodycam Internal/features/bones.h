#pragma once
#include <unordered_map>
#include <string>


static const std::vector<std::pair<std::string, std::string>> BonePairsNames = {
	{ "pelvis", "spine_03"},
	{ "spine_03","head"},

	{ "spine_03", "clavicle_l" },
	{ "clavicle_l", "upperarm_l" },
	{ "upperarm_l", "lowerarm_l" },
	{ "lowerarm_l", "hand_l" },

	{ "spine_03", "clavicle_r" },
	{ "clavicle_r", "upperarm_r" },
	{ "upperarm_r", "lowerarm_r" },
	{ "lowerarm_r", "hand_r" },

	{ "pelvis", "thigh_l"},
	{ "thigh_l", "calf_l" },
	{ "calf_l", "foot_l" },

	{ "pelvis", "thigh_r"},
	{ "thigh_r", "calf_r" },
	{ "calf_r", "foot_r" }
};


void drawBones(SDK::USkeletalMeshComponent* mesh, SDK::APlayerController* controller, ImColor color);
SDK::FVector getBonePos(SDK::USkeletalMeshComponent* mesh, std::string boneName);
SDK::FVector getHeadPos(SDK::USkeletalMeshComponent* mesh);