#include "ActorCache.h"
#include "SDK.hpp"
#include "functions.h"

void ActorsCache::ActorsCache()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	World = SDK::UWorld::GetWorld();

	actors = World->PersistentLevel->Actors;
	MyController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;

	while (alive)
	{
		WorldActors.clear();
		PlayerList.clear();

		World = SDK::UWorld::GetWorld();
		if (!World) continue;
		if (IsBadPoint(World)) continue;
		if (!World->PersistentLevel) continue;
		if (IsBadPoint(World->PersistentLevel)) continue;
		if (!World->PersistentLevel->Actors) continue;
		if (IsBadPoint(&World->PersistentLevel->Actors)) continue;
		actors = World->PersistentLevel->Actors;
		for (int i = 0; i < actors.Num(); i++)
		{
			if (i >= actors.Num()) break;
			if (!actors[i]) continue;

			SDK::AActor* actor = actors[i];
			if (!actor || actor == nullptr) continue;
			if (!actor->RootComponent) continue;
			MyController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
			auto name = actor->GetName();
			if (name.find("ALS_AnimMan_Character") != std::string::npos) {
				
				PlayerList.push_back(actor);

				if (!actor->Instigator) continue;
				if (IsBadPoint(actor->Instigator)) continue;
				if (!actor->Instigator->PlayerState) continue;
				if (IsBadPoint(actor->Instigator->PlayerState)) continue;
				if (IsBadPoint(MyController->PlayerState)) continue;
				if (actor->Instigator->PlayerState == MyController->PlayerState)
					LocalActor = actor;

				continue;
			}
			if (name.find("BP_Drone") != std::string::npos) {
				WorldActors.push_back(actor);
				continue;
			}
			
			if (name.find("Bombe") != std::string::npos) {
				WorldActors.push_back(actor);
				continue;
			}

			//if (name.find("None") != std::string::npos || name.find("Static") != std::string::npos || name.find("SM_") != std::string::npos || name.find("Sm_") != std::string::npos||
				//name.find("Decal") != std::string::npos || name.find("MI") != std::string::npos || name.find("BO") != std::string::npos || name.find("Light") != std::string::npos ||
				//name.find("Ambient") != std::string::npos || name.find("BlockingVolume") != std::string::npos || name.find("BP_Wall") != std::string::npos ||
				//name.find("BP_Lamp") != std::string::npos || name.find("BP_Floor") != std::string::npos || name.find("NODE") != std::string::npos) continue;

			//WorldActors.push_back(actor);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}