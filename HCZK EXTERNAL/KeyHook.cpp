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
	//std::cout << "que" << std::endl;
	PKBDLLHOOKSTRUCT keyp = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_SYSKEYDOWN:
			//std::cout << "Hi" << std::endl;
			if (!ShowDisplaySettings) { ShowDisplaySettings = true; }
			break;

		case WM_SYSKEYUP:
			if (keyp->vkCode == VK_F1 && keyp->flags & LLKHF_ALTDOWN) {
				ShowBorderBox = !ShowBorderBox;
			}
			else if (keyp->vkCode == VK_F2 && keyp->flags & LLKHF_ALTDOWN) {
				ShowItems = !ShowItems;
			}
			else if (keyp->vkCode == VK_F3 && keyp->flags & LLKHF_ALTDOWN) {
				ShowAnimals = !ShowAnimals;
			}
			else if (keyp->vkCode == VK_F5 && keyp->flags & LLKHF_ALTDOWN) {
				ShowAggressive = !ShowAggressive;
			}
			else if (keyp->vkCode == VK_F6 && keyp->flags & LLKHF_ALTDOWN) {
				ShowWepAmmo = !ShowWepAmmo;
			}
			else if (keyp->vkCode == VK_F7 && keyp->flags & LLKHF_ALTDOWN) {
				ShowPlayers = !ShowPlayers;
			}
			else if (keyp->vkCode == VK_F8 && keyp->flags & LLKHF_ALTDOWN) {
				ShowCities = !ShowCities;
			}
			else if (keyp->vkCode == VK_F9 && keyp->flags & LLKHF_ALTDOWN) {
				ShowToggleSettings = !ShowToggleSettings;
			}
			break;

		case WM_KEYUP:
			//std::cout << "key up" << std::endl;
			ShowDisplaySettings = false;
			break;

		default:
			break;
		}
	}

	return CallNextHookEx(m_keyHook, nCode, wParam, lParam); //send me the next Message so that we do not cause Error 
}