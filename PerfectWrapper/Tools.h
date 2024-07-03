#pragma once
#include "Global.h"

namespace Tools
{
	namespace Files
	{
		std::string ReadFile(const std::string& path);
		std::string GetDLLPath(const char* Addon = "");
		void WriteFile(const std::string& path, std::string writing);
	}

	namespace Memory
	{
        struct SegmentInfo
        {
            uint32_t Start;
            uint32_t End;
            uint32_t Size;
        };
        extern SegmentInfo Text, RData, Data;
        extern bool Loaded;
        namespace Strings
        {
            class String
            {
            private:
                uintptr_t Location = 0;
                std::string Text;
            public:
                uintptr_t GetLocation();
                std::string GetString();
                std::vector<uintptr_t> GetXRefs();
                String(uintptr_t GivenStringLocation);
            };
            extern std::vector<String> ProcessStrings;
            void FindStrings();
            String FindString(std::string GivenString);
            String FindString(uintptr_t Address);
        }
        SegmentInfo GetSegmentInfo(const char* sectionName);
        void DefineSegments();
        bool CheckSegment(const char* SegmentName, uintptr_t Address);
        uintptr_t FindNextPrologue(uintptr_t Address);
        bool IsPrologue(uintptr_t Address);
        std::vector<uintptr_t> GetCallsNoCheck(uintptr_t Given, uintptr_t NextPrologue = 0, int Limit = -1);
        std::vector<uintptr_t> GetCalls(uintptr_t Address, uintptr_t NextPrologue = 0, int Limit = -1);
        std::vector<uintptr_t> GetXRefs(uintptr_t Address, int Limit = -1);
        std::vector<DWORD> FindPattern(char* pattern, char* mask, int Amount);
        uintptr_t dwFindPattern2(const unsigned char* pat, const char* msk);
	}
}