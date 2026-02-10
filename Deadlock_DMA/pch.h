#pragma once
#include <thread>
#include <string>
#include <print>
#include <unordered_map>
#include <array>
#include <memory>
#include <mutex>
#include <iostream>
#include <random>
#include <numbers>
#include <expected>
#include <fstream>

#define NOMINMAX
#include <algorithm>

#ifdef DBGPRINT
#define DbgPrintln(...) std::println(__VA_ARGS__)
#else
#define DbgPrintln(...)
#endif

#ifdef CATCH_ENABLE
#include "catch_amalgamated.hpp"
#endif

#include "tracy/Tracy.hpp"

#include "vmmdll.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <json.hpp>

#include "DMA/DMA.h"
#include "DMA/Process.h"

#include "Deadlock/Offsets.h"
#include "Deadlock/Deadlock.h"

inline constexpr size_t MAX_ENTITIES = 512;
inline constexpr size_t MAX_ENTITY_LISTS = 32;