// DXOverlayTest.cpp : Defines the entry point for the console application.
//


#include "Overlay.h"
#include "Renderer.h"

#include "Process.h"
#include "Engine.h"
#include "Entity.h"

#include "SDK.h"

#define XDRAW_OFFSET 35 //needs to be off text length
#define YDRAW_OFFSET 22

bool ShowBorderBox = true;
bool ShowAnimals = true;
bool ShowPlayers = true;
bool ShowAggressive = true;
bool ShowDead = true; //not implented yet
bool ShowItems = true;
bool ShowContainers = true;
bool ShowCities = true;

Overlay* gOverlay = new Overlay();
Renderer* gRenderer = new Renderer();

int *g_hSize = gOverlay->getSize();

std::vector<Entity> EntityList;

void DrawTowns();
void DrawPlayerLocation(Entity LocalEntity);

//Debug
void DrawEntityLocation(Entity ent, Vector3 vBot);
void DrawFunTime(); //used for testing/adding in new Renderer functions

void H1Z1ProcessOverlay() {

	//Get Base Pointers to H1Z1
	DWORD_PTR dwGameClient = process->Read<DWORD_PTR>(GAME_CLIENT_ADDRESS);
	DWORD_PTR dwFirstEntity = process->Read<DWORD_PTR>(dwGameClient + 0x11D8);

	int entityCount = process->Read<int>(dwGameClient + 0x688);
	int cntEntProcessed = 0;

	/* PLayer Locations and Drawing Player Locations */
	Entity LocalEntity;
	LocalEntity.UpdateLocal(dwFirstEntity);
	DrawPlayerLocation(LocalEntity);

	/* Entity For Loop */
	DWORD_PTR dwEntity;
	dwEntity = process->Read<DWORD_PTR>(dwFirstEntity + 0x400);
	for (int i = 1; i < entityCount; i++) {
		Entity ent;
		ent.UpdateId(dwEntity); // + sets entity id

		if (ent.GetId() != 0) {
			cntEntProcessed++;

			ent.SetName();

			Vector3 vBot; Vector3 vFeet; //for world to screen
			Vector3 vTop; Vector3 vHead;
			switch (ent.GetId())
			{
			case 0x04/*Player*/:
			case 0x0C/*Zombie*/:
			case 0x13/*Deer*/:
			case 0x14/*Wolf*/:
			case 0x50/*Bear*/:
			case 0x55/*Rabbit*/:
			case 0x5b/*Zombie catch */:
				if (process->Read<float>(dwEntity + 0x1CC) == 1.0f)
				{
					ent.UpdateNPC();
					vFeet = ent.GetLocation();

					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						ent.SetDistanceFrom(LocalEntity.GetLocation());

						if (ent.GetId() == 0x13 || ent.GetId() == 0x55) { //deer or rabbit
							gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::LightBrown(), ent.GetDisplayText());
						}
						else if (ent.GetId() == 0x04) { // Player 0x04,  this also gets Stash/Campfire - could filter out / color
							gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Red(), ent.GetDisplayText());
						}
						else { //Wolf, Bear, Zombies
							gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::DarkRed(), ent.GetDisplayText());
						}
						
						// Draws the BorderBox
						vHead = ent.GetLocation() + Vector3(0, 1.8, 0);
						if (g_pEngine->WorldToScreen(vHead, vTop)) {		
							float h = vBot.y - vTop.y;
							float w = h / 5.0f;
							gRenderer->DrawBorderBoxOut(vTop.x - w, vTop.y + YDRAW_OFFSET, w * 2, h, 1, Color::Cyan(), Color::Black());
						}
						//DrawEntityLocation(ent, vBot);//debug function
					}

				}
				break;

			case 0x2E/*Loot*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Yellow(), ent.GetDisplayText());
				}
				break;

			case 0x2F/*Furnace*/:
			case 0x33/*Storage Container*/:
			case 0x35/*Animal Trap*/:
			case 0x36/*Dew Collector*/:
			case 0x53/*Barbeque*/:
				break;

			case 0x1B/*Campfire*/:
			case 0x6D/*Stash*/:
			case 0x9C/*Land Mine*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Red(), ent.GetDisplayText());
				}
				break;

			case 0x15/*Ammo*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Pink(), ent.GetDisplayText());
				}
				break;

			case 0x34/*Weapons*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Pink(), ent.GetDisplayText());
				}
				break;

			case 0x11/*OffRoad*/:
			case 0x72/*Pickup*/:
			case 0x76/*PoliceCar*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Blue(), ent.GetDisplayText());
				}
				break;

			case 0x2C/* Any Car Part (Battary, Turbo, Sparkplugs)*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::White(), ent.GetDisplayText());
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
				ent.UpdateLoot();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Cyan(), ent.GetDisplayText());
				}
				break;
			}

		} //end if ent = 0
		DrawTowns();
		dwEntity = process->Read<DWORD_PTR>(dwEntity + 0x400L);
	} //end for entity
	//std::cout << "processed: " << cntEntProcessed << " of " << entityCount << std::endl;
}

// Meeh........
void DrawTowns() {
	Vector3 pValleyVect(-129.0f, 40.0f, -1146.0f);
	Vector3 cranVect(-1233.0f, 90.0f, 1855.0f);
	Vector3 ranchVect(2003.0f, 50.0f, 2221.0f);

	Vector3 pBot;
	Vector3 cBot;
	Vector3 rBot;

	if (g_pEngine->WorldToScreen(pValleyVect, pBot)) {
		gRenderer->DrawString(pBot.x, pBot.y - 10, Color::Black(), "Pleasant Valley");
	}
	if (g_pEngine->WorldToScreen(cranVect, cBot)) {
		gRenderer->DrawString(cBot.x, cBot.y - 10, Color::Black(), "Cranberry");
	}
	if (g_pEngine->WorldToScreen(ranchVect, rBot)) {
		gRenderer->DrawString(rBot.x, rBot.y - 10, Color::Black(), "Ranchito");
	}
}

// Terrible...fix
void DrawPlayerLocation(Entity LocalEntity) {
	std::string s = std::to_string(LocalEntity.GetDir());
	char const *retChar = s.c_str();  //use char const* as target type
	gRenderer->DrawString(50, g_hSize[1] - 400, Color::Green(), "Direction: ");
	gRenderer->DrawString(125, g_hSize[1] - 400, Color::Green(), retChar);

	std::string s1 = std::to_string(LocalEntity.GetLocation().x);
	char const* retChar1 = s1.c_str();  //use char const* as target type
	gRenderer->DrawString(50, g_hSize[1] - 380, Color::Green(), "x: ");
	gRenderer->DrawString(90, g_hSize[1] - 380, Color::Green(), retChar1);


	std::string s2 = std::to_string(LocalEntity.GetLocation().y);
	char const *retChar2 = s2.c_str();  //use char const* as target type
	gRenderer->DrawString(50, g_hSize[1] - 360, Color::Green(), "y: ");
	gRenderer->DrawString(90, g_hSize[1] - 360, Color::Green(), retChar2);


	std::string s3 = std::to_string(LocalEntity.GetLocation().z);
	char const *retChar3 = s3.c_str();  //use char const* as target type
	gRenderer->DrawString(50, g_hSize[1] - 340, Color::Green(), "z: ");
	gRenderer->DrawString(90, g_hSize[1] - 340, Color::Green(), retChar3);
}

// Debug - Displays XYZ of ent next to it
void DrawEntityLocation(Entity ent, Vector3 vBot) {
	std::string s1 = std::to_string(ent.GetLocation().x);
	char const* retChar1 = s1.c_str();  //use char const* as target type
	gRenderer->DrawString(vBot.x + 10, vBot.y - 50, Color::Green(), retChar1);

	std::string s12 = std::to_string(ent.GetLocation().y);
	char const* retChar12 = s12.c_str();  //use char const* as target type
	gRenderer->DrawString(vBot.x + 10, vBot.y - 35, Color::Green(), retChar12);

	std::string s13 = std::to_string(ent.GetLocation().z);
	char const* retChar13 = s13.c_str();  //use char const* as target type
	gRenderer->DrawString(vBot.x + 10, vBot.y - 20, Color::Green(), retChar13);
}

void OnFrame() {
	gRenderer->PreFrame();
	H1Z1ProcessOverlay();
	//DrawFunTime();
	gRenderer->PostFrame();
}

//use this main function when you want NO CONSOLE
//int WINAPI WinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nCmdShow)
int main()
{

	process->DumpAllProcessNames();

	char * value = "Untitled - Notepad";
	HWND newhwnd = FindWindow(NULL, value);

	bool pCheck = process->Attach("H1Z1.exe"); // "H1Z1.exe"
	bool oCheck = gOverlay->Attach(newhwnd);
	gRenderer->OnSetup(gOverlay->GetDevice());

	g_hSize = gOverlay->getSize(); //set size for use in drawing
	g_pEngine->m_ScreenSize = g_hSize;
	std::cout << " x: " << g_hSize[0] << "  y: " << g_hSize[1] << std::endl;

	gOverlay->AddOnFrame(OnFrame);
	gOverlay->OnFrame();
	//getchar(); //debug hold for viewing
	return 1;
}

void DrawFunTime() {
	gRenderer->DrawRect(100, 100, 30, 60, Color::Red());
}

