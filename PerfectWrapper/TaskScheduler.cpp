#include "TaskScheduler.h"
#include "Tools.h"
#if MemCheckBypass
namespace TaskScheduler
{
	uintptr_t GetJobs;
	StepResult(__fastcall* OriginalMemcheckJob)(uintptr_t this_, void* junk, int stats);
	std::vector<std::shared_ptr<JobStruct>> Jobs;
	std::vector<MemoryPoint> MemoryChanges;
	bool Loaded = false;
	bool mempause = false;

	void UpdateTaskScheduler()
	{
		//8B BD 58 FF FF FF 83 EC 08 8B C4
		auto Found = Tools::Memory::dwFindPattern2((unsigned char*)"\x8B\xBD\x58\xFF\xFF\xFF\x83\xEC\x08\x8B\xC4", (char*)"xxxxxxxxxxx");
		if (Found != 0)
			GetJobs = ((Found - 5) + static_cast<int32_t>(*(UINT*)((Found - 5) + 1)) + 5);
		else
		{
			printf("Task Scheduler AoB Broken\n");
			auto First = Tools::Memory::Strings::FindString("dutyfraction");
			if (First.GetLocation() == 0)
			{
				First = Tools::Memory::Strings::FindString("interval.samples");
			}
			if (First.GetLocation() == 0)
				return;
			auto XRefs = First.GetXRefs();
			if (XRefs.size() == 1)
			{
				auto Calls = Tools::Memory::GetCalls(XRefs[0]);
				for (int i = 0; i < Calls.size(); i++)
				{
					uintptr_t noaslra = noaslr(Calls[i]);
					if (noaslra > 0xE00000 && noaslra < 0xF00000)
						GetJobs = Calls[i];
				}
			}
		}
	}

	void PrintJobs()
	{
		for (int i = 0; i < Jobs.size(); i++)
			printf("%s: 0x%X\n", Jobs[i]->JobName.c_str(), Jobs[i]->VFTable);
	}

	void PrintJobVFTable(std::shared_ptr<JobStruct> Job)
	{
		for (int i = 0; i < 5; i++)
			printf("[%d]: 0x%X\n", i, noaslr(*reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(Job->VFTable) + i * 4)));
	}

	bool LoadJobs()
	{
		((int(__cdecl*)(int))GetJobs)(reinterpret_cast<int>(&Jobs));
		if (Jobs.size() > 0) 
		{
			Loaded = true;
			return true;
		}
		return false;
	}

	void ReplaceMainFunc(std::shared_ptr<JobStruct> Job, uintptr_t New)
	{
		size_t vftSize = 4 * 5;
		VFTableStruct* newVft = reinterpret_cast<VFTableStruct*>(malloc(vftSize));
		memcpy(newVft, Job->VFTable, vftSize);
		newVft->Main = New;
		Job->VFTable = newVft;
	}

	std::shared_ptr<JobStruct> GetJobByName(std::string Name)
	{
		if (!Loaded) LoadJobs();
		if (!Loaded) return NULL;
		for (int i = 0; i < Jobs.size(); i++)
			if (Jobs[i]->JobName == Name)
				return Jobs[i];
		return NULL;
	}

	void WriteMemory(uintptr_t Address, uint8_t* Bytes, size_t Size, bool Revert)
	{
		uint32_t oldProtect;
		VirtualProtect(reinterpret_cast<LPVOID>(Address), Size, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&oldProtect));
		if (!Revert)
		{
			uint8_t* Reverts = reinterpret_cast<uint8_t*>(malloc(Size));
			memcpy(Reverts, reinterpret_cast<uint8_t*>(Address), Size);
			MemoryChanges.push_back({ Address, Bytes, Size, Reverts });
		}
		for (int i = 0; i < Size; i++)
			reinterpret_cast<uint8_t*>(Address)[i] = Bytes[i];
		VirtualProtect(reinterpret_cast<LPVOID>(Address), Size, oldProtect, reinterpret_cast<PDWORD>(&oldProtect));
	}

	StepResult __fastcall MemcheckHook(uintptr_t this_, void* junk, int stats)
	{
		for (int i = 0; i < MemoryChanges.size(); ++i)
			WriteMemory(MemoryChanges[i].Address, MemoryChanges[i].Reverts, MemoryChanges[i].Size, true);
		StepResult Return = OriginalMemcheckJob(this_, junk, stats);
		for (int i = 0; i < MemoryChanges.size(); ++i)
			WriteMemory(MemoryChanges[i].Address, MemoryChanges[i].Bytes, MemoryChanges[i].Size, true);
		return Return;
	}

}

#endif