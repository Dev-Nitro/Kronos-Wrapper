#include "Global.h"
#include "Environment.h"
#include "Drawing/ImGuiHook.h"


//Functions
void DebugConsole()
{
	DWORD OldPerm;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	*reinterpret_cast<BYTE*>(&FreeConsole) = 0xC3;
	VirtualProtect(&FreeConsole, 1, OldPerm, NULL);
	AllocConsole();
	SetConsoleTitle("Debug Console");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

int Main()
{
#if ConsoleOpen
	DebugConsole();
#endif // !Debug
	if (Env::Primary::CreateEnvironment())//Env::StartUp()
	{
		std::string Input;
		while (true)
		{
#if ConsoleInput
			std::getline(std::cin, Input);
			Env::Primary::Execute(Input);
#else
			std::string WholeScript = "";
			HANDLE hPipe;
			char buffer[999999];
			DWORD dwRead;
			std::string PipeNamed = "\\\\.\\pipe\\" + std::string(PipesName);
			hPipe = CreateNamedPipe(TEXT(PipeNamed.c_str()),
				PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
				PIPE_WAIT,
				1,
				999999,
				999999,
				NMPWAIT_USE_DEFAULT_WAIT,
				NULL);
			while (hPipe != INVALID_HANDLE_VALUE)
			{
				if (ConnectNamedPipe(hPipe, NULL) != FALSE)
				{
					while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
					{
						buffer[dwRead] = '\0';
						try {
							try {
								WholeScript = WholeScript + buffer;
							}
							catch (...) {
							}
						}
						catch (std::exception e) {

						}
						catch (...) {

						}
					}
					Env::Primary::Execute(WholeScript.c_str());

					WholeScript = "";
				}
				DisconnectNamedPipe(hPipe);
			}
#endif
		}
	}
	return 0;
}

/*
Used as the entry point for the DLL.
*/
unsigned int ProtectSections(HMODULE Module) 
{
	MODULEINFO ModuleInfo;
	GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo));
	uintptr_t Address = reinterpret_cast<uintptr_t>(Module);
	uintptr_t TermAddress = Address + ModuleInfo.SizeOfImage;
	MEMORY_BASIC_INFORMATION MemoryInfo;


	while (Address < TermAddress) {
		VirtualQuery(reinterpret_cast<void*>(Address), &MemoryInfo, sizeof(MemoryInfo));
		if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			DWORD OldProtection;
			VirtualProtect(reinterpret_cast<void*>(Address), MemoryInfo.RegionSize, PAGE_EXECUTE_READ, &OldProtection);
		}
		Address = reinterpret_cast<uintptr_t>(MemoryInfo.BaseAddress) + MemoryInfo.RegionSize;
	}

	VirtualQuery(reinterpret_cast<void*>(MemoryInfo.AllocationBase), &MemoryInfo, sizeof(MemoryInfo));
	if (MemoryInfo.State != MEM_COMMIT || !(MemoryInfo.Protect & PAGE_EXECUTE_READ))
		return 0x400;
	return MemoryInfo.RegionSize - 0x400;
}

__forceinline void UnlinkModule(HINSTANCE Module
) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs: [30h]
			mov eax, [eax + 0Ch]
			mov PEB_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, Module
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, Module
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, Module
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	DisableThreadLibraryCalls(Module);
	INDICIUM_ENGINE_CONFIG cfg;
	INDICIUM_ENGINE_CONFIG_INIT(&cfg);
	cfg.Direct3D.HookDirect3D9 = TRUE;
	cfg.Direct3D.HookDirect3D10 = TRUE;
	cfg.Direct3D.HookDirect3D11 = TRUE;
	cfg.EvtIndiciumGameHooked = EvtIndiciumGameHooked;
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		UnlinkModule(Module);
		DWORD OldProtection;
		VirtualProtect(Module, 4096, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 4096);
		ProtectSections(Module);
		HANDLE hThread = NULL;
		HANDLE hDllMainThread = GetCurrentThread();
		if (Reserved == NULL) {
			if (!engine)
			{
				IndiciumEngineCreate(Module, &cfg, NULL);
			}
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL)))
			{
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		IndiciumEngineDestroy(Module);
	}
	}
	return TRUE;
}