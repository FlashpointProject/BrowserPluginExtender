#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include <Dbghelp.h>

typedef DWORD VIRTUAL_ADDRESS;
typedef DWORD VIRTUAL_SIZE;
typedef DWORD RELATIVE_VIRTUAL_ADDRESS;
typedef DWORD EXTENDED_CODE_ADDRESS;

inline size_t stringSize(const char* string) {
	return strlen(string) + 1;
}

inline bool stringsEqual(const char* leftHandSide, const char* rightHandSide) {
	return !strcmp(leftHandSide, rightHandSide);
}

inline bool memoryEqual(const void* buffer, const void* buffer2, size_t bufferSize) {
	return !memcmp(buffer, buffer2, bufferSize);
}

bool shiftMemory(size_t bufferSize, const void* buffer, size_t sourceSize, const void* source, unsigned int shift, bool direction) {
	if (source < buffer || (char*)source + sourceSize >(char*)buffer + bufferSize) {
		return false;
	}

	size_t destinationSize = (char*)buffer + bufferSize - source;
	char* destination = (char*)source;

	if (!direction) {
		destination -= shift;
	} else {
		destination += shift;
	}

	if (destination < buffer || destination + sourceSize >(char*)buffer + bufferSize) {
		return false;
	}
	return !memmove_s(destination, destinationSize, source, sourceSize);
}

bool extender();

extern "C" BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(moduleHandle);

		{
			if (!extender()) {
				TerminateProcess(GetCurrentProcess(), -1);
				return FALSE;
			}
		}
	}
	return TRUE;
}

inline EXTENDED_CODE_ADDRESS createExtendedCodeAddress(HMODULE moduleHandle, RELATIVE_VIRTUAL_ADDRESS relativeVirtualAddress) {
	return (VIRTUAL_ADDRESS)moduleHandle + relativeVirtualAddress;
}

bool testSectionAddress(LPCTSTR errorCaption, HMODULE moduleHandle, VIRTUAL_ADDRESS virtualAddress, VIRTUAL_SIZE virtualSize) {
	if (!moduleHandle) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "moduleHandle cannot be NULL", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	PIMAGE_NT_HEADERS imageNTHeader = ImageNtHeader(moduleHandle);

	if (!imageNTHeader) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "Failed to Get Image NT Header", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	PIMAGE_SECTION_HEADER imageSectionHeader = (PIMAGE_SECTION_HEADER)(imageNTHeader + 1);

	if (!imageSectionHeader) {
		imageNTHeader = NULL;

		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "Failed to Get Image Section Header", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	for (WORD i = 0; i < imageNTHeader->FileHeader.NumberOfSections; i++) {
		if (virtualAddress >= (VIRTUAL_ADDRESS)moduleHandle + imageSectionHeader->VirtualAddress && virtualAddress + virtualSize < (VIRTUAL_ADDRESS)moduleHandle + imageSectionHeader->VirtualAddress + imageSectionHeader->Misc.VirtualSize) {
			imageNTHeader = NULL;
			imageSectionHeader = NULL;
			return true;
		}

		imageSectionHeader++;
	}

	imageNTHeader = NULL;
	imageSectionHeader = NULL;

	if (!errorCaption) {
		return false;
	}

	MessageBox(NULL, "Failed to Test Address", errorCaption, MB_OK | MB_ICONERROR);
	return false;
}

bool unprotectCode(LPCTSTR errorCaption, HMODULE moduleHandle, VIRTUAL_ADDRESS virtualAddress, VIRTUAL_SIZE virtualSize, DWORD &oldProtect) {
	if (!moduleHandle) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "moduleHandle cannot be NULL", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!testSectionAddress(errorCaption, moduleHandle, virtualAddress, virtualSize)) {
		return false;
	}

	if (!virtualAddress || !virtualSize || !VirtualProtect((LPVOID)virtualAddress, virtualSize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "Failed to Unprotect Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	// get Basic Memory Information
	MEMORY_BASIC_INFORMATION memoryBasicInformation;

	if (VirtualQuery((LPCVOID)virtualAddress, &memoryBasicInformation, sizeof(memoryBasicInformation)) != sizeof(memoryBasicInformation)
		|| !memoryBasicInformation.Protect
		|| memoryBasicInformation.Protect & PAGE_NOACCESS
		|| memoryBasicInformation.Protect & PAGE_EXECUTE) {
		return false;
	}
	return true;
}

bool protectCode(LPCTSTR errorCaption, HMODULE moduleHandle, VIRTUAL_ADDRESS virtualAddress, VIRTUAL_SIZE virtualSize, DWORD &oldProtect) {
	if (!moduleHandle) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "moduleHandle cannot be NULL", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!testSectionAddress(errorCaption, moduleHandle, virtualAddress, virtualSize)) {
		return false;
	}

	if (!virtualAddress || !virtualSize || !VirtualProtect((LPVOID)virtualAddress, virtualSize, oldProtect, &oldProtect)) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "Failed to Protect Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool flushCode(LPCTSTR errorCaption, HMODULE moduleHandle, VIRTUAL_ADDRESS virtualAddress, VIRTUAL_SIZE virtualSize) {
	if (!moduleHandle) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "moduleHandle cannot be NULL", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!testSectionAddress(errorCaption, moduleHandle, virtualAddress, virtualSize)) {
		return false;
	}

	if (!FlushInstructionCache(GetCurrentProcess(), (LPCVOID)virtualAddress, virtualSize)) {
		if (!errorCaption) {
			return false;
		}

		MessageBox(NULL, "Failed to Flush Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool testCode(LPCTSTR errorCaption, HMODULE moduleHandle, RELATIVE_VIRTUAL_ADDRESS relativeVirtualAddress, VIRTUAL_SIZE virtualSize, unsigned char code[]) {
	if (!moduleHandle) {
		return false;
	}

	VIRTUAL_ADDRESS virtualAddress = (VIRTUAL_ADDRESS)moduleHandle + relativeVirtualAddress;
	DWORD oldProtect = 0;

	if (!unprotectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}

	bool result = memoryEqual((const void*)virtualAddress, (const void*)code, virtualSize);

	if (!protectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}
	return result;
}

bool extendCode(LPCTSTR errorCaption, HMODULE moduleHandle, RELATIVE_VIRTUAL_ADDRESS relativeVirtualAddress, void* code, bool call = false) {
	if (!moduleHandle) {
		return false;
	}

	VIRTUAL_ADDRESS virtualAddress = (VIRTUAL_ADDRESS)moduleHandle + relativeVirtualAddress;
	VIRTUAL_SIZE virtualSize = 5;
	DWORD oldProtect = 0;

	if (!unprotectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}

	*(PBYTE)virtualAddress = 0xE9 - call;
	*(VIRTUAL_ADDRESS*)((PBYTE)virtualAddress + 1) = (VIRTUAL_ADDRESS)code - virtualAddress - virtualSize;

	if (!flushCode(errorCaption, moduleHandle, virtualAddress, virtualSize)) {
		return false;
	}

	if (!protectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}
	return true;
}

bool extendCode(LPCTSTR errorCaption, HMODULE moduleHandle, RELATIVE_VIRTUAL_ADDRESS relativeVirtualAddress) {
	if (!moduleHandle) {
		return false;
	}

	VIRTUAL_ADDRESS virtualAddress = (VIRTUAL_ADDRESS)moduleHandle + relativeVirtualAddress;
	VIRTUAL_SIZE virtualSize = 1;
	DWORD oldProtect = 0;

	if (!unprotectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}

	*(PBYTE)virtualAddress = 0x90;

	if (!flushCode(errorCaption, moduleHandle, virtualAddress, virtualSize)) {
		return false;
	}

	if (!protectCode(errorCaption, moduleHandle, virtualAddress, virtualSize, oldProtect)) {
		return false;
	}
	return true;
}

#endif