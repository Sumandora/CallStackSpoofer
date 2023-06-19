# Call Stack Spoofer
This is a thread-safe call stack spoofer for x86-64 linux systems using the SystemV ABI.

## Usage
An example can be found in the Example folder.  

A minimal example could look like this:
```c++
#include "CallStackSpoofer.hpp"

void TestFunction(int arg1) {
	// ...
}

void* CallStackSpoofer::jmpReg = returnGadget;

int main() {
	CallStackSpoofer::Invoke<void, int>(reinterpret_cast<void*>(TestFunction), 1337);
}
```

## Compiling the example
```bash
mkdir Build && cd Build

cmake ..
make
./CallStackSpoofer
```

## How it works
The call stack spoofer works by saving the actual return address in a function preserved register and setting the return address to a `jmp register` gadget.

## Caveats
The SystemV ABI does not specify how function preserved registers are used. This means it's perfectly fine to access the register in a called function. This can cause problems when [hooking functions](https://en.wikipedia.org/wiki/Hooking) whose caller has set the register and the included callee is reading it.  
Another issue are the thread local variables. These may interfere with already existing thread local storage.

## Compatibility
It is compatible with 64-bit GCC  
Clang is not supported due to its unpredictable machine code generation
