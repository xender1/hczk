#ifndef OVERLAY_H
#define OVERLAY_H

#pragma once

#include "Main.h"
#include "Direct3D.h"

const MARGINS  margin = { 0, 0, 800, 600 };
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#ifndef OVERLAY_NAME
#define OVERLAY_NAME "Overlay001"
#endif // OVERLAY_NAME

typedef void(*OnFrameFn)();

class Overlay {
public:
	Overlay();
	~Overlay();

	bool					Attach(HWND hWnd); // Attaches hWnd into object to set up control

	int						OnFrame(); //gameloop


	void					AddOnFrame(const OnFrameFn& pFunction);


	void					GetScreenSize(int* width, int* height);
	LPDIRECT3DDEVICE9		GetDevice() const;

	int*					getSize();

private:
	void					Render();
 
	void					UpdateWindowPos();

private:
	bool					InitDirectX();
	bool					InitKeyboard();

private:

	HWND					m_hGame; //hold on target window
	HWND					m_hWnd; //ours
	int						m_nSize[2]; //[0]width, [1]height

	LPDIRECT3D9				m_pDirect3D;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9		m_pDevice;		// pointer to the device class
	D3DPRESENT_PARAMETERS	m_Present;		// struct to hold various device information

	LPDIRECTINPUT8 			m_pdxKeybObj;
	LPDIRECTINPUTDEVICE8 	m_pdxKeybDevice; //keyboard

	std::vector<OnFrameFn>	m_pOnFrameList;

};

extern Overlay* gOverlay;

#endif // OVERLAY_H