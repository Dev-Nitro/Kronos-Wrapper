#pragma once
#include "Global.h"


namespace Env
{
	typedef void(*lua_CFunctionY) (lua_State* L);
	extern lua_State* GL;
	extern uintptr_t RobloxState;
	extern uintptr_t DataModel;
	namespace Primary
	{
		extern const std::vector<const char*> Globals;
		extern const std::vector<const char*> Sub;
		extern bool Ready;

		bool CreateEnvironment();
		bool ROpenState();

		void FuncWrap(uintptr_t Thread, const char* LibName, const char* Name);
		void WrapGlobals(uintptr_t RL);
		int RobloxAttached(lua_State* L);

		void SandBoxThread(lua_State* Thread);
		bool Execute(std::string Script, bool Minimal = false);

	}

	namespace RunSteppedHook
	{
		extern uint32_t ScheduleThread;

		bool AttachHook(uintptr_t RL);

		int Hook(uintptr_t RL);
	}

	namespace TeleportHandler
	{
		extern std::vector<const char*> TeleportStateEnum;
		bool AttachTeleportClosure(uintptr_t RL);
		int TeleportHitClosure(uintptr_t RL);
		void Teleporting();
	}

	namespace Wrapping
	{
		void StartUpWrap(lua_State* L, uintptr_t RL, const int32_t idx);
		void Wrap(lua_State* L, uintptr_t RL, const int32_t idx);
		void Wrap(uintptr_t RL, lua_State* L, const int32_t idx);

	}

	namespace YieldingHandling
	{

		int Resume(uintptr_t RL);
		uint8_t VResume(lua_State* L, const uint16_t args);

		bool CheckRobloxYield(lua_State* L);

		//Pcall section
		void PcallOnSuccess(lua_State* L);
		void PcallOnError(lua_State* L);
		int PCall(lua_State* L);

		namespace Coroutine
		{

			const char* const statnames[] =
			{ "running", "suspended", "normal", "dead" };

			uint8_t costatus(lua_State* L, lua_State* co);
			int luaB_costatus(lua_State* L);
			int32_t auxresume(lua_State* L, lua_State* co, int narg);
			int luaB_coresume(lua_State* L);
			int luaB_auxwrap(lua_State* L);
			int luaB_cocreate(lua_State* L);
			int luaB_cowrap(lua_State* L);
			int luaB_yield(lua_State* L);
		}

	}

	namespace FunctionHandling
	{
		int StartUpCall(uintptr_t RL);
		int VanillaCall(uintptr_t RL);
		int RobloxCall(lua_State* L);
	}

	namespace MetaMethods
	{
		int UDIndex(lua_State* L);
		int UDGC(lua_State* L);
		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int Len(lua_State* L);
	}

	namespace CustomFunctions
	{
		int SetNDM(lua_State* L);
		int SetTrustCheck(lua_State* L);
		int GNewIndex(lua_State* L);
		int GIndex(lua_State* L);
		int GetGenv(lua_State* L);
		int ReadFile(lua_State* L);
		int WriteFile(lua_State* L);
		int GetRenv(lua_State* L);
		int GetReg(lua_State* L);
		int setreadonly(lua_State* L);
		int isreadonly(lua_State* L);
		int IsCClosure(lua_State* L);
		int GetRegList(lua_State* L);
		int SetClipboard(lua_State* L);
		int GetClipboard(lua_State* L);
		int FireClickDetector(lua_State* L);
		int MoveMouseRel(lua_State* L);
		int Mouse1Click(lua_State* L);
		int Mouse1Down(lua_State* L);
		int Mouse1Up(lua_State* L);
		int Mouse2Click(lua_State* L);
		int Mouse2Down(lua_State* L);
		int Mouse2Up(lua_State* L);
		int GetGC(lua_State* L);
		int IsRobloxObject(lua_State* L);
		int CPrint(lua_State* L);
		int GetRawMetaTable(lua_State* L);
		int GetConnections(lua_State* L);
		int UnLockModule(lua_State* L);
		int CheckCaller(lua_State* L);
		int GetNameCallMethod(lua_State* L);
		int SetNameCallMethod(lua_State* L);
		int HookFunction(lua_State* L);
		int KRONOS_LOADED(lua_State* L);

		namespace DebugLib
		{
			int dGetUpValues(lua_State* L);
			int dGetUpValue(lua_State* L);
			int dSetUpValue(lua_State* L);
			int dGetConstant(lua_State* L);
			int dSetConstant(lua_State* L);
			int dGetConstants(lua_State* L);
			int dGetProtos(lua_State* L);
			int dGetProto(lua_State* L);
			int dGetRegistry(lua_State* L);

			void luaopen_rdebug(lua_State* L);
		}

	}

	namespace RobloxFunc
	{
		void r_lua_pushobject(uintptr_t RL, uintptr_t Obj, uint8_t Type);
		bool r2_lua_getupvalue(uintptr_t RL, int32_t Clo, int32_t Index);
		bool r2_lua_setupvalue(uintptr_t RL, int32_t Clo, int32_t Index);
		bool r_lua_getconstant(uintptr_t RL, int32_t Clo, int32_t Index);
		bool r_lua_setconstant(uintptr_t RL, int32_t Clo, int32_t Index);
#if FullyAutoUpdatingCallCheck
		void r2_lua_pushcclosure(uintptr_t RL, uintptr_t Function, uint32_t UpVals, bool Break = false);
#else
		void r2_lua_pushcclosure(uintptr_t RL, uintptr_t Function, uint32_t UpVals);
#endif

		void m_settvalue(TValue*& First, TValue* New, bool top = false);
		bool CheckRobloxObject(lua_State* L, const int32_t idx);
	}

}

int r_luaL_getmetafield(uintptr_t RL, int obj, const char* event);
TValue* index2adr(lua_State* L, int idx);