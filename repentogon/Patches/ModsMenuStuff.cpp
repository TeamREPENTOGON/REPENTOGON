#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <memory>
#include <array>
#include <cmath>

#include <cstring>

#include "XMLData.h"
#include "ModsMenuStuff.h"

#include "SigScan.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"
#include "mologie_detours.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "../ImGuiFeatures/LogViewer.h"
#include "../MiscFunctions.h"
#include <lua.hpp>
#include "LuaCore.h"
#include "JsonSavedata.h"
#include <filesystem>
#include <regex>
#include <queue>

ANM2* searchbar = new ANM2();
string searchstr = "";
bool issearching= false;
const int keydelay = 300;
int lastvalid = 0;
const int opensearchkey = VK_CONTROL;
unordered_map<int, int> lastKeyPressTimeMap;
bool cursorblink = false;
float prevscroll = 0;
int minmods = 15;
float paperedge = 0;


HOOK_METHOD(MenuManager, Update, () -> void) {
	prevscroll = g_MenuManager->_scrollinterpolationY;
	super();
}

HOOK_METHOD(Menu_Mods, Update, () -> void) {
	if (g_InputManagerBase->GetActualInput()->IsButtonTriggered(14, -1, 0)) {
		if ((lastvalid > -1) && (!issearching) && (this->SelectedElement <= lastvalid) && (this->SelectedElement >= 0) && (!(GetKeyState(opensearchkey) & 0x8000))) {
			this->_pointerToSelectedMod->SetEnabled(!this->_pointerToSelectedMod->IsEnabled());
			string disablepath = std::filesystem::current_path().string() + "\\mods\\" + this->_pointerToSelectedMod->GetDir().c_str() + "\\disable.it";
			if (this->_pointerToSelectedMod->IsEnabled()) {
				std::remove(disablepath.c_str());
			}
			else {
				ofstream outFile(disablepath);
			}
			this->State = 1;
		}
	}else{
		super();
	}
	if (this->SelectedElement > lastvalid) {
		this->SelectedElement = lastvalid;
	}
}

HOOK_METHOD(InputBase, IsButtonTriggered, (int btnaction, int controllerid, int unk) -> bool) {
	if (issearching){// && ((btnaction == 17) || (btnaction == 18))) {
		return false;
	}
	return super(btnaction, controllerid, unk);
}

HOOK_METHOD(Menu_Mods, Render, () -> void) {
	super();
	Vector* ref = &g_MenuManager->_ViewPosition;
	ref = new Vector(ref->x + 39, ref->y + 15);
	Vector* offset = new Vector(ref->x - 1440, ref->y + 216);
	Vector initialpos = Vector(70 + offset->x, offset->y + 35);
	Vector pos = Vector(70 + offset->x, offset->y + 35);
	Vector* barpos = new Vector(pos.x + 90, (g_HEIGHT- 30));
	Vector z = Vector(0, 0);
	ModManager* modman = g_Manager->GetModManager();
	DrawStringScaledEntry* entry = new DrawStringScaledEntry();
	HWND hwnd = GetActiveWindow();
	DWORD activeProcessId;
	GetWindowThreadProcessId(hwnd, &activeProcessId);

	entry->_boxWidth = 0;
	entry->_center = false;
	entry->_scaleX = 0.9;
	entry->_scaleY = 0.9;
	entry->_color._blue = 0;
	entry->_color._green = 0;
	entry->_color._red = 0;
	int i = 1;
	int actualpos = -1;

	bool did = false;
	lastvalid = -1;
	int currentTime = GetTickCount64();
	if (g_MenuManager->_selectedMenuID != 16) { issearching = false; }
	if (modman->_mods.size() > minmods) { //no point in search when all your mods fit in 1 screen
		if (issearching && (GetCurrentProcessId() == activeProcessId)) {
			for (int i = 8; i < 256; ++i) {
				if (GetKeyState(i) & 0x8000) {
					if ((currentTime - lastKeyPressTimeMap[i]) < keydelay) {
						continue;
					}

					if ((i == opensearchkey) || (i == VK_RETURN) || (i == VK_ESCAPE)) {
						issearching = false;
						lastKeyPressTimeMap[i] = currentTime;
						break;
					}
					else if (i == VK_BACK) {
						if (!searchstr.empty()) {
							searchstr.pop_back();
							did = true;
						}
					}
					else if ((g_Manager->_font7_TeamMeat_10.GetStringWidth(searchstr.c_str()) < 225) && ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z') || (i == ' '))) {
						char ch = static_cast<char>(i);
						searchstr += tolower(ch);
						did = true;
					}
					if (did) {
						if (lastKeyPressTimeMap[i] > 0) {
							lastKeyPressTimeMap[i] = currentTime - 250;
						}
						else {
							lastKeyPressTimeMap[i] = currentTime;
						}
					}
				}
				else {
					lastKeyPressTimeMap[i] = 0;
				}
			}
		}
		else if ((g_MenuManager->_selectedMenuID == 16) && (GetKeyState(opensearchkey) & 0x8000) && ((currentTime - lastKeyPressTimeMap[opensearchkey]) > keydelay)) {

			issearching = true;
			lastKeyPressTimeMap[opensearchkey] = currentTime;
		}
		if (did) { this->SelectedElement = 0; }
	}
	for each (ModEntry* mod in modman->_mods) {	
			string order = to_string(i) + ".";
			string modname = order + mod->GetName();
			entry->_text = modname.c_str();
			if (mod->IsEnabled()) {
				string modnamelower = stringlower(modname.c_str());
				if ((searchstr.length() > 0) && (modnamelower.find(searchstr) == string::npos)) { continue; }
				actualpos++;
				pos.y += 25;
				entry->_color._alpha = 1;
			}
			else {
				continue;
			}
			entry->_x = pos.x;
			entry->_y = pos.y;
			g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
			if (actualpos == this->SelectedElement) {
				this->_pointerToSelectedMod = mod;
			}
			lastvalid = actualpos;
			i++;
	}
	for each (ModEntry * mod in modman->_mods) {
		string order = to_string(i) + ".";
		string modname = order + mod->GetName();
		entry->_text = modname.c_str();
		if (mod->IsEnabled()) {
			continue;
		}
		else {
			if ((searchstr.length() > 0) && (stringlower(modname.c_str()).find(searchstr) == string::npos)) { continue; }
			actualpos++;
			pos.y += 25;
			entry->_color._alpha = 0.5;
		}
		entry->_x = pos.x;
		entry->_y = pos.y;
		g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
		if (actualpos == this->SelectedElement) {
			this->_pointerToSelectedMod = mod;
		}
		lastvalid = actualpos;
		i++;
	}

	if (paperedge == 0) { paperedge = (modman->_mods.size() *25)+40; } // this is to prevent the searchbar from busting into the unknown depths below

	if (modman->_mods.size() > minmods) {
		if (!searchbar->_loaded) {
			ANM2* s = this->GetModsMenuSprite();
			searchbar->construct_from_copy(s);
			string dir = string("../../../resources-repentogon/gfx/ui/modsmenupentogon.anm2");
			searchbar->Load(dir, true);
			searchbar->LoadGraphics(true);
			searchbar->Play("SearchBar", true);
			searchbar->Update();
		}

		entry->_text = searchstr.c_str();
		if (issearching) {
			entry->_color._alpha = 1;
		}
		else {
			if ((searchstr.length() <= 0)) {
				entry->_text = "Press CTRL and type to filter";
			}
			entry->_color._alpha = 0.5;
		}
		entry->_x = pos.x - 20;
		
		int egde = initialpos.y + paperedge;
		if (barpos->y >= egde) {
			Vector* vdif = new Vector(barpos->x, egde);
			entry->_y = egde;
			searchbar->Render(vdif, &z, &z);
		}
		else if (((barpos->y - pos.y) < 22) && (prevscroll > 161.9) && (g_MenuManager->_scrollinterpolationY == prevscroll) && (this->SelectedElement == lastvalid)) { // this is for when the searchbar would obstruct the last element on the list
			Vector* vdif = new Vector(barpos->x, barpos->y + 20);
			entry->_y = barpos->y + 20;
			searchbar->Render(vdif, &z, &z);
		}
		else {
			entry->_y = barpos->y;
			searchbar->Render(barpos, &z, &z);
		}
		entry->_scaleX = 1;
		entry->_scaleY = 1;
		g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
		if (issearching) {
			entry->_text = "|";
			entry->_color._alpha = 0.8;
			if (cursorblink) { entry->_color._alpha = 0.3; }
			if (lastKeyPressTimeMap[-1] < currentTime) { cursorblink = !cursorblink; lastKeyPressTimeMap[-1] = currentTime + 200; }
			//entry->_x += 2;
			entry->_x += g_Manager->_font7_TeamMeat_10.GetStringWidth(searchstr.c_str());
			entry->_scaleY = 1.2;
			g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
		}
	}
}
