#ifndef TESTENTITY_H
#define TESTENTITY_H

#pragma once

#include "Math.h"

class TestEnt {
public:
	TestEnt();
	~TestEnt();

	void		Update(DWORD_PTR dwPointer);
	void		UpdateZombie();

	DWORD_PTR	GetPointer();

	Vector3		GetOrigin();
	Vector3		GetVelocity();

	int			GetType();

	bool		IsValid();

	const char*	GetName();

private:
	void		Clear();

private:
	DWORD_PTR	m_dwPointer;

	Vector3		m_vOrigin;
	Vector3		m_vVelocity;

	int			m_iType;

	char		m_pName[32];
};

#endif // ENTITY_H