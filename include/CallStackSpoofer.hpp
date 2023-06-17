#ifndef CALLSTACKSPOOFER_HPP
#define CALLSTACKSPOOFER_HPP

namespace CallStackSpoofer {

	/*
	 * The call stack spoofer expects this to be set
	 * This has to be assembly code which starts with the following:
	 * jmp *%JMP_REGISTER
	 */
	extern void* jmpReg;

	struct Data {
		void* origReg;
		void* retAddr;
	};

	thread_local extern Data data;
	thread_local extern void* methodAddress;

	void SpoofCallStack();

	template <typename Ret, typename... Args>
	__attribute((noinline, optimize("O0"))) static Ret Invoke(void* method, Args... args)
	{
		data.retAddr = __builtin_extract_return_addr(__builtin_return_address(0));
		methodAddress = method;
		return reinterpret_cast<Ret (*)(Args...)>(SpoofCallStack)(args...);
	}
}

#endif
