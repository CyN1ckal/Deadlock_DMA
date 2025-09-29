#pragma once
#include <thread>
#include <string>
#include <print>
#include <unordered_map>
#include <array>

#include "vmmdll.h"

#include "Scheduler.hpp"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "DMA/DMA.h"
#include "DMA/Process.h"

#include "Deadlock/Offsets.h"
#include "Deadlock/Deadlock.h"