#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Vector.h"
#include "kiero/kiero.h"

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;