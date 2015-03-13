#ifndef SDK_H
#define SDK_H

#pragma once

#include "Math.h"

#define GAME_CLIENT_ADDRESS	0x142ADA390
#define GRAPHICS_ADDRESS	0x142ADA0E8

enum Entity_t {
	ENTITY_ORIGIN = 0x1D0,
	ENTITY_VELOCITY = 0x200,
	ENTITY_NEXT = 0x400,
	ENTITY_NAME = 0x470,
	ENTITY_TYPE = 0x5B0,
};

enum EntityType_t {
	ENTITY_PLAYER = 4
};

enum Graphics_t {
	GRAPHICS_CAMERA = 0x48L,
	GRAPHICS_CAMERA_MATRIX = 0x20L,
	GRAPHICS_MATRIX_PAD = 0x10L,
	GRAPHICS_SCREEN_WIDTH = 0x28,
	GRAPHICS_SCREEN_HEIGHT = 0x2C,
	GRAPHICS_MATRIX = 0x1A0L
};

extern bool ShowCities;
extern bool ShowBorderBox;
extern bool ShowAnimals;
extern bool ShowPlayers;
extern bool ShowAggressive;
extern bool ShowWepAmmo;
extern bool ShowDisplaySettings;

extern bool ShowDead;
extern bool ShowItems;
extern bool ShowContainers;

#endif // SDK_H