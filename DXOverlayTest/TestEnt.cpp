#include "TestEnt.h"
#include "SDK.h"

#include "Process.h"

TestEnt::TestEnt() {
	Clear();
}

TestEnt::~TestEnt() {

}

void TestEnt::UpdateLocal(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_fX = process->Read<long>(m_dwPointer + 0x200);
	m_fY = process->Read<long>(m_dwPointer + 0x204);
	m_fZ = process->Read<long>(m_dwPointer + 0x208);
	m_fD = process->Read<long>(m_dwPointer + 0x230);


	long posOffset = process->Read<long>(m_dwPointer + 0x190);
	m_fX = process->Read<long>(posOffset + 0x110);
	m_fY = process->Read<long>(posOffset + 0x114);
	m_fZ = process->Read<long>(posOffset + 0x118);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void TestEnt::UpdateNPC() {
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

void TestEnt::UpdateLoot() {
	m_fX = process->Read<float>(m_dwPointer + 0x13E0L);
	m_fY = process->Read<float>(m_dwPointer + 0x13E4L);
	m_fZ = process->Read<float>(m_dwPointer + 0x13E8L);

	m_vLocation = Vector3(m_fX, m_fY, m_fZ);
}

void TestEnt::UpdateId(DWORD_PTR dwPointer) {
	Clear();

	m_dwPointer = dwPointer;

	m_iId = process->Read<int>(m_dwPointer + 0x5B0);
}

void TestEnt::SetName() {
	DWORD_PTR nameEntry = process->Read<DWORD_PTR>(m_dwPointer + 0x468);

	process->Read(nameEntry, m_pName, process->Read<int>(m_dwPointer + 0x470));
}

bool TestEnt::IsValid() {
	//if (!GetPointer()) {
	//	return false;
	//}

	//if (GetId() == 0) {
	//	return false;
	//}

	if (GetLocation().IsZero()) {
		return false;
	}

	return true;
}

DWORD_PTR TestEnt::GetPointer() {
	return m_dwPointer;
}

Vector3 TestEnt::GetLocation() {
	return m_vLocation;
}


int TestEnt::GetId() {
	return m_iId;
}

const char* TestEnt::GetName() {
	return m_pName;
}

void TestEnt::Clear() {
	m_dwPointer = NULL;

	m_vLocation = Vector3(0, 0, 0);
	m_iId = NULL;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fD = 0.0f;

	memset(m_pName, NULL, sizeof(char[32]));
}