#pragma once
#include "Global.h"

typedef HANDLE(WINAPI* _CreateEventA)(LPSECURITY_ATTRIBUTES lp_event_attributes, BOOL b_banual_reset, BOOL b_initial_state, LPCTSTR lp_name);
namespace Bypasses
{

	namespace TrustCheck
	{
		extern std::string TrustCheck1Stored, TrustCheck2Stored;
		extern uintptr_t TrustCheck1, TrustCheck2;
		void UpdateTrustCheck();
		void SetCheckStore();
		void SetURLs(std::string URL);
		void Revert();

	}
#if !FullyAutoUpdatingCallCheck
	namespace CallCheck
	{
		extern uintptr_t CallCheckAddr;
		extern uintptr_t original;
		extern uintptr_t closureplacement;
		extern uintptr_t CreateEventA;
		extern _CreateEventA CreateEventAOriginal;
		extern uintptr_t HookLocation;
		extern uintptr_t Comparison;
		void CreateEventAHook();
		int ClosurePlacement(uintptr_t RL);
		bool StartUpCallCheck();
		uintptr_t UpdateCallCheckAddress();

	}
#else
	namespace CallCheck
	{
		struct Combined
		{
			uintptr_t BreakPoint;
			uintptr_t Func;
		};

		extern std::vector<Combined> int3breakpoints;
		uintptr_t FindFreeBreakPoint(uintptr_t Func);
		LONG WINAPI vehHandler(PEXCEPTION_POINTERS ex);
		void locateAllINT3();
		void VehHandlerpush();
	}
#endif
}