#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include "Math.h"

class Entity {
public:
	Entity();
	~Entity();

	void		UpdateLocal(DWORD_PTR dwPointer); //initialize self (id,name,x,y,z,d)

	void		UpdateId(DWORD_PTR dwPointer);    //standard initializor for non self
	void		SetName();    //standard initializor for non self

	void		UpdateNPC();					  //initialize enemy,zombie,animal
	void		UpdateLoot();					  //initialize for all else (guns,ammo,loot,parts,food,...)

	void		SetDistanceFrom(Vector3 vec);

	DWORD_PTR	GetPointer();
	Vector3		GetLocation();
	const char*	GetName();
	int			GetId();

	int			GetDist(); //dist from player
	float		GetDir(); //set for local player only

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
	float		m_fDir;
	int			m_iDist;


};

#endif // ENTITY_H