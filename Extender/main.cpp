#include <windows.h>
#include <atlbase.h>
#include <Shlwapi.h>
#include "main.h"

const size_t MAX_PATH_WIDE = MAX_PATH * 2;
const char* BROWSER_PLUGINS_BASILISK_PORTABLE = "\\..\\..\\..\\BrowserPlugins\\";
const char* BROWSER_PLUGINS_FLASHPOINT_SECURE_PLAYER = "\\BrowserPlugins\\";
const char* ACTIVEX_FLASHPOINT_SECURE_PLAYER = "\\ActiveX\\";
const wchar_t* SHOCKWAVE = L"Shockwave";
const wchar_t* VITALIZE = L"Vitalize";
const char* PULSE = "Pulse";
wchar_t sysdirShockwave[MAX_PATH_WIDE];
wchar_t sysdirVitalize[MAX_PATH_WIDE];
char windirPulse[MAX_PATH];
char programFilesPulse[MAX_PATH];

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
	// set this to your Error Caption
	LPCTSTR errorCaption = "Browser Plugin Extender Error";
	char root[MAX_PATH];

	if (!GetModuleFileName(NULL, root, MAX_PATH)) {
		MessageBox(NULL, "Failed to Get Module Filename", errorCaption, MB_OK | MB_ICONERROR);
		return false;
	}
	
	while (!PathIsDirectory(root)) {
		if (!PathRemoveFileSpec(root)) {
			MessageBox(NULL, "Failed to Remove Path File Spec", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
	}

	char sysdirFullPathNameA[MAX_PATH];
	wchar_t sysdirFullPathNameW[MAX_PATH_WIDE];
	char programFilesFullPathNameA[MAX_PATH];

	// get Module Handle
	HMODULE shockwaveModuleHandle = GetModuleHandle("np32dsw.dll");

	if (shockwaveModuleHandle) {
		if (wcsncpy_s(sysdirShockwave, CA2W(root), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirShockwave, CA2W(BROWSER_PLUGINS_BASILISK_PORTABLE), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirShockwave, SHOCKWAVE, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirShockwave String after Concatenating sysdirShockwave String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!GetFullPathNameW(sysdirShockwave, MAX_PATH_WIDE - 2, sysdirFullPathNameW, NULL)) {
			MessageBox(NULL, "Failed to Get sysdirShockwave Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncpy_s(sysdirShockwave, sysdirFullPathNameW, MAX_PATH_WIDE)) {
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

	HMODULE vitalizeModuleHandle = GetModuleHandle("NpCnc32.dll");

	if (vitalizeModuleHandle) {
		if (wcsncpy_s(sysdirVitalize, CA2W(root), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirVitalize String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirVitalize, CA2W(BROWSER_PLUGINS_BASILISK_PORTABLE), MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirVitalize String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncat_s(sysdirVitalize, VITALIZE, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Concatenate sysdirVitalize String after Concatenating sysdirVitalize String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!GetFullPathNameW(sysdirVitalize, MAX_PATH_WIDE - 2, sysdirFullPathNameW, NULL)) {
			MessageBox(NULL, "Failed to Get sysdirVitalize Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (wcsncpy_s(sysdirVitalize, sysdirFullPathNameW, MAX_PATH_WIDE)) {
			MessageBox(NULL, "Failed to Copy sysdirVitalize String after Getting sysdirVitalize Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// test it
		const size_t VITALIZE_TEST_CODE_SIZE = 10;
		unsigned char vitalizeTestCode[VITALIZE_TEST_CODE_SIZE] = { 0x83, 0xEC, 0x34, 0x53, 0x55, 0x56, 0x57, 0x50, 0xFF, 0x15 };

		if (!testCode(errorCaption, vitalizeModuleHandle, 0x00030225, VITALIZE_TEST_CODE_SIZE, vitalizeTestCode)) {
			MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// extend it
		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x00030416, getSystemDirectoryWExtendedCodeVitalize, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x0003041B)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x00050657, getSystemDirectoryWExtendedCodeVitalize, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x0005065C)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x00053F13, getSystemDirectoryWExtendedCodeVitalize, true)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!extendCode(errorCaption, vitalizeModuleHandle, 0x00053F18)) {
			MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}
	}

	HMODULE pulseModuleHandle = GetModuleHandle("AxPulse.dll");
	HMODULE pulse5ModuleHandle = GetModuleHandle("AxPulse5.dll");

	if (pulseModuleHandle || pulse5ModuleHandle) {
		if (strncpy_s(windirPulse, root, MAX_PATH)) {
			MessageBox(NULL, "Failed to Copy windirPulse String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (strncat_s(windirPulse, BROWSER_PLUGINS_FLASHPOINT_SECURE_PLAYER, MAX_PATH)) {
			MessageBox(NULL, "Failed to Concatenate windirPulse String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (strncat_s(windirPulse, PULSE, MAX_PATH)) {
			MessageBox(NULL, "Failed to Concatenate windirPulse String after Concatenating windirPulse String", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!GetFullPathNameA(windirPulse, MAX_PATH - 1, sysdirFullPathNameA, NULL)) {
			MessageBox(NULL, "Failed to Get windirPulse Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		if (strncpy_s(windirPulse, sysdirFullPathNameA, MAX_PATH)) {
			MessageBox(NULL, "Failed to Copy windirPulse String after Getting windirPulse Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
			return false;
		}

		// test it
		const size_t PULSE_TEST_CODE_SIZE = 20;
		unsigned char pulseTestCode[PULSE_TEST_CODE_SIZE] = { 0x50, 0x75, 0x6C, 0x73, 0x65, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x49, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x00 };

		if (pulseModuleHandle) {
			if (!testCode(errorCaption, pulseModuleHandle, 0x0001BE88, PULSE_TEST_CODE_SIZE, pulseTestCode)) {
				MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			// extend it
			if (!extendCode(errorCaption, pulseModuleHandle, 0x00003D6A, getWindowsDirectoryAExtendedCodePulse, true)) {
				MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (!extendCode(errorCaption, pulseModuleHandle, 0x00003D6F)) {
				MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}
		}

		if (pulse5ModuleHandle) {
			if (strncpy_s(programFilesPulse, root, MAX_PATH)) {
				MessageBox(NULL, "Failed to Copy programFilesPulse String", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (strncat_s(programFilesPulse, ACTIVEX_FLASHPOINT_SECURE_PLAYER, MAX_PATH)) {
				MessageBox(NULL, "Failed to Concatenate programFilesPulse String", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			/*
			if (strncat_s(programFilesPulse, PULSE, MAX_PATH)) {
				MessageBox(NULL, "Failed to Concatenate programFilesPulse String after Concatenating programFilesPulse String", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}
			*/

			if (!GetFullPathNameA(programFilesPulse, MAX_PATH - 1, programFilesFullPathNameA, NULL)) {
				MessageBox(NULL, "Failed to Get programFilesPulse Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (strncpy_s(programFilesPulse, programFilesFullPathNameA, MAX_PATH)) {
				MessageBox(NULL, "Failed to Copy programFilesPulse String after Getting programFilesPulse Full Path Name", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (!testCode(errorCaption, pulse5ModuleHandle, 0x000124F8, PULSE_TEST_CODE_SIZE, pulseTestCode)) {
				MessageBox(NULL, "Failed to Test Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			// extend it
			if (!extendCode(errorCaption, pulse5ModuleHandle, 0x0000A5A5, getWindowsDirectoryAExtendedCodePulse, true)) {
				MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (!extendCode(errorCaption, pulse5ModuleHandle, 0x0000A5AA)) {
				MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			if (!extendCode(errorCaption, pulse5ModuleHandle, 0x0000A67A, shGetFolderPathAExtendedCodePulse, true)) {
				MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
				return false;
			}

			for (DWORD i = 0; i < 5; i++) {
				if (!extendCode(errorCaption, pulse5ModuleHandle, 0x0000A67F + i)) {
					MessageBox(NULL, "Failed to Extend Code", errorCaption, MB_OK | MB_ICONERROR);
					return false;
				}
			}
		}
	}
	return true;
}