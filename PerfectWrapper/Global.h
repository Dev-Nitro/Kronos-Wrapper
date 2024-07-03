#pragma once
#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <emmintrin.h>
#include <mutex>

#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
#include "Lua/luaconf.h"
#include "Lua/lapi.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/ldo.h"
#include "Lua/lfunc.h"
#include "Lua/lopcodes.h"

//Below are for debugging purposes. Don't use unless you know what you're doing.
#define CorePrints false
#define Errors2 false
#define EWrapping false
#define DebugWrapping false
#define DebugThreading false
#define DebugCalls false
#define DebugMetaMethods false
#define FunctionNames false
#define DebugTeleport false

#define ConsoleInput false
#define ConsoleOpen false

//Basic info. Top two aren't important. Pipe name is if you are using pipes
#define ExploitFile "KronosV3.dll"
#define ExploitName "KronosV3"
#define PipesName "010010110111001001101111011011100110111101110011"

//Highly recommend you do not enable this. It is still unstable when used.
#define MemCheckBypass false

//Fully auto updating is done using the breakpoints(Typically a bad method especially if you plan to have hookfunction).
//The other one will require you to manually call the function I've stored for you in CallCheck to update it. Though this cannot happen on runtime.
//Has to be done then put it yourself as a replacement for HookFunction address in CallCheck
#define FullyAutoUpdatingCallCheck true


//Don't touch below here
#define aslr(x) (x - 0x400000 + (uint32_t)GetModuleHandle(0))
#define noaslr(x) (x + 0x400000 - (uint32_t)GetModuleHandle(0))

#define CO_RUN	0	/* running */
#define CO_SUS	1	/* suspended */
#define CO_NOR	2	/* 'normal' (it resumed another coroutine) */
#define CO_DEAD	3
