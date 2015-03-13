#include "Engine.h"
#include "SDK.h"

#include "Direct3D.h"
#include "Process.h"

Engine* g_pEngine = new Engine();

Engine::Engine() {

}

Engine::~Engine() {

}
 
bool Engine::WorldToScreen(const Vector3& vIn, Vector3& vOut) {
	DWORD_PTR Graphics = process->Read<DWORD_PTR>(GRAPHICS_ADDRESS);
	DWORD_PTR Camera = process->Read<DWORD_PTR>(Graphics + GRAPHICS_CAMERA);
	DWORD_PTR CameraMatrix = process->Read<DWORD_PTR>(Camera + GRAPHICS_CAMERA_MATRIX);

	CameraMatrix += GRAPHICS_MATRIX_PAD;

	int ScreenWidth = m_ScreenSize[0];// 1551; // process->Read<int>(Graphics + GRAPHICS_SCREEN_WIDTH);
	int ScreenHeight = m_ScreenSize[1];// 1111; // process->Read<int>(Graphics + GRAPHICS_SCREEN_HEIGHT);

	D3DXMATRIX Matrix = process->Read<D3DXMATRIX>(CameraMatrix + GRAPHICS_MATRIX);

	D3DXMatrixTranspose(&Matrix, &Matrix);

	Matrix4x4 vMatrix = *(Matrix4x4*)&Matrix;

	vMatrix._21 *= -1;
	vMatrix._22 *= -1;
	vMatrix._23 *= -1;
	vMatrix._24 *= -1;

	float w = vMatrix.GetAxis(3).Dot(vIn) + vMatrix.m[3][3];

	if (w < 0.098) {
		return false;
	}

	float x = vMatrix.GetAxis(0).Dot(vIn) + vMatrix.m[0][3];
	float y = vMatrix.GetAxis(1).Dot(vIn) + vMatrix.m[1][3];

	vOut.x = (ScreenWidth / 2) * (1.0 + x / w);
	vOut.y = (ScreenHeight / 2) * (1.0 - y / w);

	return true;
}