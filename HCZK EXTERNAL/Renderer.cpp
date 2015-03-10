
#include "Renderer.h"


Renderer::Renderer()
{
	m_pDevice = nullptr;
	m_mFont = nullptr;
}


Renderer::~Renderer()
{
	if (m_pDevice) {
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_mFont) {
		m_mFont->Release();
		m_mFont = nullptr;
	}
}

void Renderer::PreFrame() {
	m_pDevice->SetFVF(Vertex_t::FVF);
}
void Renderer::PostFrame() {

}

void Renderer::OnSetup(LPDIRECT3DDEVICE9 pDevice) {
	m_pDevice = pDevice;
	D3DXCreateFont(m_pDevice, 16, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_mFont);
}

void Renderer::DrawRect(int x, int y, int w, int h, Color color) {
	Vertex_t vertexList[4];

	BuildVertex(XMFLOAT4(x, y + h, 0, 1), color.Code(), vertexList, 0);
	BuildVertex(XMFLOAT4(x, y, 0, 1), color.Code(), vertexList, 1);
	BuildVertex(XMFLOAT4(x + w, y + h, 0, 1), color.Code(), vertexList, 2);
	BuildVertex(XMFLOAT4(x + w, y, 0, 1), color.Code(), vertexList, 3);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexList, sizeof(Vertex_t));
}
void Renderer::DrawString(int x, int y, Color color, const char* text) {
	RECT FontPos = { x, y, x + 120, y + 16 };
	char buf[1024] = { '\0' };

	sprintf(buf, text);

	m_mFont->DrawText(NULL, buf, -1, &FontPos, DT_NOCLIP, color.Code());
}

void Renderer::CreateFont(int h, int w, char* font) {
	LPD3DXFONT tFont;
	D3DXCreateFont(m_pDevice, 50, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font, &tFont);
	m_pFontList.push_back(tFont);
}
