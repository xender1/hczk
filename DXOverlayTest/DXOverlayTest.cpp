// DXOverlayTest.cpp : Defines the entry point for the console application.
//


#include "Overlay.h"
#include "Renderer.h"

#include "Process.h"
#include "Engine.h"
#include "Entity.h"
#include "TestEnt.h"

#include "SDK.h"

Overlay* gOverlay = new Overlay();
Renderer* gRenderer = new Renderer();

std::vector<Entity> EntityList;

void DrawTowns();

void H1Z1ProcessOverlay() {
	//gRenderer->DrawString(loc, 180, Color::Blue(), "such wow");
	gRenderer->DrawRect(100, 100, 30, 60, Color::Red());

	//Get Base Pointers to H1Z1
	DWORD_PTR dwGameClient = process->Read<DWORD_PTR>(GAME_CLIENT_ADDRESS);
	DWORD_PTR dwFirstEntity = process->Read<DWORD_PTR>(dwGameClient + 0x11D8);
	
	int entityCount = process->Read<int>(dwGameClient + 0x688);
	int cntEntProcessed = 0;

	/* PLayer Locations and Drawing Player Locations */
	Entity LocalEntity;
	//LocalEntity.UpdateLocal(dwFirstEntity);

	float m_x = process->Read<long>(dwFirstEntity + 0x200);
	float m_y = process->Read<long>(dwFirstEntity + 0x204);
	float m_z = process->Read<long>(dwFirstEntity + 0x208);
	float m_d = process->Read<long>(dwFirstEntity + 0x230);


	long posOffset = process->Read<long>(dwFirstEntity + 0x190);
	m_x = process->Read<long>(posOffset + 0x110);
	m_y = process->Read<long>(posOffset + 0x114);
	m_z = process->Read<long>(posOffset + 0x118);

	gRenderer->DrawString(50, 100, Color::Green(), "Direction: ");
	gRenderer->DrawString(300, 100, Color::Green(), LocalEntity.Stringingify(m_d));
	gRenderer->DrawString(800, 800, Color::Cyan(), LocalEntity.DrawLocalData());


	/* Entity For Loop
	*
	*
	*/
	DWORD_PTR dwEntity;
	dwEntity = process->Read<DWORD_PTR>(dwFirstEntity + 0x400);
	for (int i = 1; i < entityCount; i++) {
		//Entity ent(dwEntity); // + sets entity id
		//if (ent.IsValid()) {
		float EntityX = 0.0f; float EntityY = 0.0f; float EntityZ = 0.0f;
		int EntityId = process->Read<int>(dwEntity + 0x5B0);



		if (EntityId != 0) {
			cntEntProcessed++;

			char ename[32];
			memset(ename, NULL, sizeof(char[32]));

			DWORD_PTR nameEntry = process->Read<DWORD_PTR>(dwEntity + 0x468);

			process->Read(nameEntry, ename, process->Read<int>(dwEntity + 0x470));

			Vector3 vBot; Vector3 vFeet;
			switch (EntityId)
			{
			case 0x04/*Player*/:
			case 0x0C/*Zombie*/:
			case 0x13/*Deer*/:
			case 0x14/*Wolf*/:
			case 0x50/*Bear*/:
			case 0x55/*Rabbit*/:
			case 0x5b/*Zombie*/:
				if (process->Read<float>(dwEntity + 0x1CC) == 1.0f)
				{
					//	Boolean EntityAlive = Convert.ToBoolean(this.GameMemory.ReadByte(entityEntry + 0x136C));
					EntityX = process->Read<float>(dwEntity + 0x1C0);
					EntityY = process->Read<float>(dwEntity + 0x1C4);
					EntityZ = process->Read<float>(dwEntity + 0x1C8);

					if (EntityX == 0)
					{
						EntityX = process->Read<float>(dwEntity + 0x1360);
						EntityY = process->Read<float>(dwEntity + 0x1364);
						EntityZ = process->Read<float>(dwEntity + 0x1368);
					}

					vFeet = Vector3(EntityX, EntityY, EntityZ);
					//Vector3 vBot;
					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;

						double distX = (EntityX - m_x);
						double distY = EntityZ - m_z;
						double distZ = EntityY - m_y; 

						//std::cout << distX << ", " << distY << ", " << distZ << 
						//	" . player_info: " << m_x << ", " << m_y << m_z << std::endl;

						double a = sqrt(((distX * distX) + (distY * distY)) + (distZ * distZ));

						char fuckme[60]; memset(fuckme, NULL, sizeof(char[60]));
						sprintf(fuckme, "%s [%d m]", ename, round(a));
						
						gRenderer->DrawString(vBot.x + 175, vBot.y + 100, Color::Red(), fuckme);
						
					}

				}
				break;

			case 0x2E/*Loot*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);

				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Yellow(), ename);
				}

				break;

			case 0x2F/*Furnace*/:
			case 0x33/*Storage Container*/:
			case 0x35/*Animal Trap*/:
			case 0x36/*Dew Collector*/:
			case 0x53/*Barbeque*/:
				/*EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//		Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Green(), ename);
				}*/
				break;

			case 0x1B/*Campfire*/:
			case 0x6D/*Stash*/:
			case 0x9C/*Land Mine*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//	Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Red(), ename);
				}
				break;

			case 0x15/*Ammo*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//		Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Pink(), ename);
				}

				break;

			case 0x34/*Weapons*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//		Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Pink(), ename);
				}

				break;

			case 0x11/*OffRoad*/:
			case 0x72/*Pickup*/:
			case 0x76/*PoliceCar*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//	Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Blue(), ename);
				}
				break;

			case 0x2C/* Any Car Part (Battary, Turbo, Sparkplugs)*/:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//	Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::White(), ename);
				}
				break;

				// WORLD STATIC ITEMS, //
				//case 0x27/**/: 
			case 0x16/*Dresser*/:
			case 0x17/*Armoire*/:
			case 0x19/*World Doors*/:
			case 0x1D/*Cabinets*/:
			case 0x1E/*Cabinets*/:
			case 0x21/*Cabinets*/:
			case 0x22/*Cabinets*/:
			case 0x23/*Cabinets*/:
			case 0x25/*Refrigerator*/:
			case 0x26/*Garbage Can*/:
			case 0x28/*Cabinets*/:
			case 0x29/*Desk*/:
			case 0x27/*Dumpster*/:
			case 0x30/*File Cabinet*/:
			case 0x31/*Tool Cabinet*/:
			case 0x37/*Recycle Bin (with fire)*/:
			case 0x38/*Punji Sticks*/:
			case 0x3D/*Wooded Barricade*/:
			case 0x3E/*Water Well*/:
			case 0x3F/*Armoire*/:
			case 0x40/*Dresser*/:
			case 0x42/*Chest*/:
			case 0x44/*Wrecked Sedan*/:
			case 0x45/*Wrecked Van*/:
			case 0x46/*Wrecked Truck*/:
			case 0x49/*Ottoman*/:
			case 0x4A/*Ottoman*/:
			case 0x4F/*Designer-placed(?) Door*/:
			case 0x5D/*File Cabinet*/:
			case 0x61/*Cabinets*/:
			case 0x63/*Cabinets*/:
			case 0x79/*Designer-placed(?) Door*/:
			case 0x7A/*Resting (Bed)*/:
			case 0x7B/*Designer-placed(?) Door*/:
			case 0x7C/*Designer-placed(?) Door*/:
			case 0x80/*Cabinets*/:
			case 0x81/*Cabinets*/:
			case 0x82/*Cabinets*/:
			case 0x83/*Cabinets*/:
			case 0x84/*Cabinets*/:
			case 0x85/*Cabinets*/:
			case 0x86/*Cabinets*/:
			case 0x87/*Cabinets*/:
			case 0x88/*Cabinets*/:
			case 0xA1/*Washing Machine*/:
			case 0xA2/*Dryer*/:
			case 0x7D/*IO.FireHydrant*/:
			case 0x7E/*IO.FireHydrant*/:
				break;

			case 0x4C/*Shed*/:
			case 0x5F/*Metal Wall/Gate*/:
			case 0x62/*Basic Shack Door*/:
			case 0x6E/*Desk Foundation*/:
			case 0x9E/*Metal Door*/:
			case 0xA6/*Large Shelter*/:
			case 0xA7/*Shed*/:
				break;

				// UNKNOWN ITEMS //
			default:
				EntityX = process->Read<float>(dwEntity + 0x13E0L);
				EntityY = process->Read<float>(dwEntity + 0x13E4L);
				EntityZ = process->Read<float>(dwEntity + 0x13E8L);
				vFeet = Vector3(EntityX, EntityY, EntityZ);
				//	Vector3 vBot;
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					//std::cout << " drawX: " << vTop.x << " , drawY: " << vTop.y + h << std::endl;
					gRenderer->DrawString(vBot.x + 225, vBot.y + 150, Color::Cyan(), ename);
				}

				break;
			}

		} //end if ent = 0
		DrawTowns();
		dwEntity = process->Read<DWORD_PTR>(dwEntity + 0x400L);
	} //end for entity
	//std::cout << "processed: " << cntEntProcessed << " of " << entityCount << std::endl;
}

void DrawTowns() {
	Vector3 pValleyVect(-129.0f, 40.0f, -1146.0f);
	Vector3 cranVect(-1233.0f, 90.0f, 1855.0f);
	Vector3 ranchVect(2003.0f, 50.0f, 2221.0f);

	Vector3 pValleyBot;
	Vector3 cBot;
	Vector3 rBot;

	if (g_pEngine->WorldToScreen(pValleyVect, pValleyBot)) {

		gRenderer->DrawString(pValleyBot.x, pValleyBot.y - 10, Color::Black(), "Pleasant Valley");
	}
	if (g_pEngine->WorldToScreen(cranVect, cBot)) {

		gRenderer->DrawString(cBot.x, cBot.y - 10, Color::Black(), "Cranberry");
	}
	if (g_pEngine->WorldToScreen(ranchVect, rBot)) {

		gRenderer->DrawString(rBot.x, rBot.y - 10, Color::Black(), "Ranchito");
	}
}

void OnFrame() {
	gRenderer->PreFrame();
	H1Z1ProcessOverlay();
	gRenderer->PostFrame();
}

//int WINAPI WinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nCmdShow)
int main() 
{

	//process->DumpAllProcessNames();

	float test = 1.1f;

	std::cout << test << std::endl;
	
	char * value = "Untitled - Notepad";

	HWND newhwnd = FindWindow(NULL, value);

	bool pCheck = process->Attach("H1Z1.exe"); // "H1Z1.exe"
	bool oCheck = gOverlay->Attach(newhwnd);
	gRenderer->OnSetup(gOverlay->GetDevice());

	int* size = gOverlay->getSize();
	std::cout << " x: " << size[0] << "  y: " << size[1] << std::endl;
	g_pEngine->m_ScreenSize = size;

	gOverlay->AddOnFrame(OnFrame);
	gOverlay->OnFrame();
	//getchar();
	return 1;
}

/* set up entity class, handle all entities in this class */

