#include "CheckBypasses.h"
#include "Environment.h"
#include "MinHook/MinHook.h"
#include "XHosted.h"
#include "Tools.h"

namespace Bypasses
{

	namespace TrustCheck
	{
		std::string TrustCheck1Stored, TrustCheck2Stored;
		uintptr_t TrustCheck1;
		uintptr_t TrustCheck2;

		void UpdateTrustCheck()
		{
			//8B 15 ?? ?? ?? ?? 8B CA 8D 79 01 8A 01 41 84 C0
			if (!Tools::Memory::Loaded)
			{
				Tools::Memory::DefineSegments();
				Tools::Memory::Loaded = true;
			}
			uintptr_t Holder = Tools::Memory::dwFindPattern2((const unsigned char*)"\x8B\x15\x00\x00\x00\x00\x8B\xCA\x8D\x79\x01\x8A\x01\x41\x84\xC0", (char*)"xx????xxxxxxxxxx");
			if (Holder != 0)
			{
				TrustCheck1 = *reinterpret_cast<uintptr_t*>(Holder + 2);
				TrustCheck2 = TrustCheck1 + 4;
			}
			else
				printf("TrustCheck AoB OutDated\n");
		}

		void SetCheckStore()
		{
			TrustCheck1Stored = *(std::string*)(TrustCheck1);
			TrustCheck2Stored = *(std::string*)(TrustCheck2);
		}

		void SetURLs(std::string URL)
		{
			*reinterpret_cast<std::string*>(TrustCheck1) = URL;
			*reinterpret_cast<std::string*>(TrustCheck2) = URL;
		}

		void Revert()
		{
			if (*(std::string*)(TrustCheck1) != TrustCheck1Stored)
			{
				*(std::string*)(TrustCheck1) = TrustCheck1Stored;
				*(std::string*)(TrustCheck2) = TrustCheck2Stored;
			}
		}

	}
#if !FullyAutoUpdatingCallCheck
	namespace CallCheck
	{

		uintptr_t original;
		uintptr_t closureplacement = reinterpret_cast<uintptr_t>(&ClosurePlacement);
		_CreateEventA CreateEventAOriginal = NULL;
		uintptr_t CreateEventA = NULL;
		uintptr_t HookLocation = aslr(0x5A573B);
		uintptr_t Comparison = HookLocation + 6;
		uintptr_t CallCheckAddr;
		//0x5757B0 Call CreateEventAHook address

		__declspec(naked) void CreateEventAHook()
		{
			__asm
			{
				mov eax, [esp]
				cmp eax, Comparison//eax is previous Address location
				jne normal_case
				add esp, 4
				push[esp + 4]
				call[closureplacement]
				add esp, 4
				retn
				normal_case :
				jmp[original]
			}
		}

		int ClosurePlacement(uintptr_t RL)
		{
			uintptr_t Address = 0;
			for (int i = 1; i <= MAX_INT; i++)
			{
				uintptr_t Type = r_lua_type(RL, lua_upvalueindex(i));
				if (Type == LUA_TNONE)
					break;
				else if (Type == R_LUA_TNUMBER)
					Address = r_lua_tonumber(RL, lua_upvalueindex(i), NULL);
			}
			return ((int(*)(uintptr_t))Address)(RL);
		}

		bool StartUpCallCheck()
		{
			HMODULE Kernel32 = GetModuleHandleA("Kernel32.dll");
			if (!Kernel32) return false;
			CreateEventA = (uintptr_t)GetProcAddress(Kernel32, "CreateEventA");//".Client.crashevent" if you know yano
			if (!CreateEventA) return false;//MH_ERROR_ALREADY_INITIALIZED
			MH_STATUS Re = MH_Initialize();
			if (Re != MH_ERROR_ALREADY_INITIALIZED && Re != MH_OK) return false;//Needed temp until Drawing Lib is added
			if (MH_CreateHook((void*)CreateEventA, &CreateEventAHook, reinterpret_cast<LPVOID*>(&CreateEventAOriginal)) != MH_OK) return false;
			if (MH_EnableHook((void*)CreateEventA) != MH_OK) return false;
			original = (uintptr_t)CreateEventAOriginal;
			return true;
		}


		uintptr_t UpdateCallCheckAddress()
		{
			//C6 45 FC 04 FF 15 ?? ?? ?? ?? 89 06 85 C0 0F
			if (!Tools::Memory::Loaded)
			{
				Tools::Memory::DefineSegments();
				Tools::Memory::Loaded = true;
			}
			auto Found = Tools::Memory::dwFindPattern2((unsigned char*)"\xC6\x45\xFC\x04\xFF\x15\x00\x00\x00\x00\x89\x06\x85\xC0\x0F", (char*)"xxxxxx????xxxxx");
			if (Found != 0)
				return Found;
			printf("Call Check OutDated AoB\n");
			return 0;
		}

	}
#else
	namespace CallCheck
	{

		std::vector<Combined> int3breakpoints;
		LONG WINAPI vehHandler(PEXCEPTION_POINTERS ex)
		{
			switch (ex->ExceptionRecord->ExceptionCode)
			{
			case 0x80000003L:
			{

				if (ex->ContextRecord->Eip == int3breakpoints[0].BreakPoint)
				{
					ex->ContextRecord->Eip = reinterpret_cast<uintptr_t>(Env::FunctionHandling::VanillaCall);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if (ex->ContextRecord->Eip == int3breakpoints[1].BreakPoint)
				{
					ex->ContextRecord->Eip = reinterpret_cast<uintptr_t>(Env::YieldingHandling::Resume);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if (ex->ContextRecord->Eip == int3breakpoints[2].BreakPoint)
				{
					ex->ContextRecord->Eip = reinterpret_cast<uintptr_t>(Env::FunctionHandling::StartUpCall);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if (ex->ContextRecord->Eip == int3breakpoints[3].BreakPoint)
				{
					ex->ContextRecord->Eip = reinterpret_cast<uintptr_t>(Env::TeleportHandler::TeleportHitClosure);
					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else if (ex->ContextRecord->Eip == int3breakpoints[4].BreakPoint)
				{
					ex->ContextRecord->Eip = reinterpret_cast<uintptr_t>(Env::RunSteppedHook::Hook);
					return EXCEPTION_CONTINUE_EXECUTION;
				}

				for (int i = 0; i < int3breakpoints.size(); ++i)
				{
					if (ex->ContextRecord->Eip == int3breakpoints[i].BreakPoint)
					{
						ex->ContextRecord->Eip = (int3breakpoints[i].Func);
						return EXCEPTION_CONTINUE_EXECUTION;
					}
				}

				return -1;
			}
			default: return 0;
			}
			return 0;
		}

		void locateAllINT3()
		{
			if (!Tools::Memory::Loaded)
			{
				Tools::Memory::DefineSegments();
				Tools::Memory::Loaded = true;
			}
			uintptr_t _s = Tools::Memory::Text.Start;
			const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
			for (int i = 0; i < MAX_INT; i++) 
			{
				if ((_s + i) >= Tools::Memory::Text.End)
					break;
				if (memcmp(reinterpret_cast<void*>(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
					int3breakpoints.push_back({ _s + i , NULL});
				}
			}
		}

		uintptr_t FindFreeBreakPoint(uintptr_t Func)
		{
			for (int i = 0; i < int3breakpoints.size(); ++i)
			{
				if (int3breakpoints[i].Func == NULL)
				{
					int3breakpoints[i].Func = Func;
					return int3breakpoints[i].BreakPoint;
				}
			}
			printf("Ran out of breakpoints!\n");
			return NULL;
		}

		void VehHandlerpush()
		{

			locateAllINT3();
			int3breakpoints[0].Func = 1;
			int3breakpoints[1].Func = 1;
			int3breakpoints[2].Func = 1;
			int3breakpoints[3].Func = 1;
			int3breakpoints[4].Func = 1;
			AddVectoredExceptionHandler(1, vehHandler);
		}
	}
#endif
}