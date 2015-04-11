// DXOverlayTest.cpp : Defines the entry point for the console application.
//


#include "Overlay.h"
#include "Renderer.h"

#include "Process.h"
#include "Engine.h"
#include "Entity.h"

#include <thread>
#include <vector> //not needed but vs being gay atm
#include "KeyHook.h"
#include "IniReader.h"

#include "SDK.h"

#define XDRAW_OFFSET 35 //needs to be off text length
#define YDRAW_OFFSET 22

/* Global Overlay/Render declarations */
Overlay* gOverlay = new Overlay();
Renderer* gRenderer = new Renderer();

//target overlays width/height
int *g_hSize = NULL;
char* hwndName;

//FUTURE. store players for map
std::vector<Entity> EntityList;

/*** EXT VARIABLE DECL FROM SDK.H ***
************************************/
bool ShowCities			= true;
bool ShowBorderBox		= false;
bool ShowAnimals		= true;
bool ShowPlayers		= true;
bool ShowAggressive		= true;
bool ShowWepAmmo		= true;
bool ShowItems			= true;
bool ShowDisplaySettings = false;
bool BoolReadConfig		= false;
bool ShowToggleSettings = true;

bool ShowDead			= true;
bool ShowContainers		= true;
/***********************************/

/*** Functions Declarations ***
************************************/
Entity ProcessEntity();
void DrawEntityToScreen(Entity ent);

void DrawTowns(Entity LocalEntity);
void DrawDisplaySettings();
void DrawPlayerLocation(Entity LocalEntity);

//Debug Functions
void DrawEntityLocation(Entity ent, Vector3 vBot);
void DrawFunTime(); //used for testing/adding in new Renderer functions

void ReadConfigFile();
void KeyLoggerThread();
/**********************************/

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

	//std::cout << "pre loop" << std::endl;

	/* Entity For Loop */
	DWORD_PTR dwEntity;
	dwEntity = process->Read<DWORD_PTR>(dwFirstEntity + 0x410);
	for (int i = 1; i < entityCount; i++) {
		Entity ent;
		ent.UpdateId(dwEntity); // + sets entity id

		if (ent.GetId() != 0) {
			cntEntProcessed++;

			ent.SetName();

			Vector3 vBot; Vector3 vFeet; //for world to screen
			Vector3 vTop; Vector3 vHead; BYTE EntAlive;
			switch (ent.GetId())
			{
			case 0x04/*Player*/:
			case 0x05/*BR Player*/:
			case 0x0C/*Zombie*/:
			case 0x13/*Deer*/:
			case 0x14/*Wolf*/:
			case 0x50/*Bear*/:
			case 0x55/*Rabbit*/:
			case 0x5b/*Zombie + ^ catch */:
				EntAlive = process->Read<BYTE>(ent.GetPointer() + 0x137C);
				if (EntAlive == 1) // + ShowDead option
				{
					if (ent.GetId() == 0x13 || ent.GetId() == 0x55) { //deer or rabbit
						ent.UpdateNPC();
					}
					else if (ent.GetId() == 0x04 || ent.GetId() == 0x05) { //enemy
						ent.UpdateEnemy();
					}
					else { //wolf bear zombie
						ent.UpdateNPC();
					}
					//ent.UpdateNPC();
					vFeet = ent.GetLocation();

					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						ent.SetDistanceFrom(LocalEntity.GetLocation());

						if (ent.GetId() == 0x13 || ent.GetId() == 0x55) { //deer or rabbit
							if (ShowAnimals) { gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::LightBrown(), ent.GetDisplayText()); }
						}
						else if (ent.GetId() == 0x04 || ent.GetId() == 0x05) { // Player 0x04,  this also gets Stash/Campfire - could filter out / color
							if (ShowPlayers) { 
								gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Red(), ent.GetDisplayText());
								std::string s = "%%";
								char ehp[12]; memset(ehp, NULL, sizeof(char[12]));
								sprintf(ehp, "%i%s", ent.GetHealth(), s.c_str());
								gRenderer->DrawString(vBot.x - XDRAW_OFFSET + 5, vBot.y + YDRAW_OFFSET - 10, Color::Red(), ehp);
							}
						}
						else { //Wolf, Bear, Zombies
							if (ShowAggressive) { gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::DarkRed(), ent.GetDisplayText()); }
						}
						
						// Draws the BorderBox
						if (ShowBorderBox) {
							vHead = ent.GetLocation() + Vector3(0, 1.8, 0);
							if (g_pEngine->WorldToScreen(vHead, vTop)) {
								float h = vBot.y - vTop.y;
								float w = h / 5.0f;
								gRenderer->DrawBorderBoxOut(vTop.x - w, vTop.y + YDRAW_OFFSET, w * 2, h, 1, Color::Cyan(), Color::Black());
							}
						} //endif ShowBorderBox
						//DrawEntityLocation(ent, vBot);//debug function
					} //endif EntAlive

				} //endif process->Read
				break;

			case 0x2E/*Loot*/:
				if (ShowItems) {
					ent.UpdateLoot();
					vFeet = ent.GetLocation();

					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						ent.SetDistanceFrom(LocalEntity.GetLocation());

						gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Yellow(), ent.GetDisplayText());
					}
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
				break;
			case 0x9C/*Land Mine*/:
				ent.UpdateLoot();
				vFeet = ent.GetLocation();
				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Red(), ent.GetDisplayText());
				}
				break;

			case 0x15/*Ammo*/:
			case 0x34/*Weapons*/:
				if (ShowWepAmmo) {
					ent.UpdateLoot();
					vFeet = ent.GetLocation();

					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						ent.SetDistanceFrom(LocalEntity.GetLocation());

						gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Pink(), ent.GetDisplayText());
					}
				}
				break;

			case 0x11/*OffRoad*/:
			case 0x72/*Pickup*/:
			case 0x76/*PoliceCar*/:
				ent.UpdateCars();
				vFeet = ent.GetLocation();

				if (g_pEngine->WorldToScreen(vFeet, vBot)) {
					ent.SetDistanceFrom(LocalEntity.GetLocation());

					gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Blue(), ent.GetDisplayText());
				}
				break;

			case 0x2C/* Any Car Part (Battary, Turbo, Sparkplugs)*/:
				if (ShowItems) {
					ent.UpdateLoot();
					vFeet = ent.GetLocation();

					if (g_pEngine->WorldToScreen(vFeet, vBot)) {
						ent.SetDistanceFrom(LocalEntity.GetLocation());

						gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::White(), ent.GetDisplayText());
					}
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
				if (ShowItems) {
				//	ent.UpdateLoot();
				//	vFeet = ent.GetLocation();

				//	if (g_pEngine->WorldToScreen(vFeet, vBot)) {
				//		ent.SetDistanceFrom(LocalEntity.GetLocation());

				//		gRenderer->DrawString(vBot.x - XDRAW_OFFSET, vBot.y + YDRAW_OFFSET, Color::Cyan(), ent.GetDisplayText());
				//	}
				}
				break;
			} //end switch(ent.GetId()
			
			//DrawEntityToScreen(ent);

		} //end if ent = 0
		if (ShowCities) { DrawTowns(LocalEntity); }
		if (ShowToggleSettings) {
			if (ShowDisplaySettings) { DrawDisplaySettings(); }
		}
		//if (BoolReadConfig) { ReadConfigFile(); }

		dwEntity = process->Read<DWORD_PTR>(dwEntity + 0x410L);
	} //end for entity
	//std::cout << "processed: " << cntEntProcessed << " of " << entityCount << std::endl;
}

Entity entPleasantValley("Pleasant Valley", Vector3(-129.0f, 40.0f, -1146.0f));
Entity entCranberry("Cranberry", Vector3(-1233.0f, 90.0f, 1855.0f));
Entity entRanchito("Ranchito", Vector3(2003.0f, 50.0f, 2221.0f));
// Meeh........
void DrawTowns(Entity LocalEntity) {
	Vector3 pBot;
	Vector3 cBot;
	Vector3 rBot;

	if (g_pEngine->WorldToScreen(entPleasantValley.GetLocation(), pBot)) {
		entPleasantValley.SetDistanceFrom(LocalEntity.GetLocation());
		gRenderer->DrawString(pBot.x, pBot.y - 10, Color::DarkGreen(), entPleasantValley.GetDisplayText());
	}
	if (g_pEngine->WorldToScreen(entCranberry.GetLocation(), cBot)) {
		entCranberry.SetDistanceFrom(LocalEntity.GetLocation());
		gRenderer->DrawString(cBot.x, cBot.y - 10, Color::DarkGreen(), entCranberry.GetDisplayText());
	}
	if (g_pEngine->WorldToScreen(entRanchito.GetLocation(), rBot)) {
		entRanchito.SetDistanceFrom(LocalEntity.GetLocation());
		gRenderer->DrawString(rBot.x, rBot.y - 10, Color::DarkGreen(), entRanchito.GetDisplayText());
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
	//process->DumpAllProcessNames();

	//reads ini to get certain properties
	ReadConfigFile();
	std::cout << hwndName << std::endl;
	HWND newhwnd = FindWindow(NULL, hwndName);

	bool pCheck = process->Attach("H1Z1.exe"); // "H1Z1.exe"
	bool oCheck = gOverlay->Attach(newhwnd);
	gRenderer->OnSetup(gOverlay->GetDevice());

	g_hSize = gOverlay->getSize(); //set size for use in drawing
	g_pEngine->m_ScreenSize = g_hSize;
	std::cout << " x: " << g_hSize[0] << "  y: " << g_hSize[1] << std::endl;

	//start key logger for catching alt commands for display options
	std::thread keyThread(KeyLoggerThread);

	//getchar();

	gOverlay->AddOnFrame(OnFrame);
	gOverlay->OnFrame();

	keyThread.join();
	
	//getchar(); //debug hold for viewing
	return 1; 
}

int xx = 10;
void DrawFunTime() {
	gRenderer->DrawRect(xx, 100, 30, 60, Color::Red());
	char s[4]; memset(s, NULL, sizeof(char[4]));
	sprintf(s, "%i", ShowCities);
	gRenderer->DrawString(100, 400, Color::Blue(),  s);
	if (xx < 400) { xx = xx + 5; }
	else { xx = 0; }
	if (ShowDisplaySettings) {
		DrawDisplaySettings();
	}
}

//thread for KeyHook to run
void KeyLoggerThread() {
	KeyHook mykey;
	mykey.Init();
	mykey.DoIt();
}

void ReadConfigFile() {
	CIniReader iniReader(".\\Config.ini");
	hwndName = iniReader.ReadString("Setting", "Name", "");

	BoolReadConfig = false;
}

void DrawDisplaySettings() {
	int x = 100;
	int y = g_hSize[1] - 800;
	int line_offset = 15;
	char disp[4]; memset(disp, NULL, sizeof(char[4]));
	
	ShowBorderBox ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x, y, Color::DarkPink(), "Settings");
	gRenderer->DrawString(x - 60, y, Color::DarkPink(), "alt+");
	gRenderer->DrawString(x - 53, y + line_offset, Color::Cyan(), "f1 BorderBox:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::Cyan(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowItems ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::White(), "f2 Items:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::White(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowAnimals ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::Orange(), "f3 Animals:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::Orange(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowAggressive ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::DarkRed(), "f5 Aggressive:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::DarkRed(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowWepAmmo ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::Pink(), "f6 WepAmmo:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::Pink(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowPlayers ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::Red(), "f7 Players:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::Red(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowCities ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::DarkGreen(), "f8 Cities:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::DarkGreen(), disp);
	y = y + line_offset;

	memset(disp, NULL, sizeof(char[4]));
	ShowToggleSettings ? sprintf(disp, "On") : sprintf(disp, "Off");
	gRenderer->DrawString(x - 53, y + line_offset, Color::DarkPink(), "f9 Settings:");
	gRenderer->DrawString(x + 50, y + line_offset, Color::DarkPink(), disp);
}

void DrawEntityToScreen(Entity ent) {
	Vector3 vBot; Vector3 vFeet; //for world to screen
	switch (ent.GetId()) {
		
	}
}