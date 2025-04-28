#pragma once
#include "SDK.hpp"
#include "functions.h"
#include "features/Bones.h"
#include "features/Aimbot.h"

void gameLoop();

bool updateActors();
bool validActor(SDK::AActor* actor);
bool updateLocalplayer();

void _worldActors();
void _players();
void _bots();

void _local();