#pragma once
#include "Global.h"

#if MemCheckBypass
namespace TaskScheduler
{
	typedef enum//From Roblox Source
	{
		Done,	        // The job will be removed from the TaskScheduler
		Stepped,        // Another step will be scheduled
	} StepResult;

	struct VFTableStruct {
		uintptr_t Ukn;
		uintptr_t Ukn1;
		uintptr_t Ukn2;
		uintptr_t Ukn3;
		uintptr_t Main;
	};

	struct JobStruct {
		VFTableStruct* VFTable;
		int unk1, unk2, unk3;
		std::string JobName;
		int Padding_0[4];
		double JobTimeElapsed;
	};

	struct MemoryPoint {
		uintptr_t Address;
		uint8_t* Bytes;
		size_t Size;
		uint8_t* Reverts;
	};
	extern uintptr_t GetJobs;
	extern std::vector<MemoryPoint> MemoryChanges;
	extern StepResult(__fastcall* OriginalMemcheckJob)(uintptr_t this_, void* junk, int stats);
	extern std::vector<std::shared_ptr<JobStruct>> Jobs;
	extern bool Loaded;
	void UpdateTaskScheduler();
	void PrintJobs();
	bool LoadJobs();
	void PrintJobVFTable(std::shared_ptr<JobStruct> Job);
	std::shared_ptr<JobStruct> GetJobByName(std::string Name);
	void ReplaceMainFunc(std::shared_ptr<JobStruct> Job, uintptr_t New);
	void WriteMemory(uintptr_t Address, uint8_t* Bytes, size_t Size, bool Revert = false);

	//Individual Stuff
	extern bool mempause;
	StepResult __fastcall MemcheckHook(uintptr_t this_, void* junk, int stats);

}

#endif