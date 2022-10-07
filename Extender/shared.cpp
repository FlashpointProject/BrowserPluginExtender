#include "shared.h"
#include <string>
#include <sstream>
#include <windows.h>

bool shiftMemory(size_t bufferSize, void* buffer, size_t sourceSize, void* source, size_t shift, bool direction) {
	if (source < buffer || (char*)source + sourceSize >= (char*)buffer + bufferSize) {
		return false;
	}

	size_t destinationSize = (char*)buffer + bufferSize - source;
	char* destination = (char*)source;

	if (direction) {
		destination += shift;
	} else {
		destination -= shift;
	}

	if (destination < buffer || destination + destinationSize >= (char*)buffer + bufferSize) {
		return false;
	}
	return !memmove_s(destination, destinationSize, source, sourceSize);
}

static LPCSTR MESSAGE_BOX_CAPTION = "Extender Error";

bool showLastError(LPCSTR errorMessage) {
	if (!errorMessage) {
		return false;
	}

	std::ostringstream oStringStream;
	oStringStream << errorMessage << " (" << GetLastError() << ")";

	if (!MessageBox(NULL, oStringStream.str().c_str(), MESSAGE_BOX_CAPTION, MB_OK | MB_ICONERROR)) {
		return false;
	}
	return true;
}

BOOL terminateCurrentProcess() {
	return TerminateProcess(GetCurrentProcess(), GetLastError());
}