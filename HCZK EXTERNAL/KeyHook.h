#pragma once

#include "Main.h"
#include "SDK.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class KeyHook
{
public:
	KeyHook();
	~KeyHook();

	void		Init();
	void		DoIt();
};

