#include "CallStackSpoofer.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <execinfo.h>

void __attribute((naked)) ReturnGadget()
{
	__asm volatile("jmp *%" JMP_REGISTER ";");
}

void* CallStackSpoofer::jmpReg = reinterpret_cast<void*>(ReturnGadget);

const int argumentLength = 1000 * 1000 * 8; // approximate the maximum that the stack can handle

void GenerateBacktrace()
{
	constexpr int bufSize = 100;
	int nptrs;
	void* buffer[bufSize];
	char** strings;

	nptrs = backtrace(buffer, bufSize);
	printf("backtrace() returned %d addresses\n", nptrs);

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		free(strings);
		return;
	}

	for (size_t j = 0; j < nptrs; j++)
		printf("%s\n", strings[j]);

	free(strings);
}

size_t TestFunction(const char str[argumentLength])
{
	GenerateBacktrace();
	assert(__builtin_extract_return_addr(__builtin_return_address(0)) == CallStackSpoofer::jmpReg);
	return strlen(str);
}

int main()
{
	char str[argumentLength + 1];
	for (char& i : str)
		i = 'A';
	str[argumentLength] = '\0';

	int length = CallStackSpoofer::Invoke<int, const char[]>(reinterpret_cast<void*>(TestFunction), str);

	assert(length == argumentLength);

	printf("String length was: %d\n", length);
}
