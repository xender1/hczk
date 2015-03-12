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
	switch (wParam) {
	case WM_KEYUP:
		switch (keyp->vkCode) {
		case 0x70: //f1
			std::cout << "f1" << std::endl;
			break;
		case 0x71: //f2
			std::cout << "f2" << std::endl;
			break;
		default:
			std::cout << "other" << std::endl;
		}
		break;
	case WM_KEYDOWN:
		std::cout << "down" << std::endl;
		break;

	default:
		break;
	}
	return CallNextHookEx(m_keyHook, nCode, wParam, lParam); //send me the next Message so that we do not cause Error 
}