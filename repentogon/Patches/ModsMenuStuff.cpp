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
ANM2* undopop = new ANM2();
string searchstr = "";
bool issearching= false;
const int keydelay = 20;
int lastvalid = 0;
int opensearchkey = (int)KeyboardKey::KEY_LEFT_CONTROL;
int undomodchangeskey = (int)KeyboardKey::KEY_BACKSPACE;
int undomodchangesbtn = 24; //left bumber
unordered_map<int, int> lastKeyPressTimeMap;
bool cursorblink = false;
float prevscroll = 0;
int prevselect = 0;
const unsigned int minmods = 15;
float paperedge = 0;

vector<bool> enabledtstates;
bool enabledstatesset = false;
bool modswereenabled = true;

void SetupEnabledStates() {
	if (!enabledstatesset) {
		enabledstatesset = true;
		ModManager* modman = g_Manager->GetModManager();
		for each (ModEntry * mod in modman->_mods) {
			enabledtstates.push_back(mod->IsEnabled());
		}
		modswereenabled = g_Manager->GetOptions()->ModsEnabled();
	}
}

void RestoreEnabledStates() {
	ModManager* modman = g_Manager->GetModManager();
	int i = 0;
	for each (ModEntry * mod in modman->_mods) {
		if (mod->IsEnabled() != enabledtstates[i]) {
			mod->SetEnabled(enabledtstates[i]);
			string disablepath = std::filesystem::current_path().string() + "\\mods\\" + mod->GetDir().c_str() + "\\disable.it";
			if (mod->IsEnabled()) {
				std::remove(disablepath.c_str());
			}
			else {
				ofstream outFile(disablepath);
			}
		}
		i++;
	}
	g_Manager->GetOptions()->_enableMods = modswereenabled;
}

bool IsKeytriggered(int key) {
	return g_InputManagerBase.IsButtonTriggered(key, -1, 0);
}
bool IsKeyPressed(int key) {
	return g_InputManagerBase.IsButtonPressed(key, -1, 0);
}

HOOK_METHOD(MenuManager, Update, () -> void) {
	prevscroll = g_MenuManager->_scrollinterpolationY;
	super();
}

HOOK_METHOD(Menu_Mods, Update, () -> void) {
	
	if (g_InputManagerBase.IsActionTriggered(14,-1,0)) {
		if ((lastvalid > -1) && (!issearching) && (this->SelectedElement <= lastvalid) && (this->SelectedElement >= 0) && (!(IsKeytriggered(opensearchkey)))) {
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
		prevselect = this->SelectedElement;
		super();
	}
	if (this->SelectedElement > lastvalid) {
		if (prevselect <= 0) {
			this->SelectedElement = lastvalid;
		}
		else {
			this->SelectedElement = 0;
		}
	}
}

HOOK_METHOD(InputManager, IsActionTriggered, (int btnaction, int controllerid, int unk) -> bool) {
	if (issearching){// && ((btnaction == 17) || (btnaction == 18))) {
		return false;
	}
	return super(btnaction, controllerid, unk);
}

HOOK_METHOD(Menu_Mods, Render, () -> void) {
	LayerState* frame_widgetprompt = this->ModsMenuSprite.GetLayer("WidgetTab");
	LayerState* frame_widgettext = this->ModsMenuSprite.GetLayer("WidgetText");
	LayerState* frame_widgetbg = this->ModsMenuSprite.GetLayer("Widget");
	LayerState* framemain = this->ModsMenuSprite.GetLayer("Main");
	frame_widgetprompt->_visible = false;
	frame_widgettext->_visible = false;
	frame_widgetbg->_visible = false;
	super();
	frame_widgetprompt->_visible = true;
	frame_widgettext->_visible = true;
	frame_widgetbg->_visible = true;
	SetupEnabledStates();
	Vector* ref = &g_MenuManager->_ViewPosition;
	ref = new Vector(ref->x + 39, ref->y + 15);
	Vector* offset = new Vector(ref->x - 1440, ref->y + 216);
	Vector initialpos = Vector(70 + offset->x, offset->y + 35);
	Vector* undopos = new Vector(310 + offset->x, offset->y + 63);
	//printf("%f %f (%f %f)\n", (frame_widgettext->_pos.x) , frame_widgettext->_pos.y,undopos->x,undopos->y);
	Vector* tabbtnpos = new Vector(307 + offset->x, offset->y + 18);
	Vector pos = Vector(70 + offset->x, offset->y + 35);
	Vector* barpos = new Vector(pos.x + 90, (g_HEIGHT- 30));
	Vector z = Vector(0, 0);
	ModManager* modman = g_Manager->GetModManager();
	DrawStringScaledEntry* entry = new DrawStringScaledEntry();

	entry->_boxWidth = 0;
	entry->_center = false;
	entry->_scaleX = 0.9f;
	entry->_scaleY = 0.9f;
	entry->_color._blue = 0.f;
	entry->_color._green = 0.f;
	entry->_color._red = 0.f;
	int i = 1;
	int actualpos = -1;

	bool did = false;
	bool modsenabled = g_Manager->GetOptions()->ModsEnabled();
	lastvalid = -1;
	int currentTime = g_Manager->_framecount;

	if (g_MenuManager->_selectedMenuID == 16) {				//i am a newbie so here's a question:
		frame_widgettext->_cropOffset.y = (float)64 * modsenabled;	//is it worth keeping the if check here? (perf question)
	}

	if (g_MenuManager->_selectedMenuID != 16) { issearching = false; }
			if (issearching) {
				if (modman->_mods.size() > minmods) { //no point in search when all your mods fit in 1 screen
					for (int i = 32; i < 348; ++i) {
						if (IsKeyPressed(i)) {
							if ((currentTime - lastKeyPressTimeMap[i]) < keydelay) {
								continue;
							}

							if ((i == opensearchkey) || (i == (int)KeyboardKey::KEY_ENTER) || (i == (int)KeyboardKey::KEY_ESCAPE)) {
								issearching = false;
								lastKeyPressTimeMap[i] = currentTime;
								break;
							}
							else if (i == (int)KeyboardKey::KEY_BACKSPACE) {
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
									lastKeyPressTimeMap[i] = currentTime - (int)(keydelay * 0.7f);
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
			}
			else if ((modman->_mods.size() > minmods) && (g_MenuManager->_selectedMenuID == 16) && (IsKeytriggered(opensearchkey)) && ((currentTime - lastKeyPressTimeMap[opensearchkey]) > keydelay)) {

				issearching = true;
				lastKeyPressTimeMap[opensearchkey] = currentTime;
			}
			else if ((g_MenuManager->_selectedMenuID == 16) && (((g_MenuManager->_controllerIndex <= 0) && (IsKeytriggered(undomodchangeskey)) && ((currentTime - lastKeyPressTimeMap[undomodchangeskey]) > keydelay)))|| ((g_MenuManager->_controllerIndex > 0) && g_InputManagerBase.IsActionTriggered(undomodchangesbtn, -1,0))) {
				RestoreEnabledStates();
				this->State = 0; //resets the change tracker
			}
		if (did) { this->SelectedElement = 0; }
	
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
			if (!modsenabled) {
				entry->_color._alpha = entry->_color._alpha * 0.3f;
			}
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
		if (!modsenabled) {
			entry->_color._alpha = entry->_color._alpha * 0.3f;
		}
		g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
		if (actualpos == this->SelectedElement) {
			this->_pointerToSelectedMod = mod;
		}
		lastvalid = actualpos;
		i++;
	}

	if (paperedge == 0) { paperedge = (float)((modman->_mods.size() * 25) + 40); } // this is to prevent the searchbar from busting into the unknown depths below

		if (!searchbar->_loaded) {
			ANM2* s = this->GetModsMenuSprite();
			searchbar->construct_from_copy(s);
			string dir = string(REPENTOGON::GetRGONGfxAbsolutePath("gfx/ui/modsmenupentogon.anm2"));
			searchbar->Load(dir, true);
			searchbar->LoadGraphics(true);
			searchbar->Play("SearchBar", true);
			searchbar->Update();
			
			undopop->construct_from_copy(s);
			undopop->Load(dir, true);
			undopop->LoadGraphics(true);
		}
		if (g_MenuManager->_controllerIndex > 0) {
			undopop->SetFrame(&string("UndoPop"), 1);
			undopop->Update();
		}
		else {
			undopop->SetFrame(&string("UndoPop"), 0);
			undopop->Update();
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
		
		int egde = (int)(initialpos.y + paperedge);
		if (modman->_mods.size() > minmods) {
			if (barpos->y >= egde) {
				Vector* vdif = new Vector(barpos->x, (float)egde);
				entry->_y = (float)egde;
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
		}
		undopop->Render(undopos, &z, &z);
		
		//this->ModsMenuSprite.RenderLayer(11, &frame_widgetbg->_pos, &z, &z); //cant get this ones to work :(
		//this->ModsMenuSprite.RenderLayer(12, &frame_widgettext->_pos, &z, &z);
		//this->ModsMenuSprite.RenderLayer(13, &frame_widgetprompt->_pos, &z, &z);
		for (int c = 0; c < 11; c++) { 
			this->ModsMenuSprite.GetLayer(c)->_visible = false;
		}
		if (g_MenuManager->_controllerIndex > 0) {
			this->ModsMenuSprite.GetLayer(13)->_visible = false;
		}
		super(); //sorry!
		if (g_MenuManager->_controllerIndex > 0) {
			g_Manager->_buttonsSprite.SetFrame(&string("XboxOne"), 3);//you cant be fucking serious, i keep forgetting frame is a fucking float!
			g_Manager->_buttonsSprite.SetLayerFrame(0, 3);
			g_Manager->_buttonsSprite.Update();
			g_Manager->_buttonsSprite.Render(tabbtnpos, &z, &z);
		}
		for (int c = 0; c < 11; c++) {
			this->ModsMenuSprite.GetLayer(c)->_visible = true;
		}
		if (modman->_mods.size() > minmods) {
			entry->_scaleX = 1;
			entry->_scaleY = 1;
			g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
			if (issearching) {
				entry->_text = "|";
				entry->_color._alpha = 0.8f;
				if (cursorblink) { entry->_color._alpha = 0.3f; }
				if (lastKeyPressTimeMap[-1] < currentTime) { cursorblink = !cursorblink; lastKeyPressTimeMap[-1] = currentTime + 10; }
				//entry->_x += 2;
				entry->_x += g_Manager->_font7_TeamMeat_10.GetStringWidth(searchstr.c_str());
				entry->_scaleY = 1.2f;
				g_Manager->_font7_TeamMeat_10.DrawStringScaled(*entry);
			}
		}
}
