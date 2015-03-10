#include "Entity.h"
#include "SDK.h"

#include "Process.h"

Entity::Entity() {
	Clear();
}

Entity::~Entity() {

}

void Entity::UpdateLocal(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_fX = process->Read<float>(m_dwPointer + 0x200);
	m_fY = process->Read<float>(m_dwPointer + 0x204);
	m_fZ = process->Read<float>(m_dwPointer + 0x208);
	m_fDir = process->Read<float>(m_dwPointer + 0x230);


	long posOffset = process->Read<long>(m_dwPointer + 0x190);
	m_fX = process->Read<float>(posOffset + 0x110);
	m_fY = process->Read<float>(posOffset + 0x114);
	m_fZ = process->Read<float>(posOffset + 0x118);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateNPC() {
	m_fX = process->Read<float>(m_dwPointer + 0x1C0);
	m_fY = process->Read<float>(m_dwPointer + 0x1C4);
	m_fZ = process->Read<float>(m_dwPointer + 0x1C8);

	if (m_fX == 0)
	{
		m_fX = process->Read<float>(m_dwPointer + 0x1360);
		m_fY = process->Read<float>(m_dwPointer + 0x1364);
		m_fZ = process->Read<float>(m_dwPointer + 0x1368);
	}

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateLoot() {
	m_fX = process->Read<float>(m_dwPointer + 0x13E0L);
	m_fY = process->Read<float>(m_dwPointer + 0x13E4L);
	m_fZ = process->Read<float>(m_dwPointer + 0x13E8L);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateId(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_iId = process->Read<int>(m_dwPointer + 0x5B0);
}

void Entity::SetName() {
	DWORD_PTR nameEntry = process->Read<DWORD_PTR>(m_dwPointer + 0x468);

	process->Read(nameEntry, m_pName, process->Read<int>(m_dwPointer + 0x470));
}

void Entity::SetDistanceFrom(Vector3 vec) {
	float distX = m_vLocation.x - vec.x;
	float distY = m_vLocation.z - vec.z;
	float distZ = m_vLocation.y - vec.y;

	m_iDist = int(sqrt(((distX * distX) + (distY * distY)) + (distZ * distZ)) + 0.5);
}

DWORD_PTR Entity::GetPointer() {
	return m_dwPointer;
}

Vector3 Entity::GetLocation() {
	return m_vLocation;
}


int Entity::GetId() {
	return m_iId;
}

const char* Entity::GetName() {
	return m_pName;
}

int Entity::GetDist() {
	return m_iDist;
}
float Entity::GetDir() {
	return m_fDir;
}

void Entity::Clear() {
	m_dwPointer = NULL;

	m_vLocation = Vector3(0, 0, 0);
	m_iId = NULL;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fDir = 0.0f;

	memset(m_pName, NULL, sizeof(char[32]));
}
