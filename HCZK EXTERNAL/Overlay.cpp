#include "Overlay.h"

Overlay::Overlay() {
	m_hWnd = NULL;
	m_hGame = NULL;

	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct3D interface
	m_pDevice = nullptr;

	ZeroMemory(&m_Present, sizeof(m_Present));
}

Overlay::~Overlay() {
	if (m_pDevice) {
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_pDirect3D) {
		m_pDirect3D->Release();
		m_pDirect3D = nullptr;
	}

}

int Overlay::OnFrame() {
	MSG msg;
	::SetWindowPos(FindWindow(NULL, hwndName), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	while (TRUE)
	{
		 
		if (!FindWindow(NULL, hwndName))
			ExitProcess(0);

		UpdateWindowPos();
		Render();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);

	} //end while

	return msg.wParam;
}

void Overlay::AddOnFrame(const OnFrameFn& pFunction) {
	m_pOnFrameList.push_back(pFunction);
}

void Overlay::UpdateWindowPos() {
	RECT rectGame;
	GetWindowRect(m_hGame, &rectGame);
	int w = rectGame.left;
	int h = rectGame.top;

	m_nSize[0] = rectGame.right - rectGame.left;
	m_nSize[1] = rectGame.bottom - rectGame.top;

	MoveWindow(m_hWnd, w, h, m_nSize[0], m_nSize[1], TRUE);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Overlay::Render() {
	// clear the window alpha
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	m_pDevice->BeginScene();    // begins the 3D scene

	//DrawString(10, 50, D3DCOLOR_ARGB(255, 255, 0, 0), "Test rendering string :D");

	if (!m_pOnFrameList.empty()) {
		for (auto& pOnFrame : m_pOnFrameList) {
			if (pOnFrame) {
				pOnFrame();
			}
		}
	}

	m_pDevice->EndScene();    // ends the 3D scene
	m_pDevice->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

bool Overlay::Attach(HWND hWnd) {
	m_hGame = hWnd;

	if (!m_hGame) {
		return false;
	}

	RECT r_client;

	//GetClientRect(m_newHwnd, &r_client);
	GetWindowRect(m_hGame, &r_client);

	m_nSize[0] = r_client.right - r_client.left;
	m_nSize[1] = r_client.bottom - r_client.top;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL); //keyboard input
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc); //check here still

	m_hWnd = CreateWindowEx(0,
		"WindowClass",
		"",
		WS_EX_TOPMOST | WS_POPUP,
		r_client.left, r_client.top,
		m_nSize[0], m_nSize[1],
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	SetWindowLong(m_hWnd, GWL_EXSTYLE, (int)GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA);

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);

	bool ret = InitDirectX();
	bool kret = InitKeyboard();

	return 1;
}

int* Overlay::getSize() {
	return m_nSize;
}

void Overlay::GetScreenSize(int* width, int* height) {
	if (width) {
		*width = m_nSize[0];
	}

	if (height) {
		*height = m_nSize[1];
	}
}

LPDIRECT3DDEVICE9 Overlay::GetDevice() const {
	return m_pDevice;
}

bool Overlay::InitKeyboard() {
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pdxKeybObj, NULL);
	m_pdxKeybObj->CreateDevice(GUID_SysKeyboard, &m_pdxKeybDevice, NULL);

	m_pdxKeybDevice->SetDataFormat(&c_dfDIKeyboard);
	m_pdxKeybDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pdxKeybDevice->Acquire();

	return true;
}

bool Overlay::InitDirectX() {
	m_Present.Windowed = TRUE;    // program windowed, not fullscreen
	m_Present.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	m_Present.hDeviceWindow = m_hWnd;    // set the window to be used by Direct3D
	m_Present.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
	m_Present.BackBufferWidth = m_nSize[0];    // set the width of the buffer
	m_Present.BackBufferHeight = m_nSize[1];    // set the height of the buffer

	m_Present.EnableAutoDepthStencil = TRUE;
	m_Present.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the m_Present stuct
	m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd, //handle to the window Direct3D should use.
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&m_Present,
		&m_pDevice);

	return true;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		DwmExtendFrameIntoClientArea(hWnd, &margin);
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}