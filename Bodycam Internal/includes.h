#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <thread>
#include <cstdint>
#include <vector>
#include "Vector.h"
#include "kiero/kiero.h"
#include "ESP.h"
#include "globals.h"
#include "GUI.h"
#include "ActorCache.h"
#include "GameLoop.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;