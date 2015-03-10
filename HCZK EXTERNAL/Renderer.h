#ifndef RENDERER_H
#define RENDERER_H

#pragma once

#include "Color.h"

struct Vertex_t {
	XMFLOAT4 xyzrhw;
	D3DCOLOR color;

	enum {
		FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
	};
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void						PreFrame();
	void						PostFrame();

	void						OnSetup(LPDIRECT3DDEVICE9 pDevice);

	void						CreateFont(int h, int w, char* font);
	void						DrawRect(int x, int y, int w, int h, Color color);
	void						DrawString(int x, int y, Color color, const char* text);

private:
	std::vector<LPD3DXFONT>		m_pFontList;
	LPD3DXFONT					m_mFont;
	LPDIRECT3DDEVICE9			m_pDevice;		// pointer to the device class
};

inline void BuildVertex(XMFLOAT4 xyzrhw, D3DCOLOR color, Vertex_t* vertexList, int index) {
	vertexList[index].xyzrhw = xyzrhw;
	vertexList[index].color = color;
}

#endif // RENDERER_H