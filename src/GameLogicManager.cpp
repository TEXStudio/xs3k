#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif /*_WIN32*/

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

#include "GameLogicManager.hpp"
#include "GameEventCodes.h"

#include "asset/assetManager.hpp"
#include "audio/audioManager.hpp"
#include "message/message.hpp"
#include "renderer/renderManager.hpp"
#include "ui/uiManager.hpp"
#include "ui/container.hpp"
#include "ui/textbox.hpp"
#include "ui/uiUtility.hpp"
#include "util/alphamap.hpp"
#include "util/chroma.hpp"
#include "util/gradient.hpp"
#include "util/image.hpp"
#include "util/multiply.hpp"
#include "util/ppm.hpp"
#include "util/xortex.hpp"
#include "physics/PhysicsManager.hpp"
#include "util/calcnorm.hpp"

#include "SoloScene.hpp"
#include "VersusScene.hpp"
#include "MenuScene.hpp"

#include "version.h"

#include "ConsoleCommand.hpp"

using namespace TEXEngine::UI;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;
using namespace TEXEngine::Asset;
using namespace std;

GameLogicManager GameLogicManager::m_instance;

GameLogicManager& GameLogicManager::Get(){
	return m_instance;
}

GameLogicManager::GameLogicManager() :
	Object(),
#ifdef _WIN32
	m_hwnd(NULL), 
#endif //_WIn32
	m_running(false), m_fullscreen(false),
	m_soloScene(NULL), m_versusScene(NULL), m_menuScene(NULL), m_current(NULL) , 
	
	#ifdef	_WIN32
		m_reload_ui(NULL) , m_previous(NULL) , m_next(NULL) , m_left(NULL) , m_right(NULL) , m_select(NULL) , m_ingame_select(NULL) , m_console(NULL) , 
		m_cancel_change(NULL) , m_whip_key(NULL) , m_treat_key(NULL) , m_brake_key(NULL) , m_left_key(NULL) , m_right_key(NULL) , 
		m_next_camera_key(NULL) , m_previous_camera_key(NULL) , m_rearview_camera_key(NULL) , m_reset_position_key(NULL) , 
	#endif	/* _WIN32 */

	m_cprevious(NULL) , m_cnext(NULL) , m_cleft(NULL) , m_cright(NULL) , m_cselect(NULL) , m_cingame_select(NULL) , m_cconsole(NULL) , 
	m_ccancel_change(NULL) , m_cwhip_key(NULL) , m_ctreat_key(NULL) , m_cbrake_key(NULL) , m_cleft_key(NULL) , m_cright_key(NULL) ,
	m_cnext_camera_key(NULL) , m_cprevious_camera_key(NULL) , m_crearview_camera_key(NULL) , m_creset_position_key(NULL) , 

	#ifdef	_WIN32
		m_reload_value(F5) ,  m_menu_up_value(W) ,  m_menu_down_value(S) ,  m_menu_left_value(A) ,  m_menu_right_value(D) ,  m_menu_select_value(RETURN) , m_menu_cancel_value(ESC) , m_console_value(TAB) , m_pause_scene_value(SPACE) ,
		m_whip_value(0) , m_treat_value(0) , m_brake_value(0) , m_left_value(0) , m_right_value(0) ,  m_next_camera_value(0) , m_previous_camera_value(0) , m_rearview_camera_value(0) , m_reset_position_value(0) , 
		m_reload_mod_value(ALT) ,  m_menu_up_mod_value(NONE) ,  m_menu_down_mod_value(NONE) ,  m_menu_left_mod_value(NONE) ,  m_menu_right_mod_value(NONE) ,  m_menu_select_mod_value(NONE) , m_menu_cancel_mod_value(NONE) , m_console_mod_value(NONE) , m_pause_scene_mod_value(NONE) ,
		m_whip_mod_value(0) , m_treat_mod_value(0) , m_brake_mod_value(0) , m_left_mod_value(0) , m_right_mod_value(0) , m_next_camera_mod_value(0) , m_previous_camera_mod_value(0) , m_rearview_camera_mod_value(0) , m_reset_position_mod_value(0) , 
		m_previous_whip_value(0) , m_previous_treat_value(0) , m_previous_brake_value(0) , m_previous_left_value(0) , m_previous_right_value(0) , m_previous_next_camera_value(0) , m_previous_previous_camera_value(0) , m_previous_reset_position_value(0) , 
		m_previous_whip_mod_value(0) , m_previous_treat_mod_value(0) , m_previous_brake_mod_value(0) , m_previous_left_mod_value(0) , m_previous_right_mod_value(0) ,
		m_previous_next_camera_mod_value(0) , m_previous_previous_camera_mod_value(0) , m_previous_rearview_camera_mod_value(0) ,  m_previous_reset_position_mod_value(0) , 
	#endif	/* _WIN32 */

	#ifdef	_WIN32
		m_cmenu_up_value(DPAD_UP) ,  m_cmenu_down_value(DPAD_DOWN) ,  m_cmenu_left_value(DPAD_LEFT) ,  m_cmenu_right_value(DPAD_RIGHT) ,  
		m_cmenu_select_value(XB_A) , m_cmenu_cancel_value(START) , m_cconsole_value(BACK) , m_cpause_scene_value(START) , 
	#endif	/* _WIN32 */


	m_cwhip_value(0) , m_ctreat_value(0) ,  m_cbrake_value(0) , m_cleft_value(0) , m_cright_value(0) , m_cnext_camera_value(0) , m_cprevious_camera_value(0) , m_creset_position_value(0) , 
	m_previous_cwhip_value(0) , m_previous_ctreat_value(0) ,  m_previous_cbrake_value(0) , m_previous_cleft_value(0) , m_previous_cright_value(0) , m_previous_cnext_camera_value(0) , m_previous_cprevious_camera_value(0) ,
	m_previous_crearview_camera_value(0) , m_previous_creset_position_value(0) , 
	
	#ifdef	_WIN32
		m_rebind_whip(false) , m_rebind_treat(false) , m_rebind_brake(false) , m_rebind_left(false) , m_rebind_right(false) , m_rebind_next_camera(false), m_rebind_previous_camera(false) , m_rebind_rearview_camera(false) , m_rebind_reset_position(false) , 
	#endif	/* _WIN32 */

	m_rebind_cwhip(false) , m_rebind_ctreat(false) , m_rebind_cbrake(false) , m_rebind_cleft(false) , m_rebind_cright(false) , m_rebind_cnext_camera(false) , m_rebind_cprevious_camera(false) , m_rebind_crearview_camera(false) , m_rebind_creset_position(false) , 
	m_console_update_pending(false)
{
}

void GameLogicManager::init_settings(){
	m_config.set_source(PATH_STUFF"data/scripts/ncf/config.ncf1");
	//TEXEngine::Core::LogManager::get().log_message(m_config.get_source());
	int err = m_config.parse();
	if(err)
		TEXEngine::Core::LogManager::get().log_message("error loading %s", m_config.get_source());

	#ifdef	_WIN32
	DEVMODE dm;
	DWORD i = 0;
	DWORD last_width = 0;
	DWORD last_height = 0;
	while ( EnumDisplaySettings(NULL,i,&dm) != 0 )
	{
		if (  dm.dmPelsWidth != last_width  &&  dm.dmPelsHeight != last_height && dm.dmBitsPerPel == 32 )
		{
			std::vector<unsigned int> resolution(2,0);
			last_width = dm.dmPelsWidth;
			last_height = dm.dmPelsHeight;
			resolution[0] = dm.dmPelsWidth;
			resolution[1] = dm.dmPelsHeight;
			OPTION_RESOLUTIONS.push_back(resolution);
		}
		++i;
	}
	#endif	/* _WIN32 */
}

void GameLogicManager::load_lua_stuff(){
	TEXEngine::Core::LogManager::get().log_message("Game Log:\tLoading animal data and registering lua variables.");

	//push triggerable events to lua
	Message events(3, 0, 1);
	events.add_integral_data(0, 0);
	events.add_integral_data(1, 1);
	string s("FETCH_MENU_PARAMS");
	events.add_pointer(0, &s);
	events.add_integral_data(2, FETCH_MENU_PARAMS);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "BACK_TO_MENU";
	events.add_pointer(0, &s);
	events.add_integral_data(2, BACK_TO_MENU);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "QUIT_GAME";
	events.add_pointer(0, &s);
	events.add_integral_data(2, QUIT_GAME);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "PLAY_OnClick";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAY_OnClick);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PLAY_OverClick";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAY_OverClick);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PLAY_GO";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAY_GO);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PLAY_UNGO";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAY_UNGO);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PLAY_MENU_MUSIC";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAY_MENU_MUSIC);
	trigger_event(ADD_EVENT_VARIABLE, events);


	s = "SAVE_OPTIONS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SAVE_OPTIONS);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "CANCEL_OPTIONS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, CANCEL_OPTIONS);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "REQUEST_WHIP_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_WHIP_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_WHIP_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_WHIP_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_TREAT_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_TREAT_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_TREAT_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_TREAT_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_BRAKE_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_BRAKE_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_BRAKE_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_BRAKE_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_LEFT_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_LEFT_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_LEFT_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_LEFT_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_RIGHT_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_RIGHT_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_RIGHT_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_RIGHT_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_NEXTCAMERA_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_NEXTCAMERA_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_PREVIOUSCAMERA_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_PREVIOUSCAMERA_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_RESETPOSITION_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_RESETPOSITION_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_REARVIEW_PCKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_REARVIEW_PCKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REQUEST_REARVIEW_CONTROLLERKEY_REBIND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, REQUEST_REARVIEW_CONTROLLERKEY_REBIND);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "WHIP_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, WHIP_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "WHIP_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, WHIP_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "TREAT_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, TREAT_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "TREAT_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, TREAT_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "BRAKE_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, BRAKE_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "BRAKE_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, BRAKE_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "LEFT_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, LEFT_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "LEFT_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, LEFT_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "RIGHT_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, RIGHT_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "RIGHT_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, RIGHT_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "NEXT_CAMERA_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, NEXT_CAMERA_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "NEXT_CAMERA_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, NEXT_CAMERA_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PREVIOUS_CAMERA_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PREVIOUS_CAMERA_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "RESET_POSITION_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2, RESET_POSITION_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "RESET_POSITION_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2,RESET_POSITION_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REARVIEW_PCKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2,REARVIEW_PCKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "REARVIEW_CONTROLLERKEY_REBOUND";
	events.add_pointer(0, &s);
	events.add_integral_data(2,REARVIEW_CONTROLLERKEY_REBOUND);
	trigger_event(ADD_EVENT_VARIABLE, events);
	




	s = "CANCEL_KEYBIND_CHANGE";
	events.add_pointer(0, &s);
	events.add_integral_data(2, CANCEL_KEYBIND_CHANGE);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	s = "KEYBIND_UNBOUND_MESSAGE";
	events.add_pointer(0, &s);
	events.add_integral_data(2, KEYBIND_UNBOUND_MESSAGE);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE
	
	s = "END_SPLASH1";
	events.add_pointer(0, &s);
	events.add_integral_data(2, END_SPLASH1);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE	
	s = "SEND_SCORES";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SEND_SCORES);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE	
	
	

	s = "TOGGLE_CONSOLE";
	events.add_pointer(0, &s);
	events.add_integral_data(2, TOGGLE_CONSOLE);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE		
	s = "UPDATE_LOG";
	events.add_pointer(0, &s);
	events.add_integral_data(2, UPDATE_LOG);
	trigger_event(ADD_EVENT_VARIABLE, events); //even though this event will not be triggered in lua, we still need to use ADD_EVENT_VARIABLE		
	

	s = "SET_SOUND_VOLUME";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SET_SOUND_VOLUME);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "SET_MUSIC_VOLUME";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SET_MUSIC_VOLUME);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "RESIZE_WINDOWS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, RESIZE_WINDOWS);
	trigger_event(ADD_EVENT_VARIABLE, events); 

	s = "PLAYER_STATS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAYER_STATS);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "SCENE_ISPAUSED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SCENE_ISPAUSED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "SCENE_ISRESUMED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SCENE_ISRESUMED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "SCENE_FINISHED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SCENE_FINISHED);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "SCENE_QUIT";
	events.add_pointer(0, &s);
	events.add_integral_data(2, SCENE_QUIT);
	trigger_event(ADD_EVENT_VARIABLE, events);
	s = "PLAYER_POSITION";
	events.add_pointer(0, &s);
	events.add_integral_data(2, PLAYER_POSITION);
	trigger_event(ADD_EVENT_VARIABLE, events);
	

	s = "WHIP_HAPPENED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, WHIP_HAPPENED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "TREAT_HAPPENED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, TREAT_HAPPENED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "BRAKE_HAPPENED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, BRAKE_HAPPENED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "LEFT_HAPPENED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, LEFT_HAPPENED);
	trigger_event(ADD_EVENT_VARIABLE, events); 
	s = "RIGHT_HAPPENED";
	events.add_pointer(0, &s);
	events.add_integral_data(2, RIGHT_HAPPENED);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "ENABLE_MENU_KEYS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, ENABLE_MENU_KEYS);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "DISABLE_MENU_KEYS";
	events.add_pointer(0, &s);
	events.add_integral_data(2, DISABLE_MENU_KEYS);
	trigger_event(ADD_EVENT_VARIABLE, events);

	s = "TEST_ASPECT_RATIO";
	events.add_pointer(0, &s);
	events.add_integral_data(2, TEST_ASPECT_RATIO);
	trigger_event(ADD_EVENT_VARIABLE, events); 

	s = "CONSOLE_LOG";
	events.add_pointer(0, &s);
	events.add_integral_data(2, CONSOLE_LOG);
	trigger_event(ADD_EVENT_VARIABLE, events); 


	Message msg(0, 0, 2);
	std::string versionString((const char*)XSLEDDING_VERSION);
	s = "GAME_VERSION";
	msg.add_pointer(0, &s);
	msg.add_pointer(1, &versionString);
	trigger_event(ADD_STRING_VARIABLE, msg);


	load_track_data();
	load_animal_data();
	push_game_variables_to_lua();

	//if LOAD_SCRIPT is used then it doesn't matter if the load_lua_scripts function is called last, first, or whatever
	//however if LOAD_AND_RUN_SCRIPT is used then load_lua_scripts has to be called last
	load_lua_scripts();	
}

void GameLogicManager::load_animal_data() const{
	NCF1 animals;
	animals.set_source(PATH_STUFF"data/scripts/ncf/AnimalList.ncf1");
	if(animals.parse() == 0){
		unsigned int min_count = UINT_MAX;

		for(unsigned int i = 0; i < animals.count_groups(); ++i){
			if(animals.group(i) && animals.group(i)->count_groups() < min_count)
				min_count = animals.group(i)->count_groups();

			for(unsigned int j = 0; animals.group(i) && j < animals.group(i)->count_groups(); ++j){
				stringstream output;
				NCF1* group = animals.group(i)->group(j);
				const char* n = group->name();

				double e, d, m;
				if(sscanf(group->get("E"), "%Lf", &e) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the E value into a double for animal type %s named %s.",animals.get_source(),animals.group(i)->name(),n);

				if(sscanf(group->get("D"), "%Lf", &d) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the D value into a double for animal type %s named %s.",animals.get_source(),animals.group(i)->name(),n);

				if(sscanf(group->get("M"), "%Lf", &m) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the M value into a double for animal type %s named %s.",animals.get_source(),animals.group(i)->name(),n);


				string s1("");
				string s2(n);

				output.clear();
				output << animals.group(i)->name() << j << "NAME";
				output >> s1;
				output.flush();
				Message msg(0, 1, 2);
				msg.add_pointer(0, &s1);
				msg.add_pointer(1, &s2);
				trigger_event(ADD_STRING_VARIABLE, msg);

				output.clear();
				output << animals.group(i)->name() << j << "E";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, e);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << animals.group(i)->name() << j << "D";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, d);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << animals.group(i)->name() << j << "M";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, m);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);
			}
		}

		Message msgg(3, 0, 1);
		string cptstring("ANIMAL_INDIVIDUALS_COUNT");
		msgg.add_integral_data(0, 0);
		msgg.add_integral_data(1, 0);
		msgg.add_integral_data(2, min_count);
		msgg.add_pointer(0, &cptstring);
		trigger_event(ADD_UINT_VARIABLE, msgg);

	}
}

void GameLogicManager::load_track_data() const{
	NCF1 courses;
	courses.set_source(PATH_STUFF"data/scripts/ncf/courses.ncf1");
	if(courses.parse() == 0){
		unsigned int min_count = UINT_MAX;
		NCF1* tracks = courses.group("tracks");
		if(tracks){
			for(unsigned int i = 0; i < tracks->count_groups(); ++i){

				stringstream output;
				NCF1* track = tracks->group(i);
				
				double textposx = 0.0, textposy = 0.0, textwidth = 0.0, textheight = 0.0;
				unsigned int texid = 0, audioid = 0, groundid = 96, scenerystart = 200;
				unsigned int godmode = 0;

				if(sscanf(track->get("textposx"), "%Lf", &textposx) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the textposx value into a double for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("textposy"), "%Lf", &textposy) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the textposy value into a double for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("textwidth"), "%Lf", &textwidth) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the textwidth value into a double for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("textheight"), "%Lf", &textheight) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the textheight value into a double for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("texid"), "%Ld", &texid) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the texid value into an unsigned int for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("audioid"), "%Ld", &audioid) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the audioid value into an unsigned int for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("groundEntityId"), "%Ld", &groundid) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the groundEntityId value into an unsigned int for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("sceneryStartEntityIndex"), "%Ld", &scenerystart) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the sceneryStartEntityIndex value into an unsigned int for track named %s.",courses.get_source(), track->name());

				if(sscanf(track->get("godmode"), "%Ld", &godmode) == EOF)
					TEXEngine::Core::LogManager::get().log_error("Parse error in '%s'. Couldn't parse the godmode value into an unsigned int for track named %s.",courses.get_source(), track->name());




				string s1("");
				string s2("");

				output.clear();
				output << "TRACK" << i << "NAME";
				output >> s1;
				output.flush();
				s2 = track->get("name");
				Message msg(3, 1, 2);
				msg.add_pointer(0, &s1);
				msg.add_pointer(1, &s2);
				trigger_event(ADD_STRING_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "DESCRIPTION";
				output >> s1;
				output.flush();
				s2 = track->get("description");
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_pointer(1, &s2);
				trigger_event(ADD_STRING_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "DATA";
				output >> s1;
				output.flush();
				s2 = track->get("data");
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_pointer(1, &s2);
				trigger_event(ADD_STRING_VARIABLE, msg);				

				output.clear();
				output << "TRACK" << i << "TEXTPOSX";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, textposx);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "TEXTPOSY";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, textposy);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "TEXTWIDTH";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, textwidth);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "TEXTHEIGHT";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_real_data(0, textheight);
				trigger_event(ADD_DOUBLE_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "TEXID";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_integral_data(2, texid);
				trigger_event(ADD_UINT_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "AUDIOID";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_integral_data(2, audioid);
				trigger_event(ADD_UINT_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "GROUNDID";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_integral_data(2, groundid);
				trigger_event(ADD_UINT_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "SCENERYSTART";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_integral_data(2, scenerystart);
				trigger_event(ADD_UINT_VARIABLE, msg);

				output.clear();
				output << "TRACK" << i << "GODMODE";
				output >> s1;
				output.flush();
				msg.clear_pointers();
				msg.clear_real_data();
				msg.clear_integral_data();
				msg.add_pointer(0, &s1);
				msg.add_integral_data(2, godmode);
				trigger_event(ADD_UINT_VARIABLE, msg);

				
			}

			Message msgg(3, 0, 1);
			string cptstring("TRACKS_COUNT");
			msgg.add_integral_data(0, 0);
			msgg.add_integral_data(1, 0);
			msgg.add_integral_data(2, tracks->count_groups());
			msgg.add_pointer(0, &cptstring);
			trigger_event(ADD_UINT_VARIABLE, msgg);

		}
	}
}

void GameLogicManager::load_lua_scripts() const{
	Message parameters(2,0,1);
	parameters.add_integral_data(0,0);
	parameters.add_integral_data(1,0);
	
	string scriptsBaseDir(PATH_STUFF"data/scripts/lua/");
	string platformSpecificDir;
#ifdef _WIN32
	platformSpecificDir = "pc/";
#endif

	NCF1* commonScripts = m_config.group("common_lua");
	if(commonScripts){
		for(unsigned int i = 0; i < commonScripts->count_properties(); ++i){
			string script(scriptsBaseDir+"common/"+commonScripts->get(i));
			parameters.add_pointer(0, static_cast<void*>(&script));
			trigger_event(LOAD_SCRIPT, parameters);
		}
	}

	NCF1* specificScripts = m_config.group("platform_specific_lua");
	if(specificScripts){
		for(unsigned int i = 0; i < specificScripts->count_properties(); ++i){
			string script(scriptsBaseDir+platformSpecificDir+specificScripts->get(i));
			parameters.add_pointer(0, static_cast<void*>(&script));
			trigger_event(LOAD_SCRIPT, parameters);
		}
	}
}

void GameLogicManager::push_game_variables_to_lua(){

	string s1("RACETYPE_PROPERTY_SOLO");
	string s2(RACETYPE_PROPERTY_SOLO);
	Message msg(0, 0, 2);
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "RACETYPE_PROPERTY_VERSUS";
	s2 = RACETYPE_PROPERTY_VERSUS;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);

	s1 = "COURSETYPE_PROPERTY_SNOW";
	s2 = COURSETYPE_PROPERTY_SNOW;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "COURSETYPE_PROPERTY_ICE";
	s2 = COURSETYPE_PROPERTY_ICE;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "COURSETYPE_PROPERTY_DESERT";
	s2 = COURSETYPE_PROPERTY_DESERT;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);

	s1 = "SPECIES_PROPERTY_DOG";
	s2 = SPECIES_PROPERTY_DOG;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "SPECIES_PROPERTY_MOUSE";
	s2 = SPECIES_PROPERTY_MOUSE;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "SPECIES_PROPERTY_DINO";
	s2 = SPECIES_PROPERTY_DINO;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);

	s1 = "DIFFICULTY_PROPERTY_EASY";
	s2 = DIFFICULTY_PROPERTY_EASY;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);
	s1 = "DIFFICULTY_PROPERTY_HARD";
	s2 = DIFFICULTY_PROPERTY_HARD;
	msg.add_pointer(0, &s1);
	msg.add_pointer(1, &s2);
	trigger_event(ADD_STRING_VARIABLE, msg);

	Message m(3, 0, 1);
	s1 = "RESOLUTIONS_COUNT";
	m.add_integral_data(0, 0);
	m.add_integral_data(1, 1);
	m.add_integral_data(2, OPTION_RESOLUTIONS.size());
	m.add_pointer(0, &s1);
	trigger_event(ADD_UINT_VARIABLE, m);

	if( OPTION_RESOLUTIONS.size() > 0 ){
		stringstream output;

		for(unsigned int i = 0; i < OPTION_RESOLUTIONS.size(); ++i){
			output.clear();
			output << "RESOLUTION" << i << "WIDTH";
			output >> s1;
			output.flush();
			m.clear_pointers();
			m.add_pointer(0, &s1);
			m.add_integral_data(2, OPTION_RESOLUTIONS[i][0]);
			trigger_event(ADD_UINT_VARIABLE, m);
			output.clear();
			output << "RESOLUTION" << i << "HEIGHT";
			output >> s1;
			output.flush();
			m.clear_pointers();
			m.add_pointer(0, &s1);
			m.add_integral_data(2, OPTION_RESOLUTIONS[i][1]);
			trigger_event(ADD_UINT_VARIABLE, m);
		}
	}
	NCF1* config = m_config.group("config");
	if(config){
		Message m(3, 0, 1);
		m.add_integral_data(0, 0);
		m.add_integral_data(1, 1);

		s1 = "CURRENT_RESOLUTION_VALUE";
		unsigned int value;
		if(sscanf(config->get("resolution"), "%Ld", &value) == EOF){
			value = 1;
		}
		m.add_integral_data(2, value);
		m.add_pointer(0, &s1);
		trigger_event(ADD_UINT_VARIABLE, m);

		s1 = "CURRENT_QUALITY_VALUE";
		if(sscanf(config->get("quality"), "%Ld", &value) == EOF){
			value = 1;
		}
		m.add_integral_data(2, value);
		m.add_pointer(0, &s1);
		trigger_event(ADD_UINT_VARIABLE, m);

		s1 = "CURRENT_MUSIC_VALUE";
		if(sscanf(config->get("music"), "%Ld", &value) == EOF){
			value = 50;
		}
		m.add_integral_data(2, value);
		m.add_pointer(0, &s1);
		trigger_event(ADD_UINT_VARIABLE, m);

		s1 = "CURRENT_SOUND_VALUE";
		if(sscanf(config->get("sound"), "%Ld", &value) == EOF){
			value = 50;
		}
		m.add_integral_data(2, value);
		m.add_pointer(0, &s1);
		trigger_event(ADD_UINT_VARIABLE, m);

		s1 = "CURRENT_FULLSCREEN_VALUE";
		if(sscanf(config->get("fullscreen"), "%Ld", &value) == EOF){
			value = 0;
		}
		m.add_integral_data(2, value);
		m.add_pointer(0, &s1);
		trigger_event(ADD_UINT_VARIABLE, m);
	}
	NCF1* keyBindings = m_config.group("keybindings");
	if(keyBindings ){
		#ifdef	_WIN32
			if ( sscanf(keyBindings->get("whip_key"), "%Ld", &m_whip_value) == EOF){
				m_whip_value = W;
			}
			m_previous_whip_value = m_whip_value;

			if ( sscanf(keyBindings->get("whip_modifier"), "%Ld", &m_whip_mod_value) == EOF){
				m_whip_mod_value = NONE;
			}
			m_previous_whip_mod_value = m_whip_mod_value;

			if ( sscanf(keyBindings->get("treat_key"), "%Ld", &m_treat_value) == EOF){
				m_treat_value = SPACE;
			}
			m_previous_treat_value = m_treat_value;

			if ( sscanf(keyBindings->get("treat_modifier"), "%Ld", &m_treat_mod_value) == EOF){
				m_treat_mod_value = NONE;
			}
			m_previous_treat_mod_value = m_treat_mod_value;

			if ( sscanf(keyBindings->get("brake_key"), "%Ld", &m_brake_value) == EOF){
				m_brake_value = S;
			}
			m_previous_brake_value = m_brake_value;

			if ( sscanf(keyBindings->get("brake_modifier"), "%Ld", &m_brake_mod_value) == EOF){
				m_brake_mod_value = NONE;
			}
			m_previous_brake_mod_value = m_brake_mod_value;

			if ( sscanf(keyBindings->get("left_key"), "%Ld", &m_left_value) == EOF){
				m_left_value = A;
			}
			m_previous_left_value = m_left_value;

			if ( sscanf(keyBindings->get("left_modifier"), "%Ld", &m_left_mod_value) == EOF){
				m_left_mod_value = NONE;
			}
			m_previous_left_mod_value = m_left_mod_value;

			if ( sscanf(keyBindings->get("right_key"), "%Ld", &m_right_value) == EOF){
				m_right_value = D;
			}
			m_previous_right_value = m_right_value;

			if ( sscanf(keyBindings->get("right_modifier"), "%Ld", &m_right_mod_value) == EOF){
				m_right_mod_value = NONE;
			}
			m_previous_right_mod_value = m_right_mod_value;

			if ( sscanf(keyBindings->get("next_camera_key"), "%Ld", &m_next_camera_value) == EOF){
				m_next_camera_value = C;
			}
			m_previous_next_camera_value = m_next_camera_value;

			if ( sscanf(keyBindings->get("next_camera_modifier"), "%Ld", &m_next_camera_mod_value) == EOF){
				m_next_camera_mod_value = NONE;
			}
			m_previous_next_camera_mod_value = m_next_camera_mod_value;

			if ( sscanf(keyBindings->get("previous_camera_key"), "%Ld", &m_previous_camera_value) == EOF){
				m_previous_camera_value = V;
			}
			m_previous_previous_camera_value = m_previous_camera_value;

			if ( sscanf(keyBindings->get("previous_camera_modifier"), "%Ld", &m_previous_camera_mod_value) == EOF){
				m_previous_camera_mod_value = NONE;
			}
			m_previous_previous_camera_mod_value = m_previous_camera_mod_value;

			if ( sscanf(keyBindings->get("rearview_camera_key"), "%Ld", &m_rearview_camera_value) == EOF){
				m_rearview_camera_value = E;
			}
			m_previous_rearview_camera_value = m_rearview_camera_value;

			if ( sscanf(keyBindings->get("rearview_camera_modifier"), "%Ld", &m_rearview_camera_mod_value) == EOF){
				m_rearview_camera_mod_value = NONE;
			}
			m_previous_rearview_camera_mod_value = m_rearview_camera_mod_value;

			if ( sscanf(keyBindings->get("reset_position_key"), "%Ld", &m_reset_position_value) == EOF){
				m_reset_position_value = R;
			}
			m_previous_reset_position_value = m_reset_position_value;

			if ( sscanf(keyBindings->get("reset_position_modifier"), "%Ld", &m_reset_position_mod_value) == EOF){
				m_reset_position_mod_value = NONE;
			}
			m_previous_reset_position_mod_value = m_reset_position_mod_value;

		#endif	/* _WIN32 */

		if ( sscanf(keyBindings->get("cwhip_key"), "%Ld", &m_cwhip_value) == EOF){
			#ifdef	_WIN32
				m_cwhip_value = XB_A;
			#endif	/* _WIN32 */
		}
		m_previous_cwhip_value = m_cwhip_value;

		if ( sscanf(keyBindings->get("ctreat_key"), "%Ld", &m_ctreat_value) == EOF){
			#ifdef	_WIN32
				m_ctreat_value = XB_X;
			#endif	/* _WIN32 */
		}
		m_previous_ctreat_value = m_ctreat_value;

		if ( sscanf(keyBindings->get("cbrake_key"), "%Ld", &m_cbrake_value) == EOF){
			#ifdef	_WIN32
				m_cbrake_value = XB_B;
			#endif	/* _WIN32 */
		}
		m_previous_cbrake_value = m_cbrake_value;

		if ( sscanf(keyBindings->get("cleft_key"), "%Ld", &m_cleft_value) == EOF){	
			#ifdef	_WIN32
				m_cleft_value = DPAD_LEFT;
			#endif	/* _WIN32 */
		}
		m_previous_cleft_value = m_cleft_value;

		if ( sscanf(keyBindings->get("cright_key"), "%Ld", &m_cright_value) == EOF){
			#ifdef	_WIN32
				m_cright_value = DPAD_RIGHT;
			#endif	/* _WIN32 */
		}
		m_previous_cright_value = m_cright_value;

		if ( sscanf(keyBindings->get("cnext_camera_key"), "%Ld", &m_cnext_camera_value) == EOF){
			#ifdef	_WIN32
				m_cnext_camera_value = RIGHTSHOULDER;
			#endif	/* _WIN32 */
		}
		m_previous_cnext_camera_value = m_cnext_camera_value;

		if ( sscanf(keyBindings->get("cprevious_camera_key"), "%Ld", &m_cprevious_camera_value) == EOF){
			#ifdef	_WIN32
				m_cprevious_camera_value = LEFTSHOULDER;
			#endif	/* _WIN32 */
		}
		m_previous_cprevious_camera_value = m_cprevious_camera_value;

		if ( sscanf(keyBindings->get("crearview_camera_key"), "%Ld", &m_crearview_camera_value) == EOF){
			#ifdef	_WIN32
				m_crearview_camera_value = DPAD_DOWN;
			#endif	/* _WIN32 */
		}
		m_previous_crearview_camera_value = m_crearview_camera_value;

		if ( sscanf(keyBindings->get("creset_position_key"), "%Ld", &m_creset_position_value) == EOF){
			#ifdef	_WIN32
				m_creset_position_value = XB_Y;
			#endif	/* _WIN32 */
		}
		m_previous_creset_position_value = m_creset_position_value;
	}
}


#ifdef	_WIN32
	void GameLogicManager::check_keyboard_conflict( const KeyboardHotkey* changed_key )
	{
		string	text("");
		Message	parameters(0,0,1);



		if ( !changed_key ) return;

		if ( changed_key != m_whip_key  &&  m_whip_key )
		{
			if ( changed_key->get_as_text()  ==  m_whip_key->get_as_text() )
			{
				m_whip_key->keybind(NO_KEYBIND);
				m_whip_key->modifier(NONE);
				m_whip_key->disable();
				text = "The keyboard whip key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_whip_key->get_as_text();
				trigger_event(WHIP_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_treat_key  &&  m_treat_key )
		{
			if ( changed_key->get_as_text()  ==  m_treat_key->get_as_text() )
			{
				m_treat_key->keybind(NO_KEYBIND);
				m_treat_key->modifier(NONE);
				m_treat_key->disable();
				text = "The keyboard treat key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_treat_key->get_as_text();
				trigger_event(TREAT_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_brake_key  &&  m_brake_key )
		{
			if ( changed_key->get_as_text()  ==  m_brake_key->get_as_text() )
			{
				m_brake_key->keybind(NO_KEYBIND);
				m_brake_key->modifier(NONE);
				m_brake_key->disable();
				text = "The keyboard brake key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_brake_key->get_as_text();
				trigger_event(BRAKE_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_left_key  &&  m_left_key )
		{
			if ( changed_key->get_as_text()  ==  m_left_key->get_as_text() )
			{
				m_left_key->keybind(NO_KEYBIND);
				m_left_key->modifier(NONE);
				m_left_key->disable();
				text = "The keyboard left key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_left_key->get_as_text();
				trigger_event(LEFT_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_right_key  &&  m_right_key )
		{
			if ( changed_key->get_as_text()  ==  m_right_key->get_as_text() )
			{
				m_right_key->keybind(NO_KEYBIND);
				m_right_key->modifier(NONE);
				m_right_key->disable();
				text = "The keyboard right key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_right_key->get_as_text();
				trigger_event(RIGHT_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_next_camera_key  &&  m_next_camera_key )
		{
			if ( changed_key->get_as_text() == m_next_camera_key->get_as_text() )
			{
				m_next_camera_key->keybind(NO_KEYBIND);
				m_next_camera_key->modifier(NONE);
				m_next_camera_key->disable();
				text = "The keyboard next camera key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_next_camera_key->get_as_text();
				trigger_event(NEXT_CAMERA_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_previous_camera_key  &&  m_previous_camera_key )
		{
			if ( changed_key->get_as_text() == m_previous_camera_key->get_as_text() )
			{
				m_previous_camera_key->keybind(NO_KEYBIND);
				m_previous_camera_key->modifier(NONE);
				m_previous_camera_key->disable();
				text = "The keyboard previous camera key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_previous_camera_key->get_as_text();
				trigger_event(PREVIOUS_CAMERA_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_rearview_camera_key  &&  m_rearview_camera_key )
		{
			if ( changed_key->get_as_text() == m_rearview_camera_key->get_as_text() )
			{
				m_rearview_camera_key->keybind(NO_KEYBIND);
				m_rearview_camera_key->modifier(NONE);
				m_rearview_camera_key->disable();
				text = "The keyboard rear view camera key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_rearview_camera_key->get_as_text();
				trigger_event(REARVIEW_PCKEY_REBOUND,parameters);
			}
		}

		if ( changed_key != m_reset_position_key  &&  m_reset_position_key )
		{
			if ( changed_key->get_as_text() == m_reset_position_key->get_as_text() )
			{
				m_reset_position_key->keybind(NO_KEYBIND);
				m_reset_position_key->modifier(NONE);
				m_reset_position_key->disable();
				text = "The keyboard reset position key got unbound!";
				parameters.add_pointer(0,static_cast<void*>(&text));
				trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
				text = m_reset_position_key->get_as_text();
				trigger_event(RESET_POSITION_PCKEY_REBOUND,parameters);
			}
		}
	}
#endif	/* _WIN32 */

void GameLogicManager::check_controller_conflict( const ControllerHotkey* changed_key )
{
	string	text("");
	Message	parameters(0,0,1);



	if ( !changed_key ) return ;

	if ( changed_key != m_cwhip_key   &&  m_cwhip_key )
	{
		if ( changed_key->get_as_text()  ==  m_cwhip_key->get_as_text() )
		{
			m_cwhip_key->keybind(NO_KEYBIND);
			m_cwhip_key->disable();
			text = "The controller whip key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cwhip_key->get_as_text();
			trigger_event(WHIP_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_ctreat_key   &&  m_ctreat_key )
	{
		if ( changed_key->get_as_text()  ==  m_ctreat_key->get_as_text() )
		{
			m_ctreat_key->keybind(NO_KEYBIND);
			m_ctreat_key->disable();
			text = "The controller treat key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_ctreat_key->get_as_text();
			trigger_event(TREAT_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_cbrake_key   &&  m_cbrake_key )
	{
		if ( changed_key->get_as_text()  ==  m_cbrake_key->get_as_text() )
		{
			m_cbrake_key->keybind(NO_KEYBIND);
			m_cbrake_key->disable();
			text = "The controller brake key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cbrake_key->get_as_text();
			trigger_event(BRAKE_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_cleft_key   &&  m_cleft_key )
	{
		if ( changed_key->get_as_text()  ==  m_cleft_key->get_as_text() )
		{
			m_cleft_key->keybind(NO_KEYBIND);
			m_cleft_key->disable();
			text = "The controller left key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cleft_key->get_as_text();
			trigger_event(LEFT_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_cright_key   &&  m_cright_key )
	{
		if ( changed_key->get_as_text()  ==  m_cright_key->get_as_text() )
		{
			m_cright_key->keybind(NO_KEYBIND);
			m_cright_key->disable();
			text = "The controller right key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cright_key->get_as_text();
			trigger_event(RIGHT_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_cnext_camera_key   &&  m_cnext_camera_key )
	{
		if ( changed_key->get_as_text()  ==  m_cnext_camera_key->get_as_text() )
		{
			m_cnext_camera_key->keybind(NO_KEYBIND);
			m_cnext_camera_key->disable();
			text = "The controller next camera key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cnext_camera_key->get_as_text();
			trigger_event(NEXT_CAMERA_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_cprevious_camera_key   &&  m_cprevious_camera_key )
	{
		if ( changed_key->get_as_text()  ==  m_cprevious_camera_key->get_as_text() )
		{
			m_cprevious_camera_key->keybind(NO_KEYBIND);
			m_cprevious_camera_key->disable();
			text = "The controller previous camera key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_cprevious_camera_key->get_as_text();
			trigger_event(PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_crearview_camera_key   &&  m_crearview_camera_key )
	{
		if ( changed_key->get_as_text()  ==  m_crearview_camera_key->get_as_text() )
		{
			m_crearview_camera_key->keybind(NO_KEYBIND);
			m_crearview_camera_key->disable();
			text = "The controller rear view camera key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_crearview_camera_key->get_as_text();
			trigger_event(REARVIEW_CONTROLLERKEY_REBOUND,parameters);
		}
	}

	if ( changed_key != m_creset_position_key   &&  m_creset_position_key )
	{
		if ( changed_key->get_as_text()  ==  m_creset_position_key->get_as_text() )
		{
			m_creset_position_key->keybind(NO_KEYBIND);
			m_creset_position_key->disable();
			text = "The controller reset position key got unbound!";
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(KEYBIND_UNBOUND_MESSAGE,parameters);
			text = m_creset_position_key->get_as_text();
			trigger_event(RESET_POSITION_CONTROLLERKEY_REBOUND,parameters);
		}
	}
}

void GameLogicManager::send_keybind_texts()
{
	Message	parameters(0,0,1);
	string	text("");

	#ifdef	_WIN32
		
		if ( m_whip_key )
		{
			text = m_whip_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(WHIP_PCKEY_REBOUND,parameters);
		}

		if ( m_treat_key )
		{
			text = m_treat_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(TREAT_PCKEY_REBOUND,parameters);
		}

		if ( m_brake_key )
		{
			text = m_brake_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(BRAKE_PCKEY_REBOUND,parameters);
		}

		if ( m_left_key )
		{
			text = m_left_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(LEFT_PCKEY_REBOUND,parameters);
		}

		if ( m_right_key)
		{
			text = m_right_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(RIGHT_PCKEY_REBOUND,parameters);
		}

		if ( m_next_camera_key )
		{
			text = m_next_camera_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(NEXT_CAMERA_PCKEY_REBOUND,parameters);
		}

		if ( m_previous_camera_key )
		{
			text = m_previous_camera_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(PREVIOUS_CAMERA_PCKEY_REBOUND,parameters);
		}

		if ( m_rearview_camera_key )
		{
			text = m_rearview_camera_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(REARVIEW_PCKEY_REBOUND,parameters);
		}

		if ( m_reset_position_key )
		{
			text = m_reset_position_key->get_as_text();
			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(RESET_POSITION_PCKEY_REBOUND,parameters);
		}

	#endif	/* _WIN32 */

	if ( m_cwhip_key )
	{
		text = m_cwhip_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(WHIP_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_ctreat_key )
	{
		text = m_ctreat_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(TREAT_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_cbrake_key )
	{
		text = m_cbrake_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(BRAKE_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_cleft_key )
	{
		text = m_cleft_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(LEFT_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_cright_key )
	{
		text = m_cright_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(RIGHT_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_cnext_camera_key )
	{
		text = m_cnext_camera_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(NEXT_CAMERA_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_cprevious_camera_key )
	{
		text = m_cprevious_camera_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_crearview_camera_key )
	{
		text = m_crearview_camera_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(REARVIEW_CONTROLLERKEY_REBOUND,parameters);
	}

	if ( m_creset_position_key )
	{
		text = m_creset_position_key->get_as_text();
		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(RESET_POSITION_CONTROLLERKEY_REBOUND,parameters);
	}
}

bool GameLogicManager::key_change_active()
{

		return (	
					#ifdef	_WIN32
						m_rebind_whip  ||  m_rebind_treat  ||  m_rebind_brake  ||  m_rebind_left  ||  m_rebind_right  ||  m_rebind_next_camera  ||  m_rebind_previous_camera  ||  m_rebind_rearview_camera  ||  m_rebind_reset_position  ||  
					#endif	/* _WIN32 */
						
					m_rebind_cwhip  ||  m_rebind_ctreat  ||  m_rebind_cbrake  ||  m_rebind_cleft  ||  m_rebind_cright  ||  m_rebind_cnext_camera  ||  m_rebind_cprevious_camera  ||  m_rebind_crearview_camera  ||  m_rebind_creset_position );		
}

void GameLogicManager::enable_menu_keys()
{
	#ifdef	_WIN32
		if ( m_previous )
			m_previous->enable();
	
		if ( m_next )
			m_next->enable();
	
		if ( m_left )
			m_left->enable();
	
		if ( m_right )
			m_right->enable();
	
		if ( m_select )
			m_select->enable();
	#endif


	if ( m_cprevious )
		m_cprevious->enable();
	
	if ( m_cnext )
		m_cnext->enable();
	
	if ( m_cleft )
		m_cleft->enable();
	
	if ( m_cright )
		m_cright->enable();
	
	if ( m_cselect )
		m_cselect->enable();
}

void GameLogicManager::disable_menu_keys()
{
	#ifdef	_WIN32
		if ( m_previous )
			m_previous->disable();
	
		if ( m_next )
			m_next->disable();
	
		if ( m_left )
			m_left->disable();
	
		if ( m_right )
			m_right->disable();
	
		if ( m_select )
			m_select->disable();
	#endif


	if ( m_cprevious )
		m_cprevious->disable();
	
	if ( m_cnext )
		m_cnext->disable();
	
	if ( m_cleft )
		m_cleft->disable();
	
	if ( m_cright )
		m_cright->disable();
	
	if ( m_cselect )
		m_cselect->disable();
}

void GameLogicManager::init_keys_and_scenes()
{
	TEXEngine::Core::LogManager::get().log_message("Game Log:\tCreating hotkeys!");

	#ifdef	_WIN32

		vector<unsigned long>	keyboard_whitelist(0,0);



		for ( unsigned int i = CANCEL;  i <= OEMCLEAR;  ++i )
		{
			if ( i != m_console_value  &&  i != m_pause_scene_value  &&  Hotkey::check_key(i) )
				keyboard_whitelist.push_back(i);
		}


		if ( m_reload_ui )
			delete m_reload_ui;

		if ( m_previous )
			delete m_previous;

		if ( m_next )
			delete m_next;

		if ( m_left )
			delete m_left;

		if ( m_right )
			delete m_right;

		if ( m_select )
			delete m_select;

		if ( m_ingame_select )
			delete m_ingame_select;

		if ( m_console )
			delete m_console;

		if ( m_cancel_change )
			delete m_cancel_change;

		if ( m_whip_key )
			delete m_whip_key;

		if ( m_treat_key )
			delete m_treat_key;

		if ( m_brake_key )
			delete m_brake_key;

		if ( m_left_key )
			delete m_left_key;

		if ( m_right_key )
			delete m_right_key;

		if ( m_next_camera_key )
			delete m_next_camera_key;

		if ( m_previous_camera_key )
			delete m_previous_camera_key;

		if ( m_rearview_camera_key )
			delete m_rearview_camera_key;

		if ( m_reset_position_key )
			delete m_reset_position_key;


		m_reload_ui = new (nothrow) KeyboardHotkey(100,m_reload_value,m_reload_mod_value,DEFAULT,RELOAD_UI);
		if ( !m_reload_ui )
			TEXEngine::Core::LogManager::get().log_error("Error creating the Reload UI keyboard hotkey!");

		m_previous = new (nothrow) KeyboardHotkey(101,m_menu_up_value,m_menu_up_mod_value,DEFAULT,COMPONENT_NAVIGATE_TO_PREVIOUS);
		if ( !m_previous )
			TEXEngine::Core::LogManager::get().log_error("Error creating the previous element keyboard hotkey!");

		m_next = new (nothrow) KeyboardHotkey(102,m_menu_down_value,m_menu_down_mod_value,DEFAULT,COMPONENT_NAVIGATE_TO_NEXT);
		if ( !m_next )
			TEXEngine::Core::LogManager::get().log_error("Error creating the next element keyboard hotkey!");

		m_left = new (nothrow) KeyboardHotkey(103,m_menu_left_value,m_menu_left_mod_value,SLOW,COMPONENT_ACTION_LEFT);
		if ( !m_left )
			TEXEngine::Core::LogManager::get().log_error("Error creating the left element keyboard hotkey!");
		else
			m_left->continuous_fire(true);

		m_right = new (nothrow) KeyboardHotkey(104,m_menu_right_value,m_menu_right_mod_value,SLOW,COMPONENT_ACTION_RIGHT);
		if ( !m_right )
			TEXEngine::Core::LogManager::get().log_error("Error creating the right element keyboard hotkey!");
		else
			m_right->continuous_fire(true);

		m_select = new (nothrow) KeyboardHotkey(105,m_menu_select_value,m_menu_select_mod_value,DEFAULT,COMPONENT_ACTION_PERFORMED);
		if ( !m_select )
			TEXEngine::Core::LogManager::get().log_error("Error creating the select element keyboard hotkey!");

		m_ingame_select = new (nothrow) KeyboardHotkey(106,m_menu_select_value,m_menu_select_mod_value,DEFAULT,COMPONENT_ACTION_PERFORMED);
		if ( !m_ingame_select )
			TEXEngine::Core::LogManager::get().log_error("Error creating the ingame select element keyboard hotkey!");
		else
			m_ingame_select->disable();

		m_console = new (nothrow) KeyboardHotkey(107,m_console_value,m_console_mod_value,DEFAULT,TOGGLE_CONSOLE);
		if ( !m_console )
			TEXEngine::Core::LogManager::get().log_error("Error creating the console keyboard hotkey!");

		m_cancel_change = new (nothrow) KeyboardHotkey(108,m_menu_cancel_value,m_menu_cancel_mod_value,DEFAULT,CANCEL_KEYBIND_CHANGE);
		if ( !m_cancel_change )
			TEXEngine::Core::LogManager::get().log_error("Error creating the cancel keybind change keyboard hotkey!");
		else
			m_cancel_change->disable();

		m_whip_key = new (nothrow) KeyboardHotkey(109,m_whip_value,m_whip_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_whip_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the whip keybind change keyboard hotkey!");
		else
			m_whip_key->disable();

		m_treat_key = new (nothrow) KeyboardHotkey(110,m_treat_value,m_treat_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_treat_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the treat keybind change keyboard hotkey!");
		else
			m_treat_key->disable();

		m_brake_key = new (nothrow) KeyboardHotkey(111,m_brake_value,m_brake_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_brake_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the break keybind change keyboard hotkey!");
		else
			m_brake_key->disable();

		m_left_key = new (nothrow) KeyboardHotkey(112,m_left_value,m_left_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_left_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the left keybind change keyboard hotkey!");
		else
			m_left_key->disable();

		m_right_key = new (nothrow) KeyboardHotkey(113,m_right_value,m_right_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_right_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the right keybind change keyboard hotkey!");
		else
			m_right_key->disable();

		m_next_camera_key = new (nothrow) KeyboardHotkey(114,m_next_camera_value,m_next_camera_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_next_camera_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the next camera change keyboard hotkey!");
		else
			m_next_camera_key->disable();

		m_previous_camera_key = new (nothrow) KeyboardHotkey(115,m_previous_camera_value,m_previous_camera_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_previous_camera_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the previous camera change keyboard hotkey!");
		else
			m_previous_camera_key->disable();

		m_rearview_camera_key = new (nothrow) KeyboardHotkey(116,m_rearview_camera_value,m_rearview_camera_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_rearview_camera_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the rear view camera change keyboard hotkey!");
		else
			m_rearview_camera_key->disable();

		m_reset_position_key = new (nothrow) KeyboardHotkey(117,m_reset_position_value,m_reset_position_mod_value,DEFAULT,0,keyboard_whitelist);
		if ( !m_reset_position_key )
			TEXEngine::Core::LogManager::get().log_error("Error creating the reset position change keyboard hotkey!");
		else
			m_reset_position_key->disable();

	#endif


	if ( m_cprevious )
		delete m_cprevious;

	if ( m_cnext )
		delete m_cnext;

	if ( m_cleft )
		delete m_cleft;

	if ( m_cright )
		delete m_cright;

	if ( m_cselect )
		delete m_cselect;

	if ( m_cingame_select )
		delete m_cingame_select;

	if ( m_cconsole )
		delete m_cconsole;

	if ( m_ccancel_change )
			delete m_ccancel_change;

	if ( m_cwhip_key )
		delete m_cwhip_key;

	if ( m_ctreat_key )
		delete m_ctreat_key;

	if ( m_cbrake_key )
		delete m_cbrake_key;

	if ( m_cleft_key )
		delete m_cleft_key;

	if ( m_cright_key )
		delete m_cright_key;

	if ( m_cnext_camera_key )
		delete m_cnext_camera_key;

	if ( m_cprevious_camera_key )
		delete m_cprevious_camera_key;

	if ( m_crearview_camera_key )
		delete m_crearview_camera_key;

	if ( m_creset_position_key )
		delete m_creset_position_key;

	
	vector<unsigned long>	controller_whitelist(0,0);


	#ifdef	_WIN32
		for ( unsigned int i = DPAD_UP;  i <= XB_Y;  ++i )
	#endif
	{
		if ( i != m_cpause_scene_value  &&  i != m_cconsole_value  &&  Hotkey::check_key(i) )
			controller_whitelist.push_back(i);
	}


	m_cprevious = new (nothrow) ControllerHotkey(130,0,m_cmenu_up_value,DEFAULT,COMPONENT_NAVIGATE_TO_PREVIOUS);
	if ( !m_cprevious )
		TEXEngine::Core::LogManager::get().log_error("Error creating the previous element controller hotkey!");

	m_cnext = new (nothrow) ControllerHotkey(131,0,m_cmenu_down_value,DEFAULT,COMPONENT_NAVIGATE_TO_NEXT);
	if ( !m_cnext )
		TEXEngine::Core::LogManager::get().log_error("Error creating the next element controller hotkey!");

	m_cleft = new (nothrow) ControllerHotkey(132,0,m_cmenu_left_value,SLOW,COMPONENT_ACTION_LEFT);
	if ( !m_cleft )
		TEXEngine::Core::LogManager::get().log_error("Error creating the left element controller hotkey!");
	else
		m_cleft->continuous_fire(true);

	m_cright = new (nothrow) ControllerHotkey(133,0,m_cmenu_right_value,SLOW,COMPONENT_ACTION_RIGHT);
	if ( !m_cright )
		TEXEngine::Core::LogManager::get().log_error("Error creating the right element controller hotkey!");
	else
		m_cright->continuous_fire(true);

	m_cselect = new (nothrow) ControllerHotkey(134,0,m_cmenu_select_value,DEFAULT,COMPONENT_ACTION_PERFORMED);
	if ( !m_cselect )
		TEXEngine::Core::LogManager::get().log_error("Error creating the select element controller hotkey!");

	m_cingame_select = new (nothrow) ControllerHotkey(135,0,m_cmenu_select_value,DEFAULT,COMPONENT_ACTION_PERFORMED);
	if ( !m_cingame_select )
		TEXEngine::Core::LogManager::get().log_error("Error creating the ingame select element controller hotkey!");
	else
		m_cingame_select->disable();

	m_cconsole = new (nothrow) ControllerHotkey(136,0,m_cconsole_value,DEFAULT,TOGGLE_CONSOLE);
	if ( !m_cconsole )
		TEXEngine::Core::LogManager::get().log_error("Error creating the console controller hotkey!");

	m_ccancel_change = new (nothrow) ControllerHotkey(137,0,m_cmenu_cancel_value,DEFAULT,CANCEL_KEYBIND_CHANGE);
	if ( !m_ccancel_change )
		TEXEngine::Core::LogManager::get().log_error("Error creating the cancel keybind change controller hotkey!");
	else
		m_ccancel_change->disable();

	m_cwhip_key = new (nothrow) ControllerHotkey(138,0,m_cwhip_value,DEFAULT,0,controller_whitelist);
	if ( !m_cwhip_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the whip keybind change controller hotkey!");
	else
		m_cwhip_key->disable();

	m_ctreat_key = new (nothrow) ControllerHotkey(139,0,m_ctreat_value,DEFAULT,0,controller_whitelist);
	if ( !m_ctreat_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the treat keybind change controller hotkey!");
	else
		m_ctreat_key->disable();

	m_cbrake_key = new (nothrow) ControllerHotkey(140,0,m_cbrake_value,DEFAULT,0,controller_whitelist);
	if ( !m_cbrake_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the break keybind change controller hotkey!");
	else
		m_cbrake_key->disable();

	m_cleft_key = new (nothrow) ControllerHotkey(141,0,m_cleft_value,DEFAULT,0,controller_whitelist);
	if ( !m_cleft_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the left keybind change controller hotkey!");
	else
		m_cleft_key->disable();

	m_cright_key = new (nothrow) ControllerHotkey(142,0,m_cright_value,DEFAULT,0,controller_whitelist);
	if ( !m_cright_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the right keybind change controller hotkey!");
	else
		m_cright_key->disable();

	m_cnext_camera_key = new (nothrow) ControllerHotkey(143,0,m_cnext_camera_value,DEFAULT,0,controller_whitelist);
	if ( !m_cnext_camera_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the next camera change controller hotkey!");
	else
		m_cnext_camera_key->disable();

	m_cprevious_camera_key = new (nothrow) ControllerHotkey(144,0,m_cprevious_camera_value,DEFAULT,0,controller_whitelist);
	if ( !m_cprevious_camera_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the previous camera change controller hotkey!");
	else
		m_cprevious_camera_key->disable();

	m_crearview_camera_key = new (nothrow) ControllerHotkey(145,0,m_crearview_camera_value,DEFAULT,0,controller_whitelist);
	if ( !m_crearview_camera_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the rear view camera change controller hotkey!");
	else
		m_crearview_camera_key->disable();

	m_creset_position_key = new (nothrow) ControllerHotkey(146,0,m_creset_position_value,DEFAULT,0,controller_whitelist);
	if ( !m_creset_position_key )
		TEXEngine::Core::LogManager::get().log_error("Error creating the reset position change controller hotkey!");
	else
		m_creset_position_key->disable();

	enable_menu_keys();



	if(m_soloScene)
		delete m_soloScene;
	if(m_versusScene)
		delete m_versusScene;
	if(m_menuScene)
		delete m_menuScene;



	m_menuScene = new MenuScene(170);
	if(m_menuScene)
		m_menuScene->setup_scene(3);
	m_soloScene = new SoloScene(171);
	if(m_soloScene)
		m_soloScene->setup_scene(1,std::string("SoloScene"));
	m_versusScene = new VersusScene(172);
	if(m_versusScene)
		m_versusScene->setup_scene(2,std::string("VersusScene"));
	
}

void GameLogicManager::setup_physics()
{
	bool return_value = false;
	NCF1 basicEntities;
	basicEntities.set_source(PATH_STUFF"data/scripts/ncf/"BASIC_ENTITIES_FILENAME);
	
	
	if(basicEntities.parse()){
		TEXEngine::Core::LogManager::get().log_error("couldn't parse the basic entities");
	}else{
		if(TEXEngine::Core::AssetManager::get().load(&basicEntities)){
			TEXEngine::Core::LogManager::get().log_error("failed to load the basic entities");
		}else{
			TEXEngine::Core::LogManager::get().log_message("successfully loaded basic entities");
		}
	}


	Entity* sledEntity = AssetManager::get().get_entity(10);
	if(sledEntity && sledEntity->is_collidable()){
		TEXEngine::Core::LogManager::get().log_message("setting physics stuff to the sled");

		// basic physics object info
		PhysObjInfo info;
		info.apply_default();
		info.mass = 800.0f;
		info.colId = PLAYER;
		vector<int> cWI;
		cWI.push_back(PLAYER);
		cWI.push_back(STATIC);
		cWI.push_back(OTHER);
		info.colWithIds = &cWI;
		/*
		if(info.colWithIds)
		{
			info.colWithIds->push_back(PLAYER);
			info.colWithIds->push_back(STATIC);
			info.colWithIds->push_back(OTHER);
		}*/
		

		// vehicle properties
		VehicleProperties vprops;
		vprops.apply_default();
		vprops.m_fWheelRadius = 1.2f;
		vprops.m_fWheelHeight = 1.2f;
		vprops.m_fWheelFriction = 30.0f;
		vprops.m_fSuspStiffness = 20.0f;
		vprops.m_fSuspCompression = 4.4f;
		vprops.m_fSuspDamping = 2.3f;
		vprops.m_fRollInfluence = 0.001f;
		vprops.m_fMaxBrakingForce = 1000.0f;
				
		PhysicsObj* po = PhysicsManager::get().create_vehicle(Vector3f(6.9f,0.5f,1.5f),10,info,vprops);
		if(po)
		{
			sledEntity->physics_obj(po);

			if ( LogicScene::get_player() )
				PhysicsManager::get().register_listener(10,LogicScene::get_player());
		}
	}else{
		TEXEngine::Core::LogManager::get().log_error("sled entity not found");
	}

	Entity* aiSledEntity = AssetManager::get().get_entity(24);
	if(aiSledEntity && aiSledEntity->is_collidable()){
		TEXEngine::Core::LogManager::get().log_message("setting physics stuff to the sled");

		// basic physics object info
		PhysObjInfo info;
		info.apply_default();
		info.mass = 800.0f;
		info.colId = PLAYER;
		vector<int> cWI;
		cWI.push_back(PLAYER);
		cWI.push_back(STATIC);
		cWI.push_back(OTHER);
		info.colWithIds = &cWI;
		/*if(info.colWithIds)
		{
			info.colWithIds->push_back(PLAYER);
			info.colWithIds->push_back(STATIC);
			info.colWithIds->push_back(OTHER);
		}*/

		// vehicle properties
		VehicleProperties vprops;
		vprops.apply_default();
		vprops.m_fWheelRadius = 1.2f;
		vprops.m_fWheelHeight = 1.2f;
		vprops.m_fWheelFriction = 30.0f;
		vprops.m_fSuspStiffness = 20.0f;
		vprops.m_fSuspCompression = 4.4f;
		vprops.m_fSuspDamping = 2.3f;
		vprops.m_fRollInfluence = 0.001f;
		vprops.m_fMaxBrakingForce = 1000.0f;
				
		PhysicsObj* po = PhysicsManager::get().create_vehicle(Vector3f(6.9f,0.5f,1.5f),24,info,vprops);
		if(po)
		{
			aiSledEntity->physics_obj(po);
		}
	}else{
		TEXEngine::Core::LogManager::get().log_error("ai sled entity not found");
	}

	Entity* pitEntity = AssetManager::get().get_entity(99);
	if(pitEntity && pitEntity->is_collidable()){
		TEXEngine::Core::LogManager::get().log_message("setting physics stuff to the pit");

		// setting up the physical properties
		PhysObjInfo info;
		info.apply_default();
		info.mass = 0.0f;
		info.colId = STATIC;
		vector<int> cWI;
		cWI.push_back(PLAYER);
		//cWI.push_back(OTHER); if we want boxes to have collision detection with water to reappear at initial pos
		info.colWithIds = &cWI;
		/*if(info.colWithIds)
		{
			info.colWithIds->push_back(PLAYER);
			//info.colWithIds->push_back(OTHER); if we want boxes to have collision detection with water to reappear at initial pos
		}*/

		PhysicsObj* po = PhysicsManager::get().create_plane(info,99,Vector3f(0.0f,1.0f,0.0f),-14.0f);

		if(po)
		{
			pitEntity->physics_obj(po);
			po->no_clip();
		}
	}else{
		TEXEngine::Core::LogManager::get().log_error("pit entity not found");
	}

	for(int i = 92; i < 99; ++i)
	{
		Entity* groundEntity = AssetManager::get().get_entity(i);
		if(groundEntity && groundEntity->is_collidable()){
			TEXEngine::Core::LogManager::get().log_message("setting physics stuff to the ground");

			// setting up the physical properties
			PhysObjInfo info;
			info.apply_default();
			info.mass = 0.0f;
			info.colId = STATIC;
			vector<int> cWI;
			cWI.push_back(PLAYER);
			info.colWithIds = &cWI;
			/*if(info.colWithIds)
			{
				info.colWithIds->push_back(PLAYER);
			}*/
					

			info.m_initial_transform_matrix.translate(Vector3f(0.0f,-35.0f,0.0f));
				
			PhysicsObj* po = NULL;
			Mesh* mesh = AssetManager::get().get_mesh(groundEntity->mesh());
			if(mesh)
			{
				po = PhysicsManager::get().create_static_object_from_mesh(mesh,Vector3f(1.0f,1.0f,1.0f),i,info);
			}
			else 
				TEXEngine::Core::LogManager::get().log_error("ground mesh not found, couldnt create physics object for it");

			if(po)
			{
				groundEntity->physics_obj(po);
				//po->set_elasticity(0.0f);
				//po->set_friction(0.01f);

			}else 
				TEXEngine::Core::LogManager::get().log_error("physics object for ground wasnt created");
		}else{
			TEXEngine::Core::LogManager::get().log_error("ground entity not found");
		}
	}

			
	TEXEngine::Asset::Mesh* snowMesh = TEXEngine::Core::AssetManager::get().get_mesh(13);
	if(snowMesh){
		TEXEngine::Util::Mutator::calc_normals(snowMesh);
	}else{
		TEXEngine::Core::LogManager::get().log_warning("no snow mesh");
	}
	TEXEngine::Asset::Mesh* iceMesh = TEXEngine::Core::AssetManager::get().get_mesh(14);
	if(iceMesh){
		TEXEngine::Util::Mutator::calc_normals(iceMesh);
	}else{
		TEXEngine::Core::LogManager::get().log_warning("no ice mesh");
	}
	TEXEngine::Asset::Mesh* desertMesh = TEXEngine::Core::AssetManager::get().get_mesh(15);
	if(desertMesh){
		TEXEngine::Util::Mutator::calc_normals(desertMesh);
	}else{
		TEXEngine::Core::LogManager::get().log_warning("no desert mesh");
	}





	//create startpoint
	{
		Entity* e = AssetManager::get().get_entity(100);
		if(e && e->is_collidable())
		{
			// setting up the physical properties
			PhysObjInfo info;
			info.apply_default();
			info.m_initial_transform_matrix.rotate(Vector3f(0.0f,1.0f,0.0f),3.14f);
			info.mass = 0.0f;
			info.colId = STATIC;
			vector<int> cWI;
			cWI.push_back(PLAYER);
			info.colWithIds = &cWI;
			/*if(info.colWithIds)
			{
				info.colWithIds->push_back(PLAYER);
			}*/

			PhysicsObj* p = PhysicsManager::get().create_box(info,100,Vector3f(0.1f,8.0f,20.0f));
			if(p){
				e->physics_obj(p);
					
				p->no_clip();
			}else TEXEngine::Core::LogManager::get().log_message("couldnt create physics object for startpoint (entity 100)");
		}
	}

	//create billboard
	{
		Entity* e = AssetManager::get().get_entity(8);
		if(e && e->is_collidable())
		{
			Mesh* mesh = AssetManager::get().get_mesh(e->mesh());
			if(e)
			{
				// setting up the physical properties
				PhysObjInfo info;
				info.apply_default();
				//info.m_initial_transform_matrix.rotate(Vector3f(0.0f,1.0f,0.0f),3.14f);
				info.mass = 0.0f;
				info.colId = STATIC;
				vector<int> cWI;
				cWI.push_back(PLAYER);
				info.colWithIds = &cWI;
			

				PhysicsObj* p = PhysicsManager::get().create_static_object_from_mesh(mesh,Vector3f(1.0f,1.0f,1.0f),8,info);
				if(p)
					e->physics_obj(p);
				else
					TEXEngine::Core::LogManager::get().log_error("couldnt create physics object for billboard (entity 8)");
			}
		}
	}

	//create waypoint marker
	{
		Entity* e = AssetManager::get().get_entity(5);
		if(e && e->is_collidable())
		{
			// setting up the physical properties
			PhysObjInfo info;
			info.apply_default();
			info.apply_default();
			info.mass = 0.0f;
			info.colId = STATIC;
			vector<int> cWI;
			cWI.push_back(PLAYER);
			info.colWithIds = &cWI;
			/*if(info.colWithIds)
			{
				info.colWithIds->push_back(PLAYER);
			}*/

			PhysicsObj* p = PhysicsManager::get().create_cylinder(info, 5, 10.0f, 10.0f);
			if(p){
				e->physics_obj(p);
					
				p->no_clip();
			}else TEXEngine::Core::LogManager::get().log_message("couldnt create physics object for waypointmarker (entity 5)");
		}
	}

	//create waypoints entities
	for(unsigned int i = 101; i <= 150; ++i){
		Entity* e = AssetManager::get().get_entity(i);
		if(e && e->is_collidable())
		{
			// setting up the physical properties
			PhysObjInfo info;
			info.apply_default();
			info.mass = 0.0f;
			info.colId = STATIC;
			vector<int> cWI;
			cWI.push_back(PLAYER);
			info.colWithIds = &cWI;
			/*if(info.colWithIds)
			{
				info.colWithIds->push_back(PLAYER);
			}*/

			PhysicsObj* p = PhysicsManager::get().create_cylinder(info, i, 15.0f, 20.0f);
			if(p){
				e->physics_obj(p);
					
				p->no_clip();
			}else{
				TEXEngine::Core::LogManager::get().log_message("couldn't create physics object for waypoint %i", i);
			}
		}
	}
	//create powerups entities			
	for(unsigned int i = 151; i <= 165; ++i){
		Entity* e = AssetManager::get().get_entity(i);
		if(e && e->is_collidable())
		{
			// setting up the physical properties 
			PhysObjInfo info;
			info.apply_default();
			info.mass = 0.0f;
			info.colId = STATIC;
			vector<int> cWI;
			cWI.push_back(PLAYER);
			info.colWithIds = &cWI;
			/*if(info.colWithIds)
			{
				info.colWithIds->push_back(PLAYER);
			}*/

			PhysicsObj* p = PhysicsManager::get().create_cylinder(info, i, 2.0f, 10.0f);
			if(p){
				e->physics_obj(p);
					
				p->no_clip();
			}else{
				TEXEngine::Core::LogManager::get().log_message("setting physics stuff to the ground");
			}
		}
	}
	/*
	Entity* startLineEntity = AssetManager::get().get_entity(100);
	if(startLineEntity){
		PhysObjInfo info;
		info.apply_default();
		info.mass = 0.0f;
		PhysicsObj* p = PhysicsManager::get().create_sphere(info, 200, 5.0f);
		if(p){
			startLineEntity->physics_obj(p);
			p->no_clip();
		}
		soloscene->attach(200);
	}
	*/

	//load all scenery, if they exist
	for(unsigned int i = 200; i < 900; ++i){
		Entity* e = AssetManager::get().get_entity(i);
		if(e && e->is_collidable()){
			PhysicsObj* po = NULL;
			Mesh* mesh = AssetManager::get().get_mesh(e->mesh());
			if(mesh)
			{
				// setting up the physical properties
				PhysObjInfo info;
				info.apply_default();
				info.mass = 0.0f;
				info.colId = STATIC;
				vector<int> cWI;
				cWI.push_back(PLAYER);
				info.colWithIds = &cWI;
				/*if(info.colWithIds)
				{
					info.colWithIds->push_back(PLAYER);
				}*/

				

				po = PhysicsManager::get().create_static_object_from_mesh(mesh,Vector3f(1.0f,1.0f,1.0f),i,info);
				if(po){
					e->physics_obj(po);
				}
			}

		}
	}
};

void GameLogicManager::setup(){
	create_console_commands();

	load_lua_stuff();
	
	
	TEXEngine::Core::LogManager::get().log_message("Game Log:\tLoading UI textures!");
	unsigned int texid = 15105;
	AssetManager::get().create_texture(texid);
	Texture* tex = AssetManager::get().get_texture(texid);
	Image p;
	TEXEngine::Util::Generator::linear_horizontal_gradient(&p, 380, 70, ColorRGBAf(1.0, 0.0, 0.0, 1.0), ColorRGBAf(0.0, 1.0, 0.0, 1.0), Vector2f(0.0, 35.0), Vector2f(380.0, 35.0));
	tex->img = p;

	NCF1 assets;
	assets.set_source(PATH_STUFF"data/scripts/ncf/assets.ncf1");
	assets.set("PATH_STUFF", PATH_STUFF);
#ifdef _WIN32
	assets.set("PLATFORM", "");
#endif
	assets.set("SHADER_EXTENSION",PS3_SHADER_EXTENSION);
	if(assets.parse() != 0){
		TEXEngine::Core::LogManager::get().log_error("couldn't parse file %s", assets.get_source());
	}
	TEXEngine::Core::LogManager::get().log_message("finished parsing uiassets");
	if(TEXEngine::Core::AssetManager::get().load(&assets) != 0){
		TEXEngine::Core::LogManager::get().log_error("couldn't load assets", assets.get_source());
	}
	TEXEngine::Core::LogManager::get().log_message("loaded assets!");

	
	LogicScene::create_player(160);
	setup_physics();
	init_keys_and_scenes();


	trigger_event(RELOAD_UI, Message());
	
	register_event(LOG_MANAGER_ENTRY_LOGGED);
	register_event(CONSOLE_LOG);
	register_event(UPDATE_LOG);
	register_event(TEST_ASPECT_RATIO);
	register_event(UPDATE);
	register_event(RELOAD_UI);
	register_event(FETCH_MENU_PARAMS);
	register_event(BACK_TO_MENU);
	register_event(QUIT_GAME);
	register_event(EXIT);
	register_event(TOGGLE_CONSOLE);
	register_event(ENABLE_MENU_KEYS);
	register_event(DISABLE_MENU_KEYS);
	register_event(PLAY_OnClick);
	register_event(PLAY_OverClick);
	register_event(PLAY_GO);
	register_event(PLAY_UNGO);
	register_event(PLAY_MENU_MUSIC);
	register_event(SAVE_OPTIONS);
	register_event(CANCEL_OPTIONS);
	register_event(REQUEST_WHIP_PCKEY_REBIND);
	register_event(REQUEST_WHIP_CONTROLLERKEY_REBIND);
	register_event(REQUEST_TREAT_PCKEY_REBIND);
	register_event(REQUEST_TREAT_CONTROLLERKEY_REBIND);
	register_event(REQUEST_BRAKE_PCKEY_REBIND);
	register_event(REQUEST_BRAKE_CONTROLLERKEY_REBIND);
	register_event(REQUEST_LEFT_PCKEY_REBIND);
	register_event(REQUEST_LEFT_CONTROLLERKEY_REBIND);
	register_event(REQUEST_RIGHT_PCKEY_REBIND);
	register_event(REQUEST_RIGHT_CONTROLLERKEY_REBIND);
	register_event(REQUEST_PREVIOUSCAMERA_PCKEY_REBIND);
	register_event(REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND);
	register_event(REQUEST_NEXTCAMERA_PCKEY_REBIND);
	register_event(REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND);
	register_event(PREVIOUS_CAMERA_PCKEY_REBOUND);
	register_event(PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND);
	register_event(NEXT_CAMERA_PCKEY_REBOUND);
	register_event(NEXT_CAMERA_CONTROLLERKEY_REBOUND);
	register_event(REQUEST_RESETPOSITION_PCKEY_REBIND);
	register_event(REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND);
	register_event(REQUEST_REARVIEW_PCKEY_REBIND);
	register_event(REQUEST_REARVIEW_CONTROLLERKEY_REBIND);


	register_event(SET_SOUND_VOLUME);
	register_event(SET_MUSIC_VOLUME);
	register_event(RESIZE_WINDOWS);
	register_event(CANCEL_KEYBIND_CHANGE);
}

void GameLogicManager::terminate(){

	disable_menu_keys();
	
	#ifdef	_WIN32
		if ( m_reload_ui )
		{
			m_reload_ui->disable();
			delete m_reload_ui;
			m_reload_ui = NULL;
		}

		if ( m_previous )
		{
			delete m_previous;
			m_previous = NULL;
		}

		if ( m_next )
		{
			delete m_next;
			m_next = NULL;
		}

		if ( m_left )
		{
			delete m_left;
			m_left = NULL;
		}

		if ( m_right )
		{
			delete m_right;
			m_right = NULL;
		}

		if ( m_select )
		{
			delete m_select;
			m_select = NULL;
		}

		if ( m_ingame_select )
		{
			delete m_ingame_select;
			m_ingame_select = NULL;
		}

		if ( m_console )
		{
			delete m_console;
			m_console = NULL;
		}

		if ( m_cancel_change )
		{
			delete m_cancel_change;
			m_cancel_change = NULL;
		}

		if ( m_whip_key )
		{
			delete m_whip_key;
			m_whip_key = NULL;
		}

		if ( m_treat_key )
		{
			delete m_treat_key;
			m_treat_key = NULL;
		}

		if ( m_brake_key )
		{
			delete m_brake_key;
			m_brake_key = NULL;
		}

		if ( m_left_key )
		{
			delete m_left_key;
			m_left_key = NULL;
		}

		if ( m_right_key )
		{
			delete m_right_key;
			m_right_key = NULL;
		}

		if ( m_next_camera_key )
		{
			delete m_next_camera_key;
			m_next_camera_key = NULL;
		}

		if ( m_previous_camera_key )
		{
			delete m_previous_camera_key;
			m_previous_camera_key = NULL;
		}

		if ( m_rearview_camera_key )
		{
			delete m_rearview_camera_key;
			m_rearview_camera_key = NULL;
		}

		if ( m_reset_position_key )
		{
			delete m_reset_position_key;
			m_reset_position_key = NULL;
		}


		m_rebind_whip = false;
		m_rebind_treat = false;
		m_rebind_brake = false;
		m_rebind_left = false;
		m_rebind_right = false;
		m_rebind_next_camera = false;
		m_rebind_previous_camera = false;
		m_rebind_rearview_camera = false;
		m_rebind_reset_position = false;

	#endif	/* _WIN32 */

	if ( m_cprevious )
	{
		delete m_cprevious;
		m_cprevious = NULL;
	}

	if ( m_cnext )
	{
		delete m_cnext;
		m_cnext = NULL;
	}

	if ( m_cleft )
	{
		delete m_cleft;
		m_cleft = NULL;
	}

	if ( m_cright )
	{
		delete m_cright;
		m_cright = NULL;
	}

	if ( m_cselect )
	{
		delete m_cselect;
		m_cselect = NULL;
	}

	if ( m_cingame_select )
	{
		delete m_cingame_select;
		m_cingame_select = NULL;
	}

	if ( m_cconsole )
	{
		delete m_cconsole;
		m_cconsole = NULL;
	}

	if ( m_ccancel_change )
	{
		delete m_ccancel_change;
		m_ccancel_change = NULL;
	}

	if ( m_cwhip_key )
	{
		delete m_cwhip_key;
		m_cwhip_key = NULL;
	}

	if ( m_ctreat_key )
	{
		delete m_ctreat_key;
		m_ctreat_key = NULL;
	}

	if ( m_cbrake_key )
	{
		delete m_cbrake_key;
		m_cbrake_key = NULL;
	}

	if ( m_cleft_key )
	{
		delete m_cleft_key;
		m_cleft_key = NULL;
	}

	if ( m_cright_key )
	{
		delete m_cright_key;
		m_cright_key = NULL;
	}

	if ( m_cnext_camera_key )
	{
		delete m_cnext_camera_key;
		m_cnext_camera_key = NULL;
	}

	if ( m_cprevious_camera_key )
	{
		delete m_cprevious_camera_key;
		m_cprevious_camera_key = NULL;
	}

	if ( m_crearview_camera_key )
	{
		delete m_crearview_camera_key;
		m_crearview_camera_key = NULL;
	}

	if ( m_creset_position_key )
	{
		delete m_creset_position_key;
		m_creset_position_key = NULL;
	}

	
	m_rebind_cwhip = false;
	m_rebind_ctreat = false;
	m_rebind_cbrake = false;
	m_rebind_cleft = false;
	m_rebind_cright = false;
	m_rebind_cnext_camera = false;
	m_rebind_cprevious_camera = false;
	m_rebind_crearview_camera = false;
	m_rebind_creset_position = false;



	if ( m_current )
	{
		m_current->cleanup();
		m_current = NULL;
	}


	if ( m_soloScene )
	{
		delete m_soloScene;
		m_soloScene = NULL;
	}

	if ( m_versusScene )
	{
		delete m_versusScene;
		m_versusScene = NULL;
	}
	
	if ( m_menuScene )
	{
		delete m_menuScene;
		m_menuScene = NULL;
	}
}

GameLogicManager::~GameLogicManager(){
	terminate();
	LogicScene::destroy_player();
	clear_console_commands();
}

void GameLogicManager::on_event(const unsigned long event_code, const Message& parameters){
	switch(event_code){
		case CONSOLE_LOG:{
			string text("");
			string* temp = NULL;

			if ( parameters.get_pointers_size() )
			{
				temp = static_cast<string*>(parameters.get_pointer(0));

				if ( temp )
					text = *temp;
			}
			else
			{
				std::stringstream buffer; 
				
				buffer << static_cast<double>(parameters.get_real_data(0));
				buffer >> text;
			}


			handle_lua_console_input(text);
			break; 
		}
		case UPDATE_LOG:
		case LOG_MANAGER_ENTRY_LOGGED:
			m_console_update_pending = true;
			break;
		case TOGGLE_CONSOLE:{
			toggle_console();
			break;
		}
		case ENABLE_MENU_KEYS:{
			enable_menu_keys();
			break;
		}
		case DISABLE_MENU_KEYS:{
			disable_menu_keys();
			break;
		}
		case TEST_ASPECT_RATIO:{
			float param = static_cast<float>(parameters.get_real_data(0));
			UIManager::get().aspect_modifier(param);
			break;
		}
		case RELOAD_UI:{
			if(m_current){
				m_current->cleanup();
			}
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tReloading the User Interface.");
			terminate();
			init_keys_and_scenes();
			break;
		}
		case QUIT_GAME:
			set_running(false);
		break;

#ifdef _WIN32
		case SET_SOUND_VOLUME:{
			double intPart;
			double param = parameters.get_real_data(0);
			modf(param, &intPart);
			AudioManager::get().set_volume(static_cast<float>(intPart)/100.0f, TEXENG_AUDIO_FX);
			break;
			}

		case SET_MUSIC_VOLUME:{
			double intPart;
			double param = parameters.get_real_data(0);
			modf(param, &intPart);
			AudioManager::get().set_volume(static_cast<float>(intPart)/100.0f, TEXENG_AUDIO_MUSIC);
			break;
		}

#endif /*_WIN32*/ 

		case UPDATE:{

			if ( m_console_update_pending )
				update_console();

			update_handler();
			break;
		}
		case CANCEL_KEYBIND_CHANGE:{
			cancel_keybind_change_handler();
			break;
		}

		#ifdef	_WIN32
		case REQUEST_WHIP_PCKEY_REBIND:		
		case REQUEST_TREAT_PCKEY_REBIND:		
		case REQUEST_BRAKE_PCKEY_REBIND:
		case REQUEST_LEFT_PCKEY_REBIND:
		case REQUEST_RIGHT_PCKEY_REBIND:
		case REQUEST_NEXTCAMERA_PCKEY_REBIND:
		case REQUEST_PREVIOUSCAMERA_PCKEY_REBIND:
		case REQUEST_RESETPOSITION_PCKEY_REBIND:
		case REQUEST_REARVIEW_PCKEY_REBIND:

		#endif	/* _WIN32 */
		case REQUEST_WHIP_CONTROLLERKEY_REBIND:
		case REQUEST_TREAT_CONTROLLERKEY_REBIND:
		case REQUEST_BRAKE_CONTROLLERKEY_REBIND:
		case REQUEST_LEFT_CONTROLLERKEY_REBIND:
		case REQUEST_RIGHT_CONTROLLERKEY_REBIND:
		case REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND:
		case REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND:
		case REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND:
		case REQUEST_REARVIEW_CONTROLLERKEY_REBIND:
			request_keychange_handler(event_code);
			break;

		case RESIZE_WINDOWS:{
			double intPart, fullscreenzor;
			double param = parameters.get_real_data(0);
			modf(param, &intPart);
			int resolution = static_cast<int>(intPart);

			param = parameters.get_real_data(1);
			modf(param, &fullscreenzor);

			//bool oldfullscreen = is_fullscreen();
			bool fullscreen = ( static_cast<unsigned int>(fullscreenzor) > 0  ?  true : false);
			if( OPTION_RESOLUTIONS.size() > 0){
				int theRes = resolution-1;
				if(theRes >= 0 && theRes < (int)OPTION_RESOLUTIONS.size()){
					unsigned int width = OPTION_RESOLUTIONS[theRes][0];
					unsigned int height = OPTION_RESOLUTIONS[theRes][1];
					switch_to_fullscreen(fullscreen, width, height);
					if(!fullscreen)
						resize(width, height);
				}
			}
			break;
		}
		case SAVE_OPTIONS:{
			save_options_handler(parameters);
			break;
		}
		case CANCEL_OPTIONS:{
			cancel_options_handler();
			break;
		}

#ifdef _WIN32
		case PLAY_OnClick:
			AudioManager::get().play_audio(AssetManager::get().get_audioclip(100));
			break;
		case PLAY_OverClick:
			AudioManager::get().play_audio(AssetManager::get().get_audioclip(101));
			break;
		case PLAY_MENU_MUSIC:
			play_menu_music();
			break;
		case PLAY_GO:
			AudioManager::get().play_audio(AssetManager::get().get_audioclip(104));
			break;
		case PLAY_UNGO:
			AudioManager::get().play_audio(AssetManager::get().get_audioclip(105));
			break;
#endif //_WIN32
		case FETCH_MENU_PARAMS:{
			fetch_menu_params_handler(parameters);
			break;
		}
		case BACK_TO_MENU:{
			bool playMenuMusic = false;
			if(m_current){
				//if we get here, we know that we've actually been redirected from another scene (i.e. you don't get here the very first time the menu is displayed)
				play_menu_music();
				m_current->cleanup();
			}else{
				send_keybind_texts();
			}
			m_current = m_menuScene;
			if(m_current){
				Message	parameters(0,1,0);

				enable_menu_keys();
				parameters.add_real_data(0,1200+UIMANAGER_FROM_LUA_BIAS); //focus the timeTrialButton
				trigger_event(COMPONENT_REQUEST_FOCUS,parameters);
				m_current->init(vector<string>(0,""),vector<unsigned int>(0,0),vector<vector<unsigned int> >(0,vector<unsigned int>(0,0)));
				/*
#ifdef _WIN32
				AudioManager::get().stop_music();
				AudioManager::get().play_audio(AssetManager::get().get_audioclip(102));
#endif //WIN32
				*/
			}
		}
		break;
		case EXIT:{
			terminate();
			LogicScene::destroy_player();
		}
		break;
	}
}

void GameLogicManager::update_console(){
	Container* console = UIManager::get().get_container(16000);
	if(console){
		Textbox* consoleTextbox = UIManager::get().get_textbox(16100);
		if(consoleTextbox && consoleTextbox->is_visible()){
			unsigned int max_lines = consoleTextbox->visible_lines();
			if(max_lines > 5)
				max_lines-=5;
			unsigned int max_length  = consoleTextbox->visible_characters();
			unsigned int i  = 0;
			std::string temp(TEXEngine::Core::LogManager::get().get_log_entry(i));
			std::string mystring("");
			
			while( i < max_lines  &&  temp != "" ){
				temp += "\n";
				mystring.insert(0,temp);

				if ( temp.size() > max_length )
					max_lines -= static_cast<unsigned int>(ceil(static_cast<float>(temp.size())/static_cast<float>(max_length)));

				++i;	
				temp = TEXEngine::Core::LogManager::get().get_log_entry(i);
			}

			consoleTextbox->set_text(mystring);
			m_console_update_pending = false;
		}
	}
}

//TOGGLE_CONSOLE
void GameLogicManager::toggle_console(){
	//toggle console
	Container* console = UIManager::get().get_container(16000);
	if(console){
		bool b = !console->is_visible();
		if(b){
			disable_menu_keys();

			if ( m_current )
			{
				m_current->disable_keys();
				m_current->disable_player_keys();
				
				#ifdef	_WIN32
					if ( m_ingame_select )
						m_ingame_select->enable();
				#endif	/* _WIN32 */

				if ( m_cingame_select )
					m_cingame_select->enable();
			}

		}else{

			if ( m_current )
			{
				m_current->enable_keys();
				m_current->enable_player_keys();

				#ifdef	_WIN32
					if ( m_ingame_select )
						m_ingame_select->disable();
				#endif	/* _WIN32 */

				if ( m_cingame_select )
					m_cingame_select->disable();

				if ( m_current == m_menuScene ||  m_current->state() == LogicScene::SceneStateEnum_PAUSED  ||  m_current->state() == LogicScene::SceneStateEnum_FINISHED)
					enable_menu_keys();
			}
			else
				enable_menu_keys();
		}
	}
}

void GameLogicManager::request_rebind_key(Hotkey* cancel, bool& rebind, Hotkey* key, const char* s){
	if ( key_change_active() )
		cancel_keybind_change_handler();

	TEXEngine::Core::LogManager::get().log_message("Game Log:\tReceived rebinding request for %s!!", s);
	disable_menu_keys();
	cancel->enable();
	rebind = true;
	key->change_keybind();
}

void GameLogicManager::request_keychange_handler(const unsigned int e){
	switch(e){
		#ifdef	_WIN32
		case REQUEST_WHIP_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_whip, m_whip_key, "the keyboard whip hotkey");
			break;
		}
		case REQUEST_TREAT_PCKEY_REBIND:{			
			request_rebind_key(m_cancel_change, m_rebind_treat, m_treat_key, "the keyboard treat hotkey");
			break;
		}
		case REQUEST_BRAKE_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_brake, m_brake_key, "the keyboard brake hotkey");
			break;
		}
		case REQUEST_LEFT_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_left, m_left_key, "the keyboard left hotkey");
			break;
		}
		case REQUEST_RIGHT_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_right, m_right_key, "the keyboard right hotkey");
			break;
		}
		case REQUEST_NEXTCAMERA_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_next_camera, m_next_camera_key, "the keyboard next camera hotkey");
			break;
		}
		case REQUEST_PREVIOUSCAMERA_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_previous_camera, m_previous_camera_key, "the keyboard previous camera hotkey");
			break;
		}
		case REQUEST_RESETPOSITION_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_reset_position, m_reset_position_key, "the keyboard reset position hotkey");
			break;
		}
		case REQUEST_REARVIEW_PCKEY_REBIND:{
			request_rebind_key(m_cancel_change, m_rebind_rearview_camera, m_rearview_camera_key, "the keyboard rear view camera hotkey");
			break;
		}
		#endif	/* _WIN32 */

		case REQUEST_WHIP_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cwhip, m_cwhip_key, "the controller whip hotkey");
			break;
		}
		case REQUEST_TREAT_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_ctreat, m_ctreat_key, "the controller treat hotkey");
			break;
		}
		case REQUEST_BRAKE_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cbrake, m_cbrake_key, "the controller brake hotkey");
			break;
		}
		case REQUEST_LEFT_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cleft, m_cleft_key, "the controller left hotkey");
			break;
		}
		case REQUEST_RIGHT_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cright, m_cright_key, "the controller right hotkey");
			break;
		}
		case REQUEST_NEXTCAMERA_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cnext_camera, m_cnext_camera_key, "the controller next camera hotkey");
			break;
		}
		case REQUEST_PREVIOUSCAMERA_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_cprevious_camera, m_cprevious_camera_key, "the controller previous camera hotkey");
			break;
		}
		case REQUEST_RESETPOSITION_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_creset_position, m_creset_position_key, "the controller reset position hotkey");
			break;
		}
		case REQUEST_REARVIEW_CONTROLLERKEY_REBIND:{
			request_rebind_key(m_ccancel_change, m_rebind_crearview_camera, m_crearview_camera_key, "the controller rear view camera hotkey");
			break;
		}
	}
}

void GameLogicManager::cancel_options_handler(){
	#ifdef	_WIN32

		if ( m_whip_key )
		{
			m_whip_value = m_previous_whip_value;
			m_whip_mod_value = m_previous_whip_mod_value;
			m_whip_key->keybind(m_whip_value);
			m_whip_key->modifier(m_whip_mod_value);
			m_whip_key->disable();
		}

		if ( m_treat_key )
		{
			m_treat_value = m_previous_treat_value;
			m_treat_mod_value = m_previous_treat_mod_value;
			m_treat_key->keybind(m_treat_value);
			m_treat_key->modifier(m_treat_mod_value);
			m_treat_key->disable();
		}

		if ( m_brake_key )
		{
			m_brake_value = m_previous_brake_value;
			m_brake_mod_value = m_previous_brake_mod_value;
			m_brake_key->keybind(m_brake_value);
			m_brake_key->modifier(m_brake_mod_value);
			m_brake_key->disable();
		}

		if ( m_left_key )
		{
			m_left_value = m_previous_left_value;
			m_left_mod_value = m_previous_left_mod_value;
			m_left_key->keybind(m_left_value);
			m_left_key->modifier(m_left_mod_value);
			m_left_key->disable();

		}

		if ( m_right_key )
		{
			m_right_value = m_previous_right_value;
			m_right_mod_value = m_previous_right_mod_value;
			m_right_key->keybind(m_right_value);
			m_right_key->modifier(m_right_mod_value);
			m_right_key->disable();
		}

		if ( m_next_camera_key )
		{
			m_next_camera_value = m_previous_next_camera_value;
			m_next_camera_mod_value = m_previous_next_camera_mod_value;
			m_next_camera_key->keybind(m_next_camera_value);
			m_next_camera_key->modifier(m_next_camera_mod_value);
			m_next_camera_key->disable();
		}

		if ( m_previous_camera_key )
		{
			m_previous_camera_value = m_previous_previous_camera_value;
			m_previous_camera_mod_value = m_previous_previous_camera_mod_value;
			m_previous_camera_key->keybind(m_previous_camera_value);
			m_previous_camera_key->modifier(m_previous_camera_mod_value);
			m_previous_camera_key->disable();
		}

		if ( m_rearview_camera_key )
		{
			m_rearview_camera_value = m_previous_rearview_camera_value;
			m_rearview_camera_mod_value = m_previous_rearview_camera_mod_value;
			m_rearview_camera_key->keybind(m_rearview_camera_value);
			m_rearview_camera_key->modifier(m_rearview_camera_mod_value);
			m_rearview_camera_key->disable();
		}

		if ( m_reset_position_key )
		{
			m_reset_position_value = m_previous_reset_position_value;
			m_reset_position_mod_value = m_previous_reset_position_mod_value;
			m_reset_position_key->keybind(m_reset_position_value);
			m_reset_position_key->modifier(m_reset_position_mod_value);
			m_reset_position_key->disable();
		}

	#endif	/* _WIN32 */

	if ( m_cwhip_key )
	{
		m_cwhip_value = m_previous_cwhip_value;
		m_cwhip_key->keybind(m_cwhip_value);
		m_cwhip_key->disable();
	}

	if ( m_ctreat_key )
	{
		m_ctreat_value = m_previous_ctreat_value;
		m_ctreat_key->keybind(m_ctreat_value);
		m_ctreat_key->disable();
	}

	if ( m_cbrake_key )
	{
		m_cbrake_value = m_previous_cbrake_value;
		m_cbrake_key->keybind(m_cbrake_value);
		m_cbrake_key->disable();
	}

	if ( m_cleft_key )
	{
		m_cleft_value = m_previous_cleft_value;
		m_cleft_key->keybind(m_cleft_value);
		m_cleft_key->disable();
	}

	if ( m_cright_key )
	{
		m_cright_value = m_previous_cright_value;
		m_cright_key->keybind(m_cright_value);
		m_cright_key->disable();
	}

	if ( m_cnext_camera_key )
	{
		m_cnext_camera_value = m_previous_cnext_camera_value;
		m_cnext_camera_key->keybind(m_cnext_camera_value);
		m_cnext_camera_key->disable();
	}

	if ( m_cprevious_camera_key )
	{
		m_cprevious_camera_value = m_previous_cprevious_camera_value;
		m_cprevious_camera_key->keybind(m_cprevious_camera_value);
		m_cprevious_camera_key->disable();
	}

	if ( m_crearview_camera_key )
	{
		m_crearview_camera_value = m_previous_crearview_camera_value;
		m_crearview_camera_key->keybind(m_crearview_camera_value);
		m_crearview_camera_key->disable();
	}

	if ( m_creset_position_key )
	{
		m_creset_position_value = m_previous_creset_position_value;
		m_creset_position_key->keybind(m_creset_position_value);
		m_creset_position_key->disable();
	}

	send_keybind_texts();			
}

void GameLogicManager::fetch_menu_params_handler(const TEXEngine::Core::Message& parameters){

	vector<string> sceneParameters(14,"");
	string temp(*static_cast<string*>(parameters.get_pointer(2)));

	sceneParameters[0] = *static_cast<string*>(parameters.get_pointer(0));
	sceneParameters[1] = *static_cast<string*>(parameters.get_pointer(1));
	/*
	if ( temp == COURSETYPE_PROPERTY_SNOW )
			sceneParameters[2] = PATH_STUFF"data/scripts/ncf/SnowSolo.ncf1";
	else if ( temp == COURSETYPE_PROPERTY_ICE )
			sceneParameters[2] = PATH_STUFF"data/scripts/ncf/IceSolo.ncf1";
	else if (temp == COURSETYPE_PROPERTY_DESERT )
			sceneParameters[2] = PATH_STUFF"data/scripts/ncf/DesertSolo.ncf1";
	*/
	sceneParameters[3] = *static_cast<string*>(parameters.get_pointer(3));
	sceneParameters[4] = *static_cast<string*>(parameters.get_pointer(4));
	sceneParameters[5] = *static_cast<string*>(parameters.get_pointer(5));
	sceneParameters[6] = *static_cast<string*>(parameters.get_pointer(6));
	sceneParameters[7] = *static_cast<string*>(parameters.get_pointer(7));
	sceneParameters[8] = *static_cast<string*>(parameters.get_pointer(8));
	sceneParameters[9] = *static_cast<string*>(parameters.get_pointer(9));

	stringstream ss;
	double intPart;
	double param;
	for(unsigned int i = 0; i < 4; ++i){
		ss.clear();
		param = parameters.get_real_data(i);
		modf(param, &intPart);
		ss << intPart;
		ss >> sceneParameters[10+i];
		ss.flush();
	}
	/*
	sceneParameters[10] = *static_cast<string*>(parameters.get_pointer(10));
	sceneParameters[11] = *static_cast<string*>(parameters.get_pointer(11));
	sceneParameters[12] = *static_cast<string*>(parameters.get_pointer(12));
	*/
	LogicScene* tmp = m_current;			
	if (sceneParameters[1].compare(RACETYPE_PROPERTY_SOLO) == 0){
		m_current = m_soloScene;
	}else if (sceneParameters[1].compare(RACETYPE_PROPERTY_VERSUS) == 0) {
		m_current = m_versusScene;
	}else{
		TEXEngine::Core::LogManager::get().log_error("Invalid race type received");
		return;
	}

	#ifdef	_WIN32
		vector<unsigned int>			scene_keys(15,0);
		vector<vector<unsigned int> >	player_keys(1,vector<unsigned int>(15,0));
	#else
		vector<unsigned int>			scene_keys(5,0);
		vector<vector<unsigned int> >	player_keys(1,vector<unsigned int>(5,0));
	#endif	/* _WIN32 */

	#ifdef	_WIN32

		scene_keys[0] = m_pause_scene_value;
		scene_keys[1] = m_pause_scene_mod_value;
		scene_keys[2] = m_next_camera_value;
		scene_keys[3] = m_next_camera_mod_value;
		scene_keys[4] = m_previous_camera_value;
		scene_keys[5] = m_previous_camera_mod_value;
		scene_keys[6] = m_rearview_camera_value;
		scene_keys[7] = m_rearview_camera_mod_value;
		scene_keys[8] = m_reset_position_value;
		scene_keys[9] = m_reset_position_mod_value;
		scene_keys[10] = m_cpause_scene_value;
		scene_keys[11] = m_cnext_camera_value;
		scene_keys[12] = m_cprevious_camera_value;
		scene_keys[13] = m_crearview_camera_value;
		scene_keys[14] = m_creset_position_value;

		player_keys[0][0] = m_whip_value;
		player_keys[0][1] = m_whip_mod_value;
		player_keys[0][2] = m_treat_value;
		player_keys[0][3] = m_treat_mod_value;
		player_keys[0][4] = m_brake_value;
		player_keys[0][5] = m_brake_mod_value;
		player_keys[0][6] = m_left_value;
		player_keys[0][7] = m_left_mod_value;
		player_keys[0][8] = m_right_value;
		player_keys[0][9] = m_right_mod_value;
		player_keys[0][10] = m_cwhip_value;
		player_keys[0][11] = m_ctreat_value;
		player_keys[0][12] = m_cbrake_value;
		player_keys[0][13] = m_cleft_value;
		player_keys[0][14] = m_cright_value;

	#else

		scene_keys[0] = m_cpause_scene_value;
		scene_keys[1] = m_cnext_camera_value;
		scene_keys[2] = m_cprevious_camera_value;
		scene_keys[3] = m_crearview_camera_value;
		scene_keys[4] = m_creset_position_value;

		player_keys[0][0] = m_cwhip_value;
		player_keys[0][1] = m_ctreat_value;
		player_keys[0][2] = m_cbrake_value;
		player_keys[0][3] = m_cleft_value;
		player_keys[0][4] = m_cright_value;

	#endif	/* _WIN32 */

	if(m_current){
		m_current->init(sceneParameters,scene_keys,player_keys);
		disable_menu_keys();

//#ifdef _WIN32
//		AudioManager::get().stop_music();
//		AudioManager::get().play_audio(AssetManager::get().get_audioclip(103));
//#endif //_WIN32
	}

	if(tmp)
		tmp->cleanup();
}

void GameLogicManager::save_options_handler(const TEXEngine::Core::Message& parameters){

	stringstream output;
	string s1;
	double intPart;
	NCF1* config = m_config.group("config");
	if(config){
		double param = parameters.get_real_data(0);
		modf(param, &intPart);
		int resolution = static_cast<unsigned int>(intPart);
		output << resolution;
		output >> s1;
		output.flush();
		config->set("resolution", s1.c_str());

		param =  parameters.get_real_data(1);
		modf(param, &intPart);
		output.clear();
		output << static_cast<unsigned int>(intPart);
		output >> s1;
		output.flush();
		TEXEngine::Core::LogManager::get().log_warning("Warning, changing the resolution requires restarting the game!");
		config->set("quality", s1.c_str());

		param =  parameters.get_real_data(2);
		modf(param, &intPart);
		output.clear();
		output << static_cast<unsigned int>(intPart);
		output >> s1;
		output.flush();
		config->set("music", s1.c_str());

#ifdef _WIN32
		AudioManager::get().set_volume(static_cast<float>(param)/100.0f, TEXENG_AUDIO_MUSIC);
#endif /*_WIN32*/
				

		param =  parameters.get_real_data(3);
		modf(param, &intPart);
		output.clear();
		output << static_cast<unsigned int>(intPart);
		output >> s1;
		output.flush();
		config->set("sound", s1.c_str());

#ifdef _WIN32
		AudioManager::get().set_volume(static_cast<float>(param)/100.0f, TEXENG_AUDIO_FX);
#endif



		param =  parameters.get_real_data(4);
		modf(param, &intPart);
		bool oldfullscreen = is_fullscreen();
		bool fullscreen = ( static_cast<unsigned int>(intPart) > 0  ?  true : false);
				
		output.clear();
		output << fullscreen;
		output >> s1;
		output.flush();
		config->set("fullscreen", s1.c_str());
	}
	NCF1* keyBindings = m_config.group("keybindings");
	if(keyBindings){
		#ifdef	_WIN32

			output.clear();
			output << m_whip_value;
			output >> s1;
			output.flush();
			keyBindings->set("whip_key", s1.c_str());

			output.clear();
			output << m_whip_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("whip_modifier", s1.c_str());

			output.clear();
			output << m_treat_value;
			output >> s1;
			output.flush();
			keyBindings->set("treat_key", s1.c_str());

			output.clear();
			output << m_treat_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("treat_modifier", s1.c_str());

			output.clear();
			output << m_brake_value;
			output >> s1;
			output.flush();
			keyBindings->set("brake_key", s1.c_str());

			output.clear();
			output << m_brake_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("brake_modifier", s1.c_str());

			output.clear();
			output << m_left_value;
			output >> s1;
			output.flush();
			keyBindings->set("left_key", s1.c_str());

			output.clear();
			output << m_left_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("left_modifier", s1.c_str());

			output.clear();
			output << m_right_value;
			output >> s1;
			output.flush();
			keyBindings->set("right_key", s1.c_str());

			output.clear();
			output << m_right_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("right_modifier", s1.c_str());

			output.clear();
			output << m_next_camera_value;
			output >> s1;
			output.flush();
			keyBindings->set("next_camera_key", s1.c_str());

			output.clear();
			output << m_next_camera_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("next_camera_modifier", s1.c_str());

			output.clear();
			output << m_previous_camera_value;
			output >> s1;
			output.flush();
			keyBindings->set("previous_camera_key", s1.c_str());

			output.clear();
			output << m_previous_camera_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("previous_camera_modifier", s1.c_str());

			output.clear();
			output << m_rearview_camera_value;
			output >> s1;
			output.flush();
			keyBindings->set("rearview_camera_key", s1.c_str());

			output.clear();
			output << m_rearview_camera_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("rearview_camera_modifier", s1.c_str());

			output.clear();
			output << m_reset_position_value;
			output >> s1;
			output.flush();
			keyBindings->set("reset_position_key", s1.c_str());

			output.clear();
			output << m_reset_position_mod_value;
			output >> s1;
			output.flush();
			keyBindings->set("reset_position_modifier", s1.c_str());


			m_previous_whip_value = m_whip_value;
			m_previous_whip_mod_value = m_whip_mod_value;
			m_previous_treat_value = m_treat_value;
			m_previous_treat_mod_value = m_treat_mod_value;
			m_previous_brake_value = m_brake_value;
			m_previous_brake_mod_value = m_brake_mod_value;
			m_previous_left_value = m_left_value;
			m_previous_left_mod_value = m_left_mod_value;
			m_previous_right_value = m_right_value;
			m_previous_right_mod_value = m_right_mod_value;
			m_previous_next_camera_value = m_next_camera_value;
			m_previous_next_camera_mod_value = m_next_camera_mod_value;
			m_previous_previous_camera_value = m_previous_camera_value;
			m_previous_previous_camera_mod_value = m_previous_camera_mod_value;
			m_previous_rearview_camera_value = m_rearview_camera_value;
			m_previous_rearview_camera_mod_value = m_rearview_camera_mod_value;
			m_previous_reset_position_value = m_reset_position_value;
			m_previous_reset_position_mod_value = m_reset_position_mod_value;

		#endif	/* _WIN32 */

		output.clear();
		output << m_cwhip_value;
		output >> s1;
		output.flush();
		keyBindings->set("cwhip_key", s1.c_str());

		output.clear();
		output << m_ctreat_value;
		output >> s1;
		output.flush();
		keyBindings->set("ctreat_key", s1.c_str());

		output.clear();
		output << m_cbrake_value;
		output >> s1;
		output.flush();
		keyBindings->set("cbrake_key", s1.c_str());

		output.clear();
		output << m_cleft_value;
		output >> s1;
		output.flush();
		keyBindings->set("cleft_key", s1.c_str());

		output.clear();
		output << m_cright_value;
		output >> s1;
		output.flush();
		keyBindings->set("cright_key", s1.c_str());

		output.clear();
		output << m_cnext_camera_value;
		output >> s1;
		output.flush();
		keyBindings->set("cnext_camera_key", s1.c_str());

		output.clear();
		output << m_cprevious_camera_value;
		output >> s1;
		output.flush();
		keyBindings->set("cprevious_camera_key", s1.c_str());

		output.clear();
		output << m_crearview_camera_value;
		output >> s1;
		output.flush();
		keyBindings->set("crearview_camera_key", s1.c_str());

		output.clear();
		output << m_creset_position_value;
		output >> s1;
		output.flush();
		keyBindings->set("creset_position_key", s1.c_str());

		m_previous_cwhip_value = m_cwhip_value;
		m_previous_ctreat_value = m_ctreat_value;
		m_previous_cbrake_value = m_cbrake_value;
		m_previous_cleft_value = m_cleft_value;
		m_previous_cright_value = m_cright_value;
		m_previous_cnext_camera_value = m_cnext_camera_value;
		m_previous_cprevious_camera_value = m_cprevious_camera_value;
		m_previous_crearview_camera_value = m_crearview_camera_value;
		m_previous_creset_position_value = m_creset_position_value;
	}
	m_config.dump(m_config.get_source());
}

void GameLogicManager::cancel_keybind_change_handler(){

	#ifdef	_WIN32

		if ( m_rebind_whip  &&  m_whip_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard whip hotkey!!");
			m_whip_key->lock_keybind();
			m_whip_key->disable();
			m_rebind_whip = false;
		}
		else if ( m_rebind_treat  &&  m_treat_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard treat hotkey!!");
			m_treat_key->lock_keybind();
			m_treat_key->disable();
			m_rebind_treat = false;
		}
		else if ( m_rebind_brake  &&  m_brake_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard brake hotkey!!");
			m_brake_key->lock_keybind();
			m_brake_key->disable();
			m_rebind_brake = false;
		}
		else if ( m_rebind_left  &&  m_left_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard left hotkey!!");
			m_left_key->lock_keybind();
			m_left_key->disable();
			m_rebind_left = false;
		}
		else if ( m_rebind_right  &&  m_right_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard right hotkey!!");
			m_right_key->lock_keybind();
			m_right_key->disable();
			m_rebind_right = false;
		}
		else if ( m_rebind_next_camera  &&  m_next_camera_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard next camera hotkey!!");
			m_next_camera_key->lock_keybind();
			m_next_camera_key->disable();
			m_rebind_next_camera = false;
		}
		else if ( m_rebind_previous_camera  &&  m_previous_camera_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard previous camera hotkey!!");
			m_previous_camera_key->lock_keybind();
			m_previous_camera_key->disable();
			m_rebind_previous_camera = false;
		}
		else if ( m_rebind_rearview_camera  &&  m_rearview_camera_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard rear view camera hotkey!!");
			m_rearview_camera_key->lock_keybind();
			m_rearview_camera_key->disable();
			m_rebind_rearview_camera = false;
		}
		else if ( m_rebind_reset_position  &&  m_reset_position_key )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the keyboard reset position hotkey!!");
			m_reset_position_key->lock_keybind();
			m_reset_position_key->disable();
			m_rebind_reset_position = false;
		}

		m_cancel_change->disable();

	#endif	/* _WIN32 */


	if ( m_rebind_cwhip  &&  m_cwhip_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller whip hotkey!!");
		m_cwhip_key->lock_keybind();
		m_cwhip_key->disable();
		m_rebind_cwhip = false;
	}
	else if ( m_rebind_ctreat  &&  m_ctreat_key)
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller treat hotkey!!");
		m_ctreat_key->lock_keybind();
		m_ctreat_key->disable();
		m_rebind_ctreat = false;
	}
	else if ( m_rebind_cbrake  &&  m_cbrake_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller brake hotkey!!");
		m_cbrake_key->lock_keybind();
		m_cbrake_key->disable();
		m_rebind_cbrake = false;
	}
	else if ( m_rebind_cleft  &&  m_cleft_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller left hotkey!!");
		m_cleft_key->lock_keybind();
		m_cleft_key->disable();
		m_rebind_cleft = false;
	}
	else if ( m_rebind_cright  &&  m_cright_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller right hotkey!!");
		m_cright_key->lock_keybind();
		m_cright_key->disable();
		m_rebind_cright = false;
	}
	else if ( m_rebind_cnext_camera  &&  m_cnext_camera_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller next camera hotkey!!");
		m_cnext_camera_key->lock_keybind();
		m_cnext_camera_key->disable();
		m_rebind_cnext_camera = false;
	}
	else if ( m_rebind_cprevious_camera  &&  m_cprevious_camera_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller previous camera hotkey!!");
		m_cprevious_camera_key->lock_keybind();
		m_cprevious_camera_key->disable();
		m_rebind_cprevious_camera = false;
	}
	else if ( m_rebind_crearview_camera  &&  m_crearview_camera_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller rear view camera hotkey!!");
		m_crearview_camera_key->lock_keybind();
		m_crearview_camera_key->disable();
		m_rebind_crearview_camera = false;
	}
	else if ( m_rebind_creset_position  &&  m_creset_position_key )
	{
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tCanceling rebinding for the controller reset_position hotkey!!");
		m_creset_position_key->lock_keybind();
		m_creset_position_key->disable();
		m_rebind_creset_position = false;
	}

	m_ccancel_change->disable();
	enable_menu_keys();
}

void GameLogicManager::update_handler(){
	Message	parameters(0,0,1);

	#ifdef	_WIN32

		if ( m_rebind_whip  &&  m_whip_key  &&  !m_whip_key->change_pending() )
		{
			string	text(m_whip_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(WHIP_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard whip hotkey!!");
			m_whip_value = m_whip_key->keybind();
			m_whip_mod_value = m_whip_key->modifier();
			m_whip_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_whip_key);
			enable_menu_keys();
			m_rebind_whip = false;
		}
		else if ( m_rebind_treat  &&  m_treat_key  &&  !m_treat_key->change_pending() )
		{
			string	text(m_treat_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(TREAT_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard treat hotkey!!");
			m_treat_value = m_treat_key->keybind();
			m_treat_mod_value = m_treat_key->modifier();
			m_treat_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_treat_key);
			enable_menu_keys();
			m_rebind_treat = false;
		}
		else if ( m_rebind_brake  &&  m_brake_key  &&  !m_brake_key->change_pending() )
		{
			string	text(m_brake_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(BRAKE_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard brake hotkey!!");
			m_brake_value = m_brake_key->keybind();
			m_brake_mod_value = m_brake_key->modifier();
			m_brake_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_brake_key);
			enable_menu_keys();
			m_rebind_brake = false;
		}
		else if ( m_rebind_left  &&  m_left_key  &&  !m_left_key->change_pending() )
		{
			string	text(m_left_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(LEFT_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard left hotkey!!");
			m_left_value = m_left_key->keybind();
			m_left_mod_value = m_left_key->modifier();
			m_left_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_left_key);
			enable_menu_keys();
			m_rebind_left = false;
		}
		else if ( m_rebind_right  &&  m_right_key  &&  !m_right_key->change_pending() )
		{
			string	text(m_right_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(RIGHT_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard right hotkey!!");
			m_right_value = m_right_key->keybind();
			m_right_mod_value = m_right_key->modifier();
			m_right_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_right_key);
			enable_menu_keys();
			m_rebind_right = false;
		}
		else if ( m_rebind_next_camera  &&  m_next_camera_key  &&  !m_next_camera_key->change_pending() )
		{
			string	text(m_next_camera_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(NEXT_CAMERA_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard next camera hotkey!!");
			m_next_camera_value = m_next_camera_key->keybind();
			m_next_camera_mod_value = m_next_camera_key->modifier();
			m_next_camera_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_next_camera_key);
			enable_menu_keys();
			m_rebind_next_camera = false;
		}
		else if ( m_rebind_previous_camera  &&  m_previous_camera_key  &&  !m_previous_camera_key->change_pending() )
		{
			string	text(m_previous_camera_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(PREVIOUS_CAMERA_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard previous camera hotkey!!");
			m_previous_camera_value = m_previous_camera_key->keybind();
			m_previous_camera_mod_value = m_previous_camera_key->modifier();
			m_previous_camera_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_previous_camera_key);
			enable_menu_keys();
			m_rebind_previous_camera = false;
		}
		else if ( m_rebind_rearview_camera  &&  m_rearview_camera_key  &&  !m_rearview_camera_key->change_pending() )
		{
			string	text(m_rearview_camera_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(REARVIEW_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard rear view camera hotkey!!");
			m_rearview_camera_value = m_rearview_camera_key->keybind();
			m_rearview_camera_mod_value = m_rearview_camera_key->modifier();
			m_rearview_camera_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_rearview_camera_key);
			enable_menu_keys();
			m_rebind_rearview_camera = false;
		}
		else if ( m_rebind_reset_position  &&  m_reset_position_key  &&  !m_reset_position_key->change_pending() )
		{
			string	text(m_reset_position_key->get_as_text());


			parameters.add_pointer(0,static_cast<void*>(&text));
			trigger_event(RESET_POSITION_PCKEY_REBOUND,parameters);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the keyboard reset position hotkey!!");
			m_reset_position_value = m_reset_position_key->keybind();
			m_reset_position_mod_value = m_reset_position_key->modifier();
			m_reset_position_key->disable();
			m_cancel_change->disable();
			check_keyboard_conflict(m_reset_position_key);
			enable_menu_keys();
			m_rebind_reset_position = false;
		}

	#endif	/* _WIN32 */


	if ( m_rebind_cwhip  &&  m_cwhip_key  &&  !m_cwhip_key->change_pending() )
	{
		string	text(m_cwhip_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(WHIP_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller whip hotkey!!");
		m_cwhip_value = m_cwhip_key->keybind();
		m_cwhip_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cwhip_key);
		enable_menu_keys();
		m_rebind_cwhip = false;
	}
	else if ( m_rebind_ctreat  &&  m_ctreat_key  &&  !m_ctreat_key->change_pending() )
	{
		string	text(m_ctreat_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(TREAT_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller treat hotkey!!");
		m_ctreat_value = m_ctreat_key->keybind();
		m_ctreat_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_ctreat_key);
		enable_menu_keys();
		m_rebind_ctreat = false;
	}
	else if ( m_rebind_cbrake  &&  m_cbrake_key  &&  !m_cbrake_key->change_pending() )
	{
		string	text(m_cbrake_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(BRAKE_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller brake hotkey!!");
		m_cbrake_value = m_cbrake_key->keybind();
		m_ctreat_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cbrake_key);
		enable_menu_keys();
		m_rebind_cbrake = false;
	}
	else if ( m_rebind_cleft  &&  m_cleft_key  &&  !m_cleft_key->change_pending() )
	{
		string	text(m_cleft_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(LEFT_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller left hotkey!!");
		m_cleft_value = m_cleft_key->keybind();
		m_cleft_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cleft_key);
		enable_menu_keys();
		m_rebind_cleft = false;
	}
	else if ( m_rebind_cright  &&  m_cright_key  &&  !m_cright_key->change_pending() )
	{
		string	text(m_cright_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(RIGHT_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller right hotkey!!");
		m_cright_value = m_cright_key->keybind();
		m_cright_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cright_key);
		enable_menu_keys();
		m_rebind_cright = false;
	}
	else if ( m_rebind_cnext_camera &&  m_cnext_camera_key  &&  !m_cnext_camera_key->change_pending() )
	{
		string	text(m_cnext_camera_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(NEXT_CAMERA_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller next camera hotkey!!");
		m_cnext_camera_value = m_cnext_camera_key->keybind();
		m_cnext_camera_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cnext_camera_key);
		enable_menu_keys();
		m_rebind_cnext_camera = false;
	}
	else if ( m_rebind_cprevious_camera &&  m_cprevious_camera_key  &&  !m_cprevious_camera_key->change_pending() )
	{
		string	text(m_cprevious_camera_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(PREVIOUS_CAMERA_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller previous camera hotkey!!");
		m_cprevious_camera_value = m_cprevious_camera_key->keybind();
		m_cprevious_camera_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_cprevious_camera_key);
		enable_menu_keys();
		m_rebind_cprevious_camera = false;
	}
	else if ( m_rebind_crearview_camera &&  m_crearview_camera_key  &&  !m_crearview_camera_key->change_pending() )
	{
		string	text(m_crearview_camera_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(REARVIEW_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller rear view camera hotkey!!");
		m_crearview_camera_value = m_crearview_camera_key->keybind();
		m_crearview_camera_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_crearview_camera_key);
		enable_menu_keys();
		m_rebind_crearview_camera = false;
	}
	else if ( m_rebind_creset_position &&  m_creset_position_key  &&  !m_creset_position_key->change_pending() )
	{
		string	text(m_creset_position_key->get_as_text());


		parameters.add_pointer(0,static_cast<void*>(&text));
		trigger_event(RESET_POSITION_CONTROLLERKEY_REBOUND,parameters);
		TEXEngine::Core::LogManager::get().log_message("Game Log:\tRebinding complete for the controller reset position hotkey!!");
		m_creset_position_value = m_creset_position_key->keybind();
		m_creset_position_key->disable();
		m_ccancel_change->disable();
		check_controller_conflict(m_creset_position_key);
		enable_menu_keys();
		m_rebind_creset_position = false;
	}
}

void GameLogicManager::switch_to_fullscreen(const bool fullscreen, const int w, const int h, const bool force){
#ifdef _WIN32
	//bool currentfs = is_fullscreen();
	//if(force || (currentfs != fullscreen)){
	TEXEngine::Core::LogManager::get().log_message("Game Log:\tIn switch_to_fullscreen");
		DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);

		if ( !dwStyle )
		{
			TEXEngine::Core::LogManager::get().log_error("Retrieving the window style failed!");
			return;
		}
		

		DEVMODE dmScreenSettings;                 // device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings); 
				
		if(fullscreen){
			unsigned int settings = 0;
			if (  EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmScreenSettings) > 0 ){
				if(!m_fullscreen){
					TEXEngine::Core::LogManager::get().log_error("Game Log:\tIn switch_to_fullscreen, using DEVMODE width = %i, height = %i",dmScreenSettings.dmPelsWidth,dmScreenSettings.dmPelsHeight);
					if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
						TEXEngine::Core::LogManager::get().log_error("Switching to fullscreen failed!");
					}else{
						m_fullscreen = true;
						//let's go fullscreen then
						dwStyle &= ~(WS_CAPTION | WS_SIZEBOX);
						// apply new style
						if ( !SetWindowLong( m_hwnd, GWL_STYLE, dwStyle ) ) {
							TEXEngine::Core::LogManager::get().log_error("SetWindowLong(GWL_STYLE) failed");
						}
						// resize window and bring to top
						if(!SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, dmScreenSettings.dmPelsWidth,dmScreenSettings.dmPelsHeight,0)){
							TEXEngine::Core::LogManager::get().log_error("SetWindowPos failed");
						}
					}
				}
			}
			else
				TEXEngine::Core::LogManager::get().log_error("Getting display settings failed!");
		}else{
			m_fullscreen = false;
			resize(w, h);
		}
	//}
#endif /*_WIN32*/
}

void GameLogicManager::resize(const int w, const int h){
#ifdef _WIN32
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = w;
	rect.bottom = h;
	DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);

	// add proper window attributes
	dwStyle |= PROPER_WINDOW_ATTRIBUTES;
	SetWindowLongPtr(m_hwnd, GWL_STYLE, dwStyle);
	AdjustWindowRect(&rect, dwStyle, FALSE);
	SetWindowPos(m_hwnd, HWND_TOP, 5, 5, rect.right-rect.left, rect.bottom-rect.top, 0);
	TEXEngine::Core::UIManager::get().aspect_modifier(static_cast<float>(w)/static_cast<float>(h));
	ChangeDisplaySettings(NULL, 0);
#endif _WIN32
}

unsigned int GameLogicManager::get_resolution(const unsigned int which) const{
#ifdef _WIN32

	if(which > 1) return 1; //magic number

	unsigned int res;
	if(OPTION_RESOLUTIONS.size() > 0){
		NCF1* config = m_config.group("config");
		if(config){
			if(sscanf_s(config->get("resolution"), "%Ld", &res) == EOF){
				return OPTION_RESOLUTIONS[0][which];
			}else {
				//the res we store is actually 1-based
				if(res > 0)
					--res;
				if(res >= OPTION_RESOLUTIONS.size())
					return OPTION_RESOLUTIONS[0][which];
				else return OPTION_RESOLUTIONS[res][which];
			}
		}else return OPTION_RESOLUTIONS[0][which];
	}else return which == 0 ? 640 : 480;
#else
	return 1;
#endif /* _WIN32*/
}

bool GameLogicManager::is_fullscreen() const{
#ifdef _WIN32
	NCF1* config = m_config.group("config");
	if(config){
		int v;
		if(sscanf(config->get("fullscreen"), "%d", &v) == EOF){
			return false;
		}
		return v > 0 ? true : false;
	}
	return false;
#else
	return true;
#endif /* _WIN32*/
}

float GameLogicManager::get_volume(const char* which) const{
	NCF1* config = m_config.group("config");
	if(config && which){
		unsigned int value;
		if(sscanf(config->get(which), "%Ld", &value) == EOF){
			value = 50;
		}
		return value/100.0f;
	}
	return 1.0f;
}

float GameLogicManager::get_sound_volume() const{
	return get_volume("sound");
}

float GameLogicManager::get_music_volume() const{
	return get_volume("music");
}

void GameLogicManager::play_menu_music(){
#ifdef _WIN32
	AudioManager::get().stop_music();
	AudioManager::get().play_audio(AssetManager::get().get_audioclip(102));
#endif
}

void GameLogicManager::handle_lua_console_input(std::string& s){
	size_t pos = s.find(' ');
	std::string params;
	std::string command = s;
	if(pos != std::string::npos){
		params = s.substr(pos+1);
		command = s.substr(0, pos);
	}
	TEXEngine::Core::LogManager::get().log_message(s.c_str());
	map<std::string, ConsoleCommand*>::iterator it = m_console_commands.find(command);
	if(it != m_console_commands.end() && it->second){
		it->second->execute(params.c_str());
	}

}

void GameLogicManager::clear_powerup_queue(){
	while(!m_PowerupQueue.empty()) m_PowerupQueue.pop();
}

void GameLogicManager::add_powerup(Vector3f& v){
	m_PowerupQueue.push(v);
}

Vector3f GameLogicManager::front_powerup(){
	if(m_PowerupQueue.empty())
		return Vector3f();

	return m_PowerupQueue.front();
}

void GameLogicManager::pop_and_add_powerup(Vector3f& v){
	m_PowerupQueue.pop();
	m_PowerupQueue.push(v);
}

void GameLogicManager::push_powerup(Vector3f& v){
	m_PowerupQueue.push(v);
}

void GameLogicManager::create_console_commands(){
	clear_console_commands();
	ConsoleCommand* clear = new (std::nothrow) ClearCommand();
	if(clear){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("clear", clear));
		m_console_command_ptrs.push_back(clear);
	}
	ConsoleCommand* quit = new (std::nothrow) QuitCommand();
	if(quit){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("exit", quit));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("quit", quit));
		m_console_command_ptrs.push_back(quit);
	}
	DebugRenderingCommand* debugr = new (std::nothrow) DebugRenderingCommand();
	if(debugr){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("debugr", debugr));
		m_console_command_ptrs.push_back(debugr);
	}
	TreatHackCommand* treatHack = new (std::nothrow) TreatHackCommand();
	if(treatHack){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("treat", treatHack));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("yumyum", treatHack));
		m_console_command_ptrs.push_back(treatHack);
	}
	ClearColorCommand* ccolor = new (std::nothrow) ClearColorCommand();
	if(ccolor){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("ccolor", ccolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("clearcolor", ccolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("color", ccolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("colour", ccolor));
		m_console_command_ptrs.push_back(ccolor);
	}
	ConsoleBackgroundColorCommand* consolecolor = new (std::nothrow) ConsoleBackgroundColorCommand();
	if(ccolor){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("bg", consolecolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("bgcolour", consolecolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("bgcolor", consolecolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("consolecolor", consolecolor));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("consolecolour", consolecolor));
		m_console_command_ptrs.push_back(consolecolor);
	}


	NoMechanicsCommand* nomechanics = new (std::nothrow) NoMechanicsCommand();
	if(ccolor){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("no_mechanics", nomechanics));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("iamfrancis", nomechanics));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("iamthomas", nomechanics));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("godmode", nomechanics));
		m_console_command_ptrs.push_back(nomechanics);
	}


	PlayerPositionCommand* playerpos = new (std::nothrow) PlayerPositionCommand();
	if(playerpos){
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("player", playerpos));
		m_console_commands.insert(pair<std::string, ConsoleCommand*>("playerpos", playerpos));
		m_console_command_ptrs.push_back(playerpos);
	}



}

void GameLogicManager::clear_console_commands(){
	for(vector<ConsoleCommand*>::iterator it = m_console_command_ptrs.begin(); it != m_console_command_ptrs.end(); ++it){
		delete *it;
		*it = NULL;
	}
	m_console_commands.clear();
	m_console_command_ptrs.clear();
}

void GameLogicManager::cheat_set_treats(const unsigned int v){
	if( LogicScene::get_player() )
		LogicScene::get_player()->consumables(v);
}

void GameLogicManager::set_francis_hack(const bool value){
	if( LogicScene::get_player() )
		LogicScene::get_player()->set_francis_hack(value);
}

bool GameLogicManager::get_francis_hack() const{
	if( LogicScene::get_player() )
		return LogicScene::get_player()->get_francis_hack();
	return false;
}
