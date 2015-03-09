#ifndef TESTENTITY_H
#define TESTENTITY_H

#pragma once

#include "Math.h"

class TestEnt {
public:
	TestEnt();
	~TestEnt();

	void		UpdateLocal(DWORD_PTR dwPointer); //initialize self (id,name,x,y,z,d)

	void		UpdateId(DWORD_PTR dwPointer);    //standard initializor for non self
	void		SetName();    //standard initializor for non self

	void		UpdateNPC();					  //initialize enemy,zombie,animal
	void		UpdateLoot();					  //initialize for all else (guns,ammo,loot,parts,food,...)

	DWORD_PTR	GetPointer();
	Vector3		GetLocation();
	const char*	GetName();
	int			GetId();

	bool		IsValid();	//ensure type is not 0



private:
	void		Clear(); //clear alls

private:
	DWORD_PTR	m_dwPointer;

	int			m_iId;

	char		m_pName[32];

	Vector3		m_vLocation;
	float		m_fX;
	float		m_fY;
	float		m_fZ;
public:
	float		m_fD;


};

#endif // ENTITY_H