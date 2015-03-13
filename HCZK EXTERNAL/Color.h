#ifndef COLOR_H
#define COLOR_H

#pragma once

#include "Main.h"
#include "Direct3D.h"

class Color {
public:
	Color() {
		m_r = m_g = m_b = m_a = m_code = 0;
	}

	Color(BYTE r, BYTE g, BYTE b, BYTE a = 255) {
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
		m_code = D3DCOLOR_RGBA(r, g, b, a);
		//D3DCOLOR_ARGB 4 params
	}

	Color(D3DCOLOR code) {
		m_code = code;
	}

	BYTE R() const { return m_r; }
	BYTE G() const { return m_g; }
	BYTE B() const { return m_b; }
	BYTE A() const { return m_a; }

	D3DCOLOR Code() const { return m_code; }

	static Color None() { return Color(0, 0, 0, 0); }
	static Color White() { return Color(255, 255, 255); }
	static Color Black() { return Color(0, 0, 0); }
	static Color Red() { return Color(255, 0, 0); }
	static Color DarkRed() { return Color(117, 0, 0); }
	static Color Green() { return Color(0, 255, 0); }
	static Color Blue() { return Color(0, 0, 255); }
	static Color Cyan() { return Color(0, 128, 255); }
	static Color Yellow() { return Color(255, 255, 0); }
	static Color Pink() { return Color(242, 12, 223); }
	static Color DarkPink() { return Color(133, 42, 130); }
	static Color LightBrown() { return Color(168, 66, 35); }
	static Color DarkGreen() { return Color(31, 143, 31); }

private:
	BYTE		m_r, m_g, m_b, m_a;
	D3DCOLOR	m_code;
};

#endif // COLOR_H
