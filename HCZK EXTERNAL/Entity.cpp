#include "Entity.h"
#include "SDK.h"

#include "Process.h"

Entity::Entity() {
	Clear();
}

Entity::Entity(const char* name, Vector3 location) {
	m_vLocation = Vector3(location.x, location.y, location.z);
	sprintf(m_pName, name);
}

Entity::~Entity() {

}

void Entity::UpdateLocal(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_fDir = process->Read<float>(m_dwPointer + 0x240);

	DWORD_PTR posOffset = process->Read<DWORD_PTR>(m_dwPointer + 0x198);
	m_fX = process->Read<float>(posOffset + 0x110);
	m_fY = process->Read<float>(posOffset + 0x114);
	m_fZ = process->Read<float>(posOffset + 0x118);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateEnemy() {
	
	DWORD_PTR posOffset = process->Read<DWORD_PTR>(m_dwPointer + 0x198);
	m_fX = process->Read<float>(posOffset + 0x110);
	m_fY = process->Read<float>(posOffset + 0x114);
	m_fZ = process->Read<float>(posOffset + 0x118);

	if (m_fX == 0)
	{
		m_fX = process->Read<float>(m_dwPointer + 0x414);
		m_fY = process->Read<float>(m_dwPointer + 0x1364);
		m_fZ = process->Read<float>(m_dwPointer + 0x418);
	}

	//gets health
	DWORD_PTR num22 = process->Read<DWORD_PTR>(m_dwPointer + 0x4068L);
	m_uiHealth = process->Read<unsigned int>(num22 + 0xB0L) / 100;

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateNPC() {
	DWORD_PTR posOffset = process->Read<DWORD_PTR>(m_dwPointer + 0x198);
	m_fX = process->Read<float>(posOffset + 0x110);
	m_fY = process->Read<float>(posOffset + 0x114);
	m_fZ = process->Read<float>(posOffset + 0x118);
	
	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateLoot() {
	m_fX = process->Read<float>(m_dwPointer + 0x13F0);
	m_fY = process->Read<float>(m_dwPointer + 0x13F4);
	m_fZ = process->Read<float>(m_dwPointer + 0x13F8);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateCars() {
	//m_fX = process->Read<float>(m_dwPointer + 0x13F0);
	//m_fY = process->Read<float>(m_dwPointer + 0x13F4);
	//m_fZ = process->Read<float>(m_dwPointer + 0x13F8);

	DWORD_PTR posOffset = process->Read<DWORD_PTR>(m_dwPointer + 0x198);
	m_fX = process->Read<float>(posOffset + 0x110);
	m_fY = process->Read<float>(posOffset + 0x114);
	m_fZ = process->Read<float>(posOffset + 0x118);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void Entity::UpdateId(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_iId = process->Read<int>(m_dwPointer + 0x5C8);
}

void Entity::SetName() {
	DWORD_PTR nameEntry = process->Read<DWORD_PTR>(m_dwPointer + 0x04E0);

	process->Read(nameEntry, m_pName, process->Read<int>(m_dwPointer + 0x04E8));
}

void Entity::SetDistanceFrom(Vector3 vec) {
	float distX = m_vLocation.x - vec.x;
	float distY = m_vLocation.z - vec.z;
	float distZ = m_vLocation.y - vec.y;

	m_iDist = int(sqrt(((distX * distX) + (distY * distY)) + (distZ * distZ)) + 0.5);
}

const char* Entity::GetDisplayText() {
	sprintf(m_pDisplayText, "%s [%im]", GetName(), GetDist());
	return m_pDisplayText;
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

unsigned int Entity::GetHealth() {
	return m_uiHealth;
}

void Entity::Clear() {
	m_dwPointer = NULL;

	m_vLocation = Vector3(0, 0, 0);
	m_iId = NULL;

	m_uiHealth = NULL;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fDir = 0.0f;

	memset(m_pName, NULL, sizeof(char[32]));
	memset(m_pDisplayText, NULL, sizeof(char[64]));
}
