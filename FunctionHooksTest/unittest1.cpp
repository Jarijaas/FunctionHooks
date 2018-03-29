/*
Copyright 2018 Jari J‰‰skel‰

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\FunctionHooks\include\function_hooks.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


int HookedFunction() {
	int x = 5;
	int y = 10;
	return x + y;
}


bool hook_success = false;

void Hook(fn_hooks::Registers regs) {
	hook_success = true;
}

bool HookVTable() {
	return true;
}

class BaseClass {
public:

	virtual bool HookedFunction() { return false; };
};

class TargetClass : BaseClass {
public:

	bool HookedFunction() {
		return false;
	}
};


namespace FunctionHooksTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestInlineHook)
		{
			hook_success = false;

			const uint64_t fn_ptr = fn_hooks::ReadJump(reinterpret_cast<uint64_t>(HookedFunction));

			// Test inline hook
			Assert::IsTrue(fn_hooks::InlineHook(fn_ptr, Hook, 0x1A));
			HookedFunction();
			Assert::IsTrue(hook_success);

			// Test unhook
			hook_success = false;
			Assert::IsTrue(fn_hooks::Unhook(fn_ptr));
			HookedFunction();
			Assert::IsFalse(hook_success);
		}

		TEST_METHOD(TestVTableHook)
		{
			TargetClass tc;

			uint64_t vtable = *reinterpret_cast<uint64_t*>(&tc);
			uint64_t fn_ptr = *reinterpret_cast<uint64_t*>(vtable);

			// Test VTable hook
			Assert::IsTrue(fn_hooks::VTableHook<bool(void)>(vtable, HookVTable));
			Assert::IsTrue((&tc)->HookedFunction());

			// Test unhook
			Assert::IsTrue(fn_hooks::Unhook(vtable));
			Assert::IsFalse((&tc)->HookedFunction());
		}
	};
}