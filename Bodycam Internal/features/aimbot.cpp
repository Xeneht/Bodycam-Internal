#include "pch.h"
#include "aimbot.h"
#include "../ESP.h"
#include "../globals.h"
#include "../functions.h"
#include "bones.h"

void _target(SDK::USkeletalMeshComponent* mesh)
{
	auto cameraLocation = myController->PlayerCameraManager->GetCameraLocation();

	auto bonePos = getBonePos(mesh, hitboxesBones[gl::aimbot::hitbox]);
	auto rot = SDK::UKismetMathLibrary::FindLookAtRotation(cameraLocation, bonePos);

	SDK::FVector2D targetPos2D{};
	if (!myController->ProjectWorldLocationToScreen(bonePos, &targetPos2D, false)) return;

	SDK::FVector2D screenMiddle2D = { screenWidth / 2, screenHeight / 2 };
	aimbotDistance = SDK::UKismetMathLibrary::Distance2D(targetPos2D, screenMiddle2D);

	// Check inside fov and closest target
	if (aimbotDistance <= gl::aimbot::fov) {
		if (targetDistance > 0 && targetDistance < aimbotDistance) return;

		targetRotation = rot;
		targetDistance = aimbotDistance;
		target2D = { static_cast<float>(targetPos2D.X), static_cast<float>(targetPos2D.Y) };
		targetActor = mesh->GetOwner();
	}
}


void _aimbot()
{
	if (gl::aimbot::showFov)
		ESP::DrawCircle(Vec2(screenWidth / 2, screenHeight / 2), gl::aimbot::fov, gl::espColors::fovColor, 80);

	// Aimline
	if (gl::aimbot::aimLine && target2D.x > 1) {
		ESP::DrawLine({screenWidth / 2, screenHeight / 2 }, target2D, gl::espColors::aimLine, 1);
	}

	// On right click
	if ((GetKeyState(VK_RBUTTON) & 0x8000) != 0 && targetRotation.Pitch != 0 && targetRotation.Yaw != 0) {
		myController->SetControlRotation(targetRotation);
	}
}
