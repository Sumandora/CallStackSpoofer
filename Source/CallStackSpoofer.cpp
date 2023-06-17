#include "CallStackSpoofer.hpp"

thread_local CallStackSpoofer::Data CallStackSpoofer::data;
thread_local void* CallStackSpoofer::methodAddress;

static __attribute((naked, noinline)) void Backup()
{
	__asm(
		"mov %%" JMP_REGISTER ", %0;"
		: "=a"(CallStackSpoofer::data.origReg));
	__asm("ret;");
}

static __attribute((naked, noinline)) void Restore()
{
	__asm(
		"leave;"
		"pop %" JMP_REGISTER ";" // Fix call stack by removing the invalid address
		"push %rax;"); // Save rax, because GCC is special and overrides it sometimes
	__asm("mov %0,%%" JMP_REGISTER ";" // Restore register
		  :
		  : "D"(CallStackSpoofer::data.origReg));
	__asm("mov %0, %%rdi;"
		  :
		  : "D"(CallStackSpoofer::data.retAddr));
	__asm("pop %rax;"
		  "jmp *%rdi;"); // Jump to the return address
}

void __attribute((naked, noinline)) CallStackSpoofer::SpoofCallStack()
{
	__asm("call *%0;"
		  :
		  : "r"(&Backup));
	__asm("mov %0,%%" JMP_REGISTER ";"
		  :
		  : "r"(&Restore));
	__asm( // Destroy call stack
		"lea 0x0,%rax;"
		"push %rax;");
	__asm("push %0;"
		  :
		  : "m"(CallStackSpoofer::jmpReg));
	__asm(
		"jmp *%0;"
		:
		: "m"(methodAddress));
}