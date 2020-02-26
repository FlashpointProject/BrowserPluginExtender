#include <windows.h>
#include "main.h"

const wchar_t* SYSDIR = L"..\\..\\..\\BrowserPlugins\\Shockwave";

__declspec(naked) void getSystemDirectoryWExtendedCode() {
	__asm {
		push eax;
		push [esp + 0x0000000C];
		push [SYSDIR];
		push [esp + 0x00000010];
		call wcsncpy;
		pop eax;
		pop eax;
		pop eax;
		pop eax;
		retn 0x00000008;
	}
}

bool extender() {
	// set this to your Error Caption
	LPCTSTR errorCaption = "Shockwave Extender Error";

	// get Module Handle
	HMODULE moduleHandle = GetModuleHandle("np32dsw.dll");

	if (!moduleHandle) {
		MessageBox(NULL, "Failed to get Module Handle", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	// test it
	const size_t SHOCKWAVE_TEST_CODE_SIZE = 10;
	unsigned char shockwaveTestCode[SHOCKWAVE_TEST_CODE_SIZE] = {0x41, 0x00, 0x64, 0x00, 0x6F, 0x00, 0x62, 0x00, 0x65, 0x00};

	if (!testCode(errorCaption, moduleHandle, 0x000198D8, SHOCKWAVE_TEST_CODE_SIZE, shockwaveTestCode)) {
		MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	// extend it
	if (!extendCode(errorCaption, moduleHandle, 0x00003ECE, getSystemDirectoryWExtendedCode, true)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x00003ED3)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x00003F60, getSystemDirectoryWExtendedCode, true)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x00003F65)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x00004085, getSystemDirectoryWExtendedCode, true)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x0000408A)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	// gets KERNEL32
	/*
	if (!extendCode(errorCaption, moduleHandle, 0x00004E82, getSystemDirectoryWExtendedCode, true)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x00004E87)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	*/

	if (!extendCode(errorCaption, moduleHandle, 0x000057D7, getSystemDirectoryWExtendedCode, true)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}

	if (!extendCode(errorCaption, moduleHandle, 0x000057DC)) {
		MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}