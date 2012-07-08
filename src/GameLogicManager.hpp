#ifndef XSLEDDING_GAMELOGICMANAGER_HPP_INCLUDED
#define XSLEDDING_GAMELOGICMANAGER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <vector>

#include "message/TEXEngineInterface.hpp"
#include "log/logmanager.hpp"
#include "keyboardHotkey.hpp"
#include "port/Timer.h"

#include "controllerHotkey.hpp"
#include "LogicScene.hpp"
#include "util/ncf.hpp"
#include "MenuParams.h"

class ConsoleCommand;

using namespace X3000;

class GameLogicManager : public TEXEngine::Core::Object{
private:


	enum CollisionGroup { PLAYER, STATIC, OTHER };

	GameLogicManager();

	static GameLogicManager m_instance;


	TEXEngine::Util::NCF1 m_config;

	std::map<std::string, ConsoleCommand*> m_console_commands;
	std::vector<ConsoleCommand*> m_console_command_ptrs;


	// Queue used for designating/obtaining powerup locations 
	//////////////////////////////////////////////////////////
	std::queue<Vector3f> m_PowerupQueue;


	LogicScene* m_soloScene;
	LogicScene* m_versusScene;
	LogicScene* m_menuScene;
	LogicScene* m_current;

	#ifdef	_WIN32
		KeyboardHotkey* m_reload_ui;
		KeyboardHotkey* m_previous;
		KeyboardHotkey* m_next;
		KeyboardHotkey* m_left;
		KeyboardHotkey* m_right;
		KeyboardHotkey* m_select;
		KeyboardHotkey* m_ingame_select;
		KeyboardHotkey* m_console;
		KeyboardHotkey* m_cancel_change;
		KeyboardHotkey* m_whip_key;
		KeyboardHotkey* m_treat_key;
		KeyboardHotkey* m_brake_key;
		KeyboardHotkey* m_left_key;
		KeyboardHotkey* m_right_key;
		KeyboardHotkey* m_next_camera_key;
		KeyboardHotkey* m_previous_camera_key;
		KeyboardHotkey* m_rearview_camera_key;
		KeyboardHotkey* m_reset_position_key;
	#endif	/* _WIN32 */

	ControllerHotkey* m_cprevious;
	ControllerHotkey* m_cnext;
	ControllerHotkey* m_cleft;
	ControllerHotkey* m_cright;
	ControllerHotkey* m_cselect;
	ControllerHotkey* m_cingame_select;
	ControllerHotkey* m_cconsole;
	ControllerHotkey* m_ccancel_change;
	ControllerHotkey* m_cwhip_key;
	ControllerHotkey* m_ctreat_key;
	ControllerHotkey* m_cbrake_key;
	ControllerHotkey* m_cleft_key;
	ControllerHotkey* m_cright_key;
	ControllerHotkey* m_cnext_camera_key;
	ControllerHotkey* m_cprevious_camera_key;
	ControllerHotkey* m_crearview_camera_key;
	ControllerHotkey* m_creset_position_key;

	#ifdef	_WIN32
		unsigned int m_reload_value;
		unsigned int m_menu_up_value;
		unsigned int m_menu_down_value;
		unsigned int m_menu_left_value;
		unsigned int m_menu_right_value;
		unsigned int m_menu_select_value;
		unsigned int m_menu_cancel_value;
		unsigned int m_console_value;
		unsigned int m_pause_scene_value;
		unsigned int m_whip_value;
		unsigned int m_treat_value;
		unsigned int m_brake_value;
		unsigned int m_left_value;
		unsigned int m_right_value;
		unsigned int m_next_camera_value;
		unsigned int m_previous_camera_value;
		unsigned int m_rearview_camera_value;
		unsigned int m_reset_position_value;
		unsigned int m_reload_mod_value;
		unsigned int m_menu_up_mod_value;
		unsigned int m_menu_down_mod_value;
		unsigned int m_menu_left_mod_value;
		unsigned int m_menu_right_mod_value;
		unsigned int m_menu_select_mod_value;
		unsigned int m_menu_cancel_mod_value;
		unsigned int m_console_mod_value;
		unsigned int m_pause_scene_mod_value;
		unsigned int m_whip_mod_value;
		unsigned int m_treat_mod_value;
		unsigned int m_brake_mod_value;
		unsigned int m_left_mod_value;
		unsigned int m_right_mod_value;
		unsigned int m_next_camera_mod_value;
		unsigned int m_previous_camera_mod_value;
		unsigned int m_rearview_camera_mod_value;
		unsigned int m_reset_position_mod_value;
		unsigned int m_previous_whip_value;
		unsigned int m_previous_treat_value;
		unsigned int m_previous_brake_value;
		unsigned int m_previous_left_value;
		unsigned int m_previous_right_value;
		unsigned int m_previous_next_camera_value;
		unsigned int m_previous_previous_camera_value;
		unsigned int m_previous_rearview_camera_value;
		unsigned int m_previous_reset_position_value;
		unsigned int m_previous_whip_mod_value;
		unsigned int m_previous_treat_mod_value;
		unsigned int m_previous_brake_mod_value;
		unsigned int m_previous_left_mod_value;
		unsigned int m_previous_right_mod_value;
		unsigned int m_previous_next_camera_mod_value;
		unsigned int m_previous_previous_camera_mod_value;
		unsigned int m_previous_rearview_camera_mod_value;
		unsigned int m_previous_reset_position_mod_value;
	#endif	/* _WIN32 */

	unsigned int m_cmenu_up_value;
	unsigned int m_cmenu_down_value;
	unsigned int m_cmenu_left_value;
	unsigned int m_cmenu_right_value;
	unsigned int m_cmenu_select_value;
	unsigned int m_cmenu_cancel_value;
	unsigned int m_cconsole_value;
	unsigned int m_cpause_scene_value;
	unsigned int m_cwhip_value;
	unsigned int m_ctreat_value;
	unsigned int m_cbrake_value;
	unsigned int m_cleft_value;
	unsigned int m_cright_value;
	unsigned int m_cnext_camera_value;
	unsigned int m_cprevious_camera_value;
	unsigned int m_crearview_camera_value;
	unsigned int m_creset_position_value;
	unsigned int m_previous_cwhip_value;
	unsigned int m_previous_ctreat_value;
	unsigned int m_previous_cbrake_value;
	unsigned int m_previous_cleft_value;
	unsigned int m_previous_cright_value;
	unsigned int m_previous_cnext_camera_value;
	unsigned int m_previous_cprevious_camera_value;
	unsigned int m_previous_crearview_camera_value;
	unsigned int m_previous_creset_position_value;

	#ifdef	_WIN32
		bool m_rebind_whip;
		bool m_rebind_treat;
		bool m_rebind_brake; 
		bool m_rebind_left;
		bool m_rebind_right;
		bool m_rebind_next_camera;
		bool m_rebind_previous_camera;
		bool m_rebind_rearview_camera;
		bool m_rebind_reset_position;
	#endif	/* _WIN32 */

	bool m_rebind_cwhip;
	bool m_rebind_ctreat;
	bool m_rebind_cbrake;
	bool m_rebind_cleft;
	bool m_rebind_cright;
	bool m_rebind_cnext_camera;
	bool m_rebind_cprevious_camera;
	bool m_rebind_crearview_camera;
	bool m_rebind_creset_position;


	bool m_console_update_pending;
	bool m_running;
	bool m_fullscreen;

	
	void load_lua_stuff();

	//splitting the load functions into several functions in the interest of not having just one methods with 79856743 lines of code
	void load_animal_data() const;
	void load_track_data() const;
	void load_lua_scripts() const;
	void push_game_variables_to_lua();


	#ifdef	_WIN32
		void check_keyboard_conflict( const KeyboardHotkey* changed_key );
	#endif	/* _WIN32 */

	void check_controller_conflict( const ControllerHotkey* changed_key );
	void send_keybind_texts();
	bool key_change_active();

	void disable_menu_keys();
	void init_keys_and_scenes();

	unsigned int get_resolution(const unsigned int which) const;

	void resize(const int, const int);

	void update_handler();

	void cancel_keybind_change_handler();

	void cancel_options_handler();

	void save_options_handler(const TEXEngine::Core::Message&);

	void fetch_menu_params_handler(const TEXEngine::Core::Message&);

	void request_keychange_handler(const unsigned int e);

	void request_rebind_key(Hotkey* cancel, bool& rebind, Hotkey* key, const char*);

	float get_volume(const char* which) const;

	void toggle_console();

	void play_menu_music();

	void handle_lua_console_input(std::string& s);//e.g. handle cheatZor!!!1

	void create_console_commands();

	void clear_console_commands();

	void setup_physics();

public:
#ifdef _WIN32
	static const long PROPER_WINDOW_ATTRIBUTES = WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_CAPTION;
#endif /*_WIN32*/

	static GameLogicManager& Get();

#ifdef _WIN32
	HWND m_hwnd;
#endif /*_WIN32*/

	void init_settings();
	void enable_menu_keys();

	void on_event(const unsigned long event_code, const TEXEngine::Core::Message&);

	void setup();

	void terminate();

	bool is_running() const{
		return m_running;
	}

	void set_running(const bool b){
		m_running = b;
	}
	
	unsigned int get_resolution_width() const{
		return get_resolution(0);
	}

	unsigned int get_resolution_height() const{
		return get_resolution(1);
	}

	bool is_fullscreen() const;

	void switch_to_fullscreen(const bool fullscreen, const int width, const int height, const bool force = false);

	float get_sound_volume() const;

	float get_music_volume() const;

	void update_console();

	void clear_powerup_queue();
	void add_powerup(Vector3f& v);
	Vector3f front_powerup();
	void pop_and_add_powerup(Vector3f& v);
	void push_powerup(Vector3f& v);

	void cheat_set_treats(const unsigned int v);
	void set_francis_hack(const bool value);
	bool get_francis_hack() const;



	~GameLogicManager();
};

#endif