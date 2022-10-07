#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>

typedef DWORD VIRTUAL_ADDRESS;
typedef DWORD VIRTUAL_SIZE;
typedef DWORD RELATIVE_VIRTUAL_ADDRESS;

typedef DWORD EXTENDED_CODE_ADDRESS;

typedef BYTE CODE1;
typedef WORD CODE2;

inline size_t stringSize(const char* string) {
	return strlen(string) + 1;
}

inline bool stringsEqual(const char* leftHandSide, const char* rightHandSide) {
	return !strcmp(leftHandSide, rightHandSide);
}

inline bool memoryEqual(const void* buffer, const void* buffer2, size_t bufferSize) {
	return !memcmp(buffer, buffer2, bufferSize);
}

bool shiftMemory(size_t bufferSize, void* buffer, size_t sourceSize, void* source, size_t shift, bool direction);

bool showLastError(LPCSTR errorMessage);
BOOL terminateCurrentProcess();