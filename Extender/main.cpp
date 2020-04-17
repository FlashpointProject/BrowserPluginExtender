#include <windows.h>
#include <atlbase.h>
#include <Shlwapi.h>
#include "main.h"

const size_t MAX_PATH_WIDE = MAX_PATH * 2;
const wchar_t* BROWSER_PLUGINS = L"\\..\\..\\..\\BrowserPlugins\\";
const wchar_t* SHOCKWAVE = L"Shockwave";
const wchar_t* CLICKTEAM = L"Clickteam";
wchar_t sysdirShockwave[MAX_PATH_WIDE];
wchar_t sysdirClickteam[MAX_PATH_WIDE];

__declspec(naked) void getSystemDirectoryWExtendedCodeShockwave() {
	__asm {
		push eax;
		push[esp + 0x0000000C];
		lea eax, [sysdirShockwave];
		push eax;
		push[esp + 0x00000010];
		call wcsncpy;
		pop eax;
		pop eax;
		pop eax;
		pop eax;
		retn 0x00000008;
	}
}

__declspec(naked) void getSystemDirectoryWExtendedCodeClickteam() {
	__asm {
		push eax;
		push[esp + 0x0000000C];
		lea eax, [sysdirClickteam];
		push eax;
		push[esp + 0x00000010];
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
	LPCTSTR errorCaption = "Browser Plugin Extender Error";
	char sysdir[MAX_PATH];

	if (!GetModuleFileName(NULL, sysdir, MAX_PATH)) {
		MessageBox(NULL, "Failed to Get Module Filename", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	
	while (!PathIsDirectory(sysdir)) {
		if (!PathRemoveFileSpec(sysdir)) {
			MessageBox(NULL, "Failed to Remove Path File Spec", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
	}

	wchar_t sysdirFullPathName[MAX_PATH_WIDE];

	// get Module Handle
	HMODULE shockwaveModuleHandle = GetModuleHandle("np32dsw.dll");

	if (shockwaveModuleHandle) {
		if (wcsncpy_s(sysdirShockwave, CA2W(sysdir), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirShockwave, BROWSER_PLUGINS, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirShockwave, SHOCKWAVE, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirShockwave String after Concatenating sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!GetFullPathNameW(sysdirShockwave, MAX_PATH_WIDE - 2, sysdirFullPathName, NULL)) {
			MessageBox(NULL, "Failed to Get sysdirShockwave Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncpy_s(sysdirShockwave, sysdirFullPathName, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirShockwave String after Getting sysdirShockwave Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// test it
		const size_t SHOCKWAVE_TEST_CODE_SIZE = 10;
		unsigned char shockwaveTestCode[SHOCKWAVE_TEST_CODE_SIZE] = { 0x41, 0x00, 0x64, 0x00, 0x6F, 0x00, 0x62, 0x00, 0x65, 0x00 };

		if (!testCode(errorCaption, shockwaveModuleHandle, 0x000198D8, SHOCKWAVE_TEST_CODE_SIZE, shockwaveTestCode)) {
			MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// extend it
		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00003ECE, getSystemDirectoryWExtendedCodeShockwave, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00003ED3)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00003F60, getSystemDirectoryWExtendedCodeShockwave, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00003F65)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00004085, getSystemDirectoryWExtendedCodeShockwave, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x0000408A)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// gets KERNEL32
		/*
		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00004E82, getSystemDirectoryWExtendedCodeShockwave, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x00004E87)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
		*/

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x000057D7, getSystemDirectoryWExtendedCodeShockwave, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, shockwaveModuleHandle, 0x000057DC)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
	}

	HMODULE clickteamModuleHandle = GetModuleHandle("NpCnc32.dll");

	if (clickteamModuleHandle) {
		if (wcsncpy_s(sysdirClickteam, CA2W(sysdir), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirClickteam String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirClickteam, BROWSER_PLUGINS, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirClickteam String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirClickteam, CLICKTEAM, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirClickteam String after Concatenating sysdirClickteam String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!GetFullPathNameW(sysdirClickteam, MAX_PATH_WIDE - 2, sysdirFullPathName, NULL)) {
			MessageBox(NULL, "Failed to Get sysdirClickteam Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncpy_s(sysdirClickteam, sysdirFullPathName, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirClickteam String after Getting sysdirClickteam Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// test it
		const size_t CLICKTEAM_TEST_CODE_SIZE = 10;
		unsigned char clickteamTestCode[CLICKTEAM_TEST_CODE_SIZE] = { 0x83, 0xEC, 0x34, 0x53, 0x55, 0x56, 0x57, 0x50, 0xFF, 0x15 };

		if (!testCode(errorCaption, clickteamModuleHandle, 0x00030225, CLICKTEAM_TEST_CODE_SIZE, clickteamTestCode)) {
			MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// extend it
		if (!extendCode(errorCaption, clickteamModuleHandle, 0x00030416, getSystemDirectoryWExtendedCodeClickteam, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, clickteamModuleHandle, 0x0003041B)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, clickteamModuleHandle, 0x00050657, getSystemDirectoryWExtendedCodeClickteam, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, clickteamModuleHandle, 0x0005065C)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, clickteamModuleHandle, 0x00053F13, getSystemDirectoryWExtendedCodeClickteam, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, clickteamModuleHandle, 0x00053F18)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
	}
	return true;
}