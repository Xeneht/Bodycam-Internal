#include "pch.h"
#include "cache.h"


void Cache::Cache()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	world = SDK::UWorld::GetWorld();
	if (world && world->PersistentLevel && world->PersistentLevel->Actors) {
		actors = world->PersistentLevel->Actors;
		myController = world->OwningGameInstance->LocalPlayers[0]->PlayerController;
	}

	while (alive)
	{
		world = SDK::UWorld::GetWorld();
		if (isBadPoint(world)) continue;
		if (isBadPoint(world->PersistentLevel)) continue;
		if (isBadPoint(&world->PersistentLevel->Actors)) continue;

		actors = world->PersistentLevel->Actors;

		// these vectors are used to store the actors temporarily
		std::vector<SDK::AActor*> tmpPlayers;
		std::vector<SDK::AActor*> tmpBots;
		std::vector<SDK::AActor*> tmpWorldActors;

		// reserve space to avoid reallocations
		tmpPlayers.reserve(10);
		tmpBots.reserve(10);
		tmpWorldActors.reserve(10);

		myController = world->OwningGameInstance->LocalPlayers[0]->PlayerController;
		if (isBadPoint(myController)) continue;

		for (int i = 0; i < actors.Num(); i++)
		{
			SDK::AActor* actor = actors[i];
			if (isBadPoint(actor) || isBadPoint(actor->RootComponent)) continue;

			// normal players
			if (actor->IsA(SDK::AALS_AnimMan_CharacterBP_C::StaticClass()))
			{
				tmpPlayers.push_back(actor);

				if (!actor->Instigator || isBadPoint(actor->Instigator)) continue;
				if (!actor->Instigator->PlayerState || isBadPoint(actor->Instigator->PlayerState)) continue;
				if (isBadPoint(myController->PlayerState)) continue;

				if (actor->Instigator->PlayerState == myController->PlayerState)
					localActor = actor;

				continue;
			}

			// bots
			if (actor->IsA(SDK::AAI_Humanoid_C::StaticClass()))
			{
				tmpBots.push_back(actor);
				continue;
			}

			// drones and bomb
			if (actor->IsA(SDK::ABP_Drone_C::StaticClass()) || actor->IsA(SDK::ABombe_C::StaticClass()))
			{
				tmpWorldActors.push_back(actor);
				continue;
			}

		}

		{
			// mutex to protect the render thread from accessing actors while updating
			std::lock_guard<std::mutex> lock(mtx);

			playerList = std::move(tmpPlayers);
			botsList = std::move(tmpBots);
			worldActors = std::move(tmpWorldActors);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}