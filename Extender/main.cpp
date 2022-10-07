#include "shared.h"
#include "Extender.h"
#include <windows.h>
#include <atlbase.h>
#include <Shlwapi.h>

const size_t MAX_PATH_WIDE = MAX_PATH * 2;
const char* BROWSER_PLUGINS_BASILISK_PORTABLE = "\\..\\..\\..\\BrowserPlugins\\";
const char* BROWSER_PLUGINS_FLASHPOINT_SECURE_PLAYER = "\\BrowserPlugins\\";
const char* ACTIVEX_FLASHPOINT_SECURE_PLAYER = "\\ActiveX\\";
const wchar_t* SHOCKWAVE = L"Shockwave";
const wchar_t* VITALIZE = L"Vitalize";
const char* PULSE = "Pulse";
wchar_t sysdirShockwave[MAX_PATH_WIDE] = L"";
wchar_t sysdirVitalize[MAX_PATH_WIDE] = L"";
char windirPulse[MAX_PATH] = "";
char programFilesPulse[MAX_PATH] = "";

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

__declspec(naked) void getSystemDirectoryWExtendedCodeVitalize() {
	__asm {
		push eax;
		push[esp + 0x0000000C];
		lea eax, [sysdirVitalize];
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

__declspec(naked) void getWindowsDirectoryAExtendedCodePulse() {
	__asm {
		push eax;
		push[esp + 0x0000000C];
		lea eax, [windirPulse];
		push eax;
		push[esp + 0x00000010];
		call strncpy;
		pop eax;
		pop eax;
		pop eax;
		pop eax;
		retn 0x00000008;
	}
}

__declspec(naked) void shGetFolderPathAExtendedCodePulse() {
	__asm {
		push eax;
		push MAX_PATH;
		lea eax, [programFilesPulse];
		push eax;
		push[esp + 0x00000010];
		call strncpy;
		pop eax;
		pop eax;
		pop eax;
		pop eax;
		retn 0x00000004;
	}
}

bool extender() {
	CHAR root[MAX_PATH] = "";

	if (!GetModuleFileName(NULL, root, MAX_PATH - 1)) {
		showLastError("Failed to Get Module File Name");
		return false;
	}
	
	while (!PathIsDirectory(root)) {
		if (!PathRemoveFileSpec(root)) {
			showLastError("Failed to Remove Path File Spec");
			return false;
		}
	}

	CHAR sysdirFullPathNameA[MAX_PATH] = "";
	WCHAR sysdirFullPathNameW[MAX_PATH_WIDE] = L"";
	CHAR programFilesFullPathNameA[MAX_PATH] = "";

	// get Module Handle
	HMODULE shockwaveModuleHandle = GetModuleHandle("NP32DSW.DLL");

	if (shockwaveModuleHandle) {
		if (wcsncpy_s(sysdirShockwave, CA2W(root), MAX_PATH_WIDE)) {
			showLastError("Failed to Copy String");
			return false;
		}

		if (wcsncat_s(sysdirShockwave, CA2W(BROWSER_PLUGINS_BASILISK_PORTABLE), MAX_PATH_WIDE)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (wcsncat_s(sysdirShockwave, SHOCKWAVE, MAX_PATH_WIDE)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (!GetFullPathNameW(sysdirShockwave, MAX_PATH_WIDE - 2, sysdirFullPathNameW, NULL)) {
			showLastError("Failed to Get Full Path Name");
			return false;
		}

		if (wcsncpy_s(sysdirShockwave, sysdirFullPathNameW, MAX_PATH_WIDE)) {
			showLastError("Failed to Copy String");
			return false;
		}

		// test it
		const VIRTUAL_SIZE SHOCKWAVE_TEST_CODE_SIZE = 10;
		CODE1 shockwaveTestedCode[SHOCKWAVE_TEST_CODE_SIZE] = { 0x41, 0x00, 0x64, 0x00, 0x6F, 0x00, 0x62, 0x00, 0x65, 0x00 };

		if (!testCode(shockwaveModuleHandle, 0x000198D8, SHOCKWAVE_TEST_CODE_SIZE, shockwaveTestedCode)) {
			showLastError("Failed to Test Code");
			return false;
		}

		// extend it
		if (!extendCode(shockwaveModuleHandle, 0x00003ECE, getSystemDirectoryWExtendedCodeShockwave, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x00003ED3)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x00003F60, getSystemDirectoryWExtendedCodeShockwave, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x00003F65)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x00004085, getSystemDirectoryWExtendedCodeShockwave, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x0000408A)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		// gets KERNEL32
		/*
		if (!extendCode(shockwaveModuleHandle, 0x00004E82, getSystemDirectoryWExtendedCodeShockwave, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x00004E87)) {
			showLastError("Failed to Extend Code");
			return false;
		}
		*/

		if (!extendCode(shockwaveModuleHandle, 0x000057D7, getSystemDirectoryWExtendedCodeShockwave, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(shockwaveModuleHandle, 0x000057DC)) {
			showLastError("Failed to Extend Code");
			return false;
		}
	}

	HMODULE vitalizeModuleHandle = GetModuleHandle("NPCNC32.DLL");

	if (vitalizeModuleHandle) {
		if (wcsncpy_s(sysdirVitalize, CA2W(root), MAX_PATH_WIDE)) {
			showLastError("Failed to Copy String");
			return false;
		}

		if (wcsncat_s(sysdirVitalize, CA2W(BROWSER_PLUGINS_BASILISK_PORTABLE), MAX_PATH_WIDE)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (wcsncat_s(sysdirVitalize, VITALIZE, MAX_PATH_WIDE)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (!GetFullPathNameW(sysdirVitalize, MAX_PATH_WIDE - 2, sysdirFullPathNameW, NULL)) {
			showLastError("Failed to Get Full Path Name");
			return false;
		}

		if (wcsncpy_s(sysdirVitalize, sysdirFullPathNameW, MAX_PATH_WIDE)) {
			showLastError("Failed to Copy String");
			return false;
		}

		// test it
		const VIRTUAL_SIZE VITALIZE_TEST_CODE_SIZE = 10;
		CODE1 vitalizeTestedCode[VITALIZE_TEST_CODE_SIZE] = { 0x83, 0xEC, 0x34, 0x53, 0x55, 0x56, 0x57, 0x50, 0xFF, 0x15 };

		if (!testCode(vitalizeModuleHandle, 0x00030225, VITALIZE_TEST_CODE_SIZE, vitalizeTestedCode)) {
			showLastError("Failed to Test Code");
			return false;
		}

		// extend it
		if (!extendCode(vitalizeModuleHandle, 0x00030416, getSystemDirectoryWExtendedCodeVitalize, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(vitalizeModuleHandle, 0x0003041B)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(vitalizeModuleHandle, 0x00050657, getSystemDirectoryWExtendedCodeVitalize, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(vitalizeModuleHandle, 0x0005065C)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(vitalizeModuleHandle, 0x00053F13, getSystemDirectoryWExtendedCodeVitalize, true)) {
			showLastError("Failed to Extend Code");
			return false;
		}

		if (!extendCode(vitalizeModuleHandle, 0x00053F18)) {
			showLastError("Failed to Extend Code");
			return false;
		}
	}

	HMODULE pulseModuleHandle = GetModuleHandle("AXPULSE.DLL");
	HMODULE pulse5ModuleHandle = GetModuleHandle("AXPULSE5.DLL");

	if (pulseModuleHandle || pulse5ModuleHandle) {
		if (strncpy_s(windirPulse, root, MAX_PATH)) {
			showLastError("Failed to Copy String");
			return false;
		}

		if (strncat_s(windirPulse, BROWSER_PLUGINS_FLASHPOINT_SECURE_PLAYER, MAX_PATH)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (strncat_s(windirPulse, PULSE, MAX_PATH)) {
			showLastError("Failed to Concatenate String");
			return false;
		}

		if (!GetFullPathNameA(windirPulse, MAX_PATH - 1, sysdirFullPathNameA, NULL)) {
			showLastError("Failed to Get Full Path Name");
			return false;
		}

		if (strncpy_s(windirPulse, sysdirFullPathNameA, MAX_PATH)) {
			showLastError("Failed to Copy String");
			return false;
		}

		// test it
		const VIRTUAL_SIZE PULSE_TEST_CODE_SIZE = 20;
		CODE1 pulseTestedCode[PULSE_TEST_CODE_SIZE] = { 0x50, 0x75, 0x6C, 0x73, 0x65, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x49, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x00 };

		if (pulseModuleHandle) {
			if (!testCode(pulseModuleHandle, 0x0001BE88, PULSE_TEST_CODE_SIZE, pulseTestedCode)) {
				showLastError("Failed to Test Code");
				return false;
			}

			// extend it
			if (!extendCode(pulseModuleHandle, 0x00003D6A, getWindowsDirectoryAExtendedCodePulse, true)) {
				showLastError("Failed to Extend Code");
				return false;
			}

			if (!extendCode(pulseModuleHandle, 0x00003D6F)) {
				showLastError("Failed to Extend Code");
				return false;
			}
		}

		if (pulse5ModuleHandle) {
			if (strncpy_s(programFilesPulse, root, MAX_PATH)) {
				showLastError("Failed to Copy String");
				return false;
			}

			if (strncat_s(programFilesPulse, ACTIVEX_FLASHPOINT_SECURE_PLAYER, MAX_PATH)) {
				showLastError("Failed to Concatenate String");
				return false;
			}

			/*
			if (strncat_s(programFilesPulse, PULSE, MAX_PATH)) {
				showLastError("Failed to Concatenate String");
				return false;
			}
			*/

			if (!GetFullPathNameA(programFilesPulse, MAX_PATH - 1, programFilesFullPathNameA, NULL)) {
				showLastError("Failed to Get Full Path Name");
				return false;
			}

			if (strncpy_s(programFilesPulse, programFilesFullPathNameA, MAX_PATH)) {
				showLastError("Failed to Copy String");
				return false;
			}

			if (!testCode(pulse5ModuleHandle, 0x000124F8, PULSE_TEST_CODE_SIZE, pulseTestedCode)) {
				showLastError("Failed to Test Code");
				return false;
			}

			// extend it
			if (!extendCode(pulse5ModuleHandle, 0x0000A5A5, getWindowsDirectoryAExtendedCodePulse, true)) {
				showLastError("Failed to Extend Code");
				return false;
			}

			if (!extendCode(pulse5ModuleHandle, 0x0000A5AA)) {
				showLastError("Failed to Extend Code");
				return false;
			}

			if (!extendCode(pulse5ModuleHandle, 0x0000A67A, shGetFolderPathAExtendedCodePulse, true)) {
				showLastError("Failed to Extend Code");
				return false;
			}

			for (DWORD i = 0; i < 5; i++) {
				if (!extendCode(pulse5ModuleHandle, 0x0000A67F + i)) {
					showLastError("Failed to Extend Code");
					return false;
				}
			}
		}
	}
	return true;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		if (!DisableThreadLibraryCalls(instance)) {
			showLastError("Failed to Disable Thread Library Calls");
			return FALSE;
		}

		if (!extender()) {
			terminateCurrentProcess();
			return FALSE;
		}
	}
	return TRUE;
}