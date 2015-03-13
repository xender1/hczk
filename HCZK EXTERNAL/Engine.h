#ifndef ENGINE_H
#define ENGINE_H

#pragma once

#include "Math.h"

class Engine {
public:
	Engine();
	~Engine();

	bool	WorldToScreen(const Vector3& vIn, Vector3& vOut);

public:
	int* m_ScreenSize; //width height ref to overlay m_size
};

extern Engine* g_pEngine;

#endif // ENGINE_H 