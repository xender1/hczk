#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include "Math.h"

class Entity {
public:
	Entity();
	Entity(long dwPointer);
	~Entity();

	void		UpdateLocal(long dwPointer);
	void		UpdateZombie();
	void		UpdateLoot();

	DWORD_PTR	GetPointer();

	Vector3		GetOrigin();
	Vector3		GetVelocity();

	int			GetType();

	bool		IsValid();

	const char*	GetName();
	const char*		Stringingify(float fVal);

	const char* DrawLocalData();

private:
	void			Clear();

private:
	DWORD_PTR	m_dwPointer;

	Vector3		m_vFeet;
	Vector3		m_vHead;
	Vector3		m_vVelocity;

	int			m_iType;
	char		m_pName[32];

public:
	float		m_x;
	float		m_y;
	float		m_z;
	float		m_d;
	float		m_speed;
	float		m_yaw;
	float		m_pitch;
};

#endif // ENTITY_H