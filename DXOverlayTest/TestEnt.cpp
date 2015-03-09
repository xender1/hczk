#include "TestEnt.h"
#include "SDK.h"

#include "Process.h"

TestEnt::TestEnt() {
	Clear();
}

TestEnt::~TestEnt() {

}

void TestEnt::Update(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	if (!m_dwPointer) {
		return;
	}

	m_vOrigin = process->Read<Vector3>(m_dwPointer + ENTITY_ORIGIN);
	m_vVelocity = process->Read<Vector3>(m_dwPointer + ENTITY_VELOCITY);

	m_iType = process->Read<int>(m_dwPointer + ENTITY_TYPE);

	process->Read(m_dwPointer + ENTITY_NAME, m_pName, sizeof(char[32]));
}

DWORD_PTR TestEnt::GetPointer() {
	return m_dwPointer;
}

Vector3 TestEnt::GetOrigin() {
	return m_vOrigin;
}

Vector3 TestEnt::GetVelocity() {
	return m_vVelocity;
}

int TestEnt::GetType() {
	return m_iType;
}

bool TestEnt::IsValid() {
	if (!GetPointer()) {
		return false;
	}

	if (GetType() != ENTITY_PLAYER) {
		return false;
	}

	if (GetOrigin().IsZero()) {
		return false;
	}

	return true;
}

const char* TestEnt::GetName() {
	return m_pName;
}

void TestEnt::Clear() {
	m_dwPointer = NULL;

	m_vOrigin = Vector3(0, 0, 0);
	m_vVelocity = Vector3(0, 0, 0);

	m_iType = NULL;

	memset(m_pName, NULL, sizeof(char[32]));
}