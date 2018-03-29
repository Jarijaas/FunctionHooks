# FunctionHooks

Header-only C++ library for hooking functions on Windows x64. Supports inline and virtual table hooks.

## Getting Started

Include "function_hooks.h" in your project.

Inlike hook example:
```C++
#include <function_hooks.h>

// Function that is called when hooked function is called
void Hook(fn_hooks::Registers regs) {

}

int main() {

  // To hook
  const size_t epilog_size = 0x1A;
  fn_hooks::InlineHook(function_ptr, Hook, epilog_size);

  // To unhook
  fn_hooks::Unhook(function_ptr);
}
```

## Running the tests

Tests are located in **FunctionHooksTest**.
Tests can be run with Visual Studio. See [Run unit tests with Test Explorer](https://docs.microsoft.com/en-us/visualstudio/test/run-unit-tests-with-test-explorer)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
