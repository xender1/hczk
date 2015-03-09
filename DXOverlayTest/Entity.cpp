#include "Entity.h"
#include "SDK.h"

#include "Process.h"

Entity::Entity() {
	Clear();
}

Entity::Entity(long dwPointer) {
	Clear();
	m_dwPointer = dwPointer;
	m_iType = process->Read<int>(m_dwPointer + ENTITY_TYPE);
}

Entity::~Entity() {

}

void Entity::UpdateZombie() {
	m_x = process->Read<long>(m_dwPointer + 0x1C0);
	m_y = process->Read<long>(m_dwPointer + 0x1C4);
	m_z = process->Read<long>(m_dwPointer + 0x1C8);

	if (m_x == 0) {
		m_x = process->Read<long>(m_dwPointer + 0x1360);
		m_y = process->Read<long>(m_dwPointer + 0x1364);
		m_z = process->Read<long>(m_dwPointer + 0x1368);
	}

	//m_speed = ;
	//m_yaw = ;
	//m_pitch = ;

	//if on what type deer rabbit, player, agressive wolf bear zombie

	//m_vOrigin = process->Read<Vector3>(m_dwPointer + ENTITY_ORIGIN);
	//m_vVelocity = process->Read<Vector3>(m_dwPointer + ENTITY_VELOCITY);

	process->Read(m_dwPointer + ENTITY_NAME, m_pName, sizeof(char[32]));


}

void Entity::UpdateLoot() {
	m_x = process->Read<float>(m_dwPointer + 0x1C0);
	m_y = process->Read<float>(m_dwPointer + 0x1C4);
	m_z = process->Read<float>(m_dwPointer + 0x1C8);

	process->Read(m_dwPointer + ENTITY_NAME, m_pName, sizeof(char[32]));
}


void Entity::UpdateLocal(long dwPointer) {
	Clear();
	
	m_dwPointer = dwPointer;

	if (!m_dwPointer) {
		return;
	}

	m_x = process->Read<long>(m_dwPointer + 0x200);
	m_y = process->Read<long>(m_dwPointer + 0x204);
	m_z = process->Read<long>(m_dwPointer + 0x208);
	m_d = process->Read<long>(m_dwPointer + 0x230);


	long posOffset = process->Read<long>(m_dwPointer + 0x190);
	m_x = process->Read<long>(posOffset + 0x110);
	m_y = process->Read<long>(posOffset + 0x114);
	m_z = process->Read<long>(posOffset + 0x118);
}

const char* Entity::DrawLocalData() { 
	std::string s = "Hello";
	//s.append("World");// +" WOrld";
	char const *retChar = s.c_str();
	return retChar;
}

const char* Entity::Stringingify(float fVal) {
	std::string s = std::to_string(fVal);
	char const *retChar = s.c_str();  //use char const* as target type
	return retChar;
}

DWORD_PTR Entity::GetPointer() {
	return m_dwPointer;
}

Vector3 Entity::GetOrigin() {
	return m_vFeet;
}

Vector3 Entity::GetVelocity() {
	return m_vVelocity;
}

int Entity::GetType() {
	return m_iType;
}

bool Entity::IsValid() {
/*	if (!GetPointer()) {
		return false;
	}

	if (GetType() != ENTITY_PLAYER) {
		return false;
	}

	if (GetOrigin().IsZero()) {
		return false;
	}
*/
	if (m_iType == 0)
		return false;

	return true;
}

const char* Entity::GetName() {
	return m_pName;
}

void Entity::Clear() {
	m_dwPointer = NULL;

	m_vFeet = Vector3(0, 0, 0);
	m_vHead = Vector3(0, 0, 0);
	m_vVelocity = Vector3(0, 0, 0);

	m_iType = NULL;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_speed = 0.0f;
	m_yaw = 0.0f;
	m_pitch = 0.0f;

	//memset(m_pName, NULL, sizeof(char[32]));
}