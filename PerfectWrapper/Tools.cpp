#include "Tools.h"

namespace Tools
{
	namespace Files
	{
        std::string ReadFile(const std::string& path)//http://0x80.pl/notesen/2019-01-07-cpp-read-file.html
        {
            auto close_file = [](FILE* f) {fclose(f); };
            auto holder = std::unique_ptr<FILE, decltype(close_file)>(fopen(path.c_str(), "rb"), close_file);
            if (!holder)
                return "";
            FILE* f = holder.get();
            if (fseek(f, 0, SEEK_END) < 0)
                return "";
            const long size = ftell(f);
            if (size < 0)
                return "";
            if (fseek(f, 0, SEEK_SET) < 0)
                return "";
            std::string res;
            res.resize(size);
            fread(const_cast<char*>(res.data()), 1, size, f);
            return res;
        }

        void WriteFile(const std::string& path, std::string writing)
        {
            std::ofstream out(path);
            out << writing;
            out.close();
        }

        std::string GetDLLPath(const char* Addon)//\\workspace
        {
            HMODULE hModule = GetModuleHandleA(ExploitFile);
            if (hModule == NULL)
                return "";
            char buffer[MAX_PATH + 1];
            GetModuleFileNameA(hModule, buffer, MAX_PATH);
            std::string::size_type pos = std::string(buffer).find_last_of("\\/");
            return std::string(buffer).substr(0, pos).append(Addon);
        }
	}

    namespace Memory
    {
		SegmentInfo Text, RData, Data;
		bool Loaded = false;
		namespace Strings
		{
			std::vector<String> ProcessStrings;
			uintptr_t String::GetLocation()
			{
				return Location;
			}
			std::string String::GetString()
			{
				return Text;
			}
			std::vector<uintptr_t> String::GetXRefs()
			{
				return Memory::GetXRefs(Location);
			}
			String::String(uintptr_t GivenStringLocation)
			{
				Location = GivenStringLocation;
				if (GivenStringLocation != 0)
				{
					Text = std::string((char*)GivenStringLocation);
				}
			}
			void FindStrings()
			{
				if (!Loaded) {
					Memory::DefineSegments();
					Loaded = true;
				}
				for (int i = RData.Start; i < RData.End; i++)
				{
					if (*reinterpret_cast<uint8_t*>(i) != 0x0) {
						std::string x = std::string((char*)i);
						bool Invalid = false;
						for (int o = 0; o < x.size(); o++)
						{
							if ((int)x[o] < 0x20 || (int)x[o] > 0x7E)
								Invalid = true;
						}
						if (!Invalid)
							ProcessStrings.push_back(String(i));
						i = i + x.size() - 1;
					}
				}
			}
			String FindString(std::string GivenString)
			{
				for (int i = 0; i < ProcessStrings.size(); i++)
				{
					if (ProcessStrings[i].GetString() == GivenString)
					{
						return ProcessStrings[i];
					}
				}
				return String(0);
			}

			String FindString(uintptr_t Address)
			{
				for (int i = 0; i < ProcessStrings.size(); i++)
				{
					if (ProcessStrings[i].GetLocation() == Address)
					{
						return ProcessStrings[i];
					}
				}
				return String(0);
			}
		}

		SegmentInfo GetSegmentInfo(const char* sectionName)
		{
			IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)GetModuleHandle(0);
			IMAGE_NT_HEADERS* e_lfanewPtr = (IMAGE_NT_HEADERS*)(reinterpret_cast<uint8_t*>(dosHeader) + dosHeader->e_lfanew);
			IMAGE_SECTION_HEADER* sections = IMAGE_FIRST_SECTION(e_lfanewPtr);

			for (WORD i = 0; i < e_lfanewPtr->FileHeader.NumberOfSections; i++)
			{
				if (!memcmp(&sections[i].Name, (char*)sectionName, 8))
				{
					uintptr_t Start = reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(dosHeader) + sections[i].VirtualAddress);
					uintptr_t Size = sections[i].Misc.VirtualSize;
					uintptr_t End = Start + Size;
					return SegmentInfo({ Start, End, Size });
				}
			}
			return SegmentInfo({ 0, 0, 0 });
		}
		void DefineSegments()
		{
			Memory::Text = GetSegmentInfo(".text");
			Memory::RData = GetSegmentInfo(".rdata");
			Memory::Data = GetSegmentInfo(".data");
		}
		bool IsPrologue(uintptr_t Address)
		{
			BYTE* b = (BYTE*)Address;
			if (b[0] == 0x55 && b[1] == 0x8B && b[2] == 0xEC)
			{
				return true;
			}
			else if (b[0] == 0x53 && b[1] == 0x8B && b[2] == 0xDC)
			{
				return true;
			}
			else if (b[0] == 0x56 && b[1] == 0x8B && b[2] == 0xF1)
			{

				for (uint32_t i = 0; i < 0xFF; i++) {
					uintptr_t Current = (Address + i);
					if (*(BYTE*)Current == 0x5E && (*(BYTE*)(Current + 1) == 0xC3 || *(BYTE*)(Current + 1) == 0xC2)) {
						return true;
					}
				}

				return false;
			}
			else
			{
				return false;
			}
		}
		std::vector<uintptr_t> GetXRefs(uintptr_t Address, int Limit)
		{
			std::vector<uintptr_t> XRefs;
			for (int i = Text.Start; i < Text.End; i++)
			{
				BYTE* b = (BYTE*)i;
				if (IsPrologue(Address)) {
					if (b[0] == 0xE8)
					{
						uintptr_t CAddress = (i + static_cast<int32_t>(*(UINT*)(i + 1)) + 5);
						if (CAddress == Address) {
							XRefs.push_back(i);
							if (XRefs.size() == Limit)
								break;
						}
						i += 4;
					}
				}
				else
				{
					if (b[0] == 0xBA || b[0] == 0x68)
					{
						if (*reinterpret_cast<uintptr_t*>(i + 1) == Address) {
							XRefs.push_back(i);
							if (XRefs.size() == Limit)
								break;
						}
					}
				}
			}
			return XRefs;
		}
		bool CheckSegment(const char* SegmentName, uintptr_t Address)
		{
			SegmentInfo Segment = Memory::GetSegmentInfo((char*)SegmentName);
			if (Address >= Segment.Start && Address <= Segment.End)
				return true;
			else
				return false;
		}
		uintptr_t FindNextPrologue(uintptr_t Address)
		{
			uintptr_t Location = Address;
			bool Prologue = false;
			if (Address != 0) {
				do
				{
					Location += 0x1;
					if (Location > Text.End)
						return 0;
					Prologue = Memory::IsPrologue(Location);
				} while (!Prologue);
			}
			return Location;
		}
		std::vector<uintptr_t> GetCallsNoCheck(uintptr_t Given, uintptr_t NextPrologue, int Limit)
		{
			std::vector<uintptr_t> Calls;
			if (!NextPrologue) {
				for (int i = 0; (i + Given) < Memory::FindNextPrologue(Given); i++)
				{
					uintptr_t CLocation = Given + i;
					if (*(BYTE*)CLocation == 0xE8)
					{
						uintptr_t CAddress = (CLocation + static_cast<int32_t>(*(UINT*)(CLocation + 1)) + 5);
						if (CheckSegment(".text", CAddress))
						{
							Calls.push_back(CAddress);
							if (Calls.size() == Limit)
								break;
						}
					}
				}
			}
			else
			{
				for (int i = 0; (i + Given) < NextPrologue; i++)
				{
					uintptr_t CLocation = Given + i;
					if (*(BYTE*)CLocation == 0xE8)
					{
						uintptr_t CAddress = (CLocation + static_cast<int32_t>(*(UINT*)(CLocation + 1)) + 5);
						if (CheckSegment(".text", CAddress))
						{
							Calls.push_back(CAddress);
							if (Calls.size() == Limit)
								break;
						}
					}
				}
			}
			return Calls;
		}
		std::vector<uintptr_t> GetCalls(uintptr_t Given, uintptr_t NextPrologue, int Limit)
		{
			std::vector<uintptr_t> Calls;
			if (!NextPrologue) {
				for (int i = 0; (i + Given) < Memory::FindNextPrologue(Given); i++)
				{
					uintptr_t CLocation = Given + i;
					if (*(BYTE*)CLocation == 0xE8)
					{
						uintptr_t CAddress = (CLocation + static_cast<int32_t>(*(UINT*)(CLocation + 1)) + 5);
						if (CheckSegment(".text", CAddress) && IsPrologue(CAddress))
						{
							Calls.push_back(CAddress);
							if (Calls.size() == Limit)
								break;
						}
					}
				}
			}
			else
			{
				for (int i = 0; (i + Given) < NextPrologue; i++)
				{
					uintptr_t CLocation = Given + i;
					if (*(BYTE*)CLocation == 0xE8)
					{
						uintptr_t CAddress = (CLocation + static_cast<int32_t>(*(UINT*)(CLocation + 1)) + 5);
						if (CheckSegment(".text", CAddress) && IsPrologue(CAddress))
						{
							Calls.push_back(CAddress);
							if (Calls.size() == Limit)
								break;
						}
					}
				}
			}
			return Calls;
		}
		std::vector<DWORD> FindPattern(char* pattern, char* mask, int Amount)
		{
			std::vector<DWORD> PatternsFound;
			//Get length for our mask, this will allow us to loop through our array
			DWORD patternLength = (DWORD)strlen(mask);

			for (DWORD i = Text.Start; i < Text.End - patternLength; i++)
			{
				bool found = true;
				for (DWORD j = 0; j < patternLength; j++)
				{
					//if we have a ? in our mask then we have true by default,
					//or if the bytes match then we keep searching until finding it or not
					found &= mask[j] == '?' || pattern[j] == *(char*)(i + j);
				}

				//found = true, our entire pattern was found
				if (found)
				{
					PatternsFound.push_back(i);
					if (PatternsFound.size() == Amount)
					{
						break;
					}
				}
			}
			return PatternsFound;
		}
		uintptr_t dwFindPattern2(const unsigned char* pat, const char* msk)
		{
			uint8_t* pData = reinterpret_cast<uint8_t*>(Memory::Text.Start);
			const unsigned char* end = reinterpret_cast<uint8_t*>(Memory::Text.End) - strlen(msk);
			int num_masks = ceil((float)strlen(msk) / (float)16);
			int masks[32]; //32*16 = enough masks for 512 bytes
			memset(masks, 0, num_masks * sizeof(int));
			for (int i = 0; i < num_masks; ++i)
				for (int j = strnlen(msk + i * 16, 16) - 1; j >= 0; --j)
					if (msk[i * 16 + j] == 'x')
						masks[i] |= 1 << j;

			__m128i xmm1 = _mm_loadu_si128((const __m128i*) pat);
			__m128i xmm2, xmm3, mask;
			for (; pData != end; _mm_prefetch((const char*)(++pData + 64), _MM_HINT_NTA)) {
				if (pat[0] == pData[0]) {
					xmm2 = _mm_loadu_si128((const __m128i*) pData);
					mask = _mm_cmpeq_epi8(xmm1, xmm2);
					if ((_mm_movemask_epi8(mask) & masks[0]) == masks[0]) {
						for (int i = 1; i < num_masks; ++i) {
							xmm2 = _mm_loadu_si128((const __m128i*) (pData + i * 16));
							xmm3 = _mm_loadu_si128((const __m128i*) (pat + i * 16));
							mask = _mm_cmpeq_epi8(xmm2, xmm3);
							if ((_mm_movemask_epi8(mask) & masks[i]) == masks[i]) {
								if ((i + 1) == num_masks)
									return (uintptr_t)pData;
							}
							else goto cont;
						}
						return (uintptr_t)pData;
					}
				}cont:;
			}
			return NULL;
		}
    }

}