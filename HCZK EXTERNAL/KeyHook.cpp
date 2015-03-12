#include "KeyHook.h"

HHOOK m_keyHook;

KeyHook::KeyHook()
{
	m_keyHook = NULL;
}

KeyHook::~KeyHook()
{
	UnhookWindowsHookEx(m_keyHook);
	m_keyHook = NULL;
}

void KeyHook::Init() {
	m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, NULL);
	ShowCities = true;
}

void KeyHook::DoIt() {
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

	PKBDLLHOOKSTRUCT keyp = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_SYSKEYUP:
			if (keyp->vkCode == VK_F1 && keyp->flags & LLKHF_ALTDOWN) {
				std::cout << "alt f1" << std::endl;
				ShowCities = !ShowCities;
				std::cout << ShowCities << std::endl;
			}
			else if (keyp->vkCode == VK_F2 && keyp->flags & LLKHF_ALTDOWN) {
				std::cout << "alt f2" << std::endl;
			}

		default:
			break;
		}
	}

	return CallNextHookEx(m_keyHook, nCode, wParam, lParam); //send me the next Message so that we do not cause Error 
}