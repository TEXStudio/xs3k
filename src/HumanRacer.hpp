#ifndef XSLEDDING_HUMANRACER_HPP_INCLUDED
#define XSLEDDING_HUMANRACER_HPP_INCLUDED

#include "keyboardHotkey.hpp"
#include "controllerHotkey.hpp"
#include "port/Timer.h"
#include "log/logmanager.hpp"
#include "Racer.hpp" 

using TEXEngine::Port::Timer;
using namespace X3000;



class HumanRacer : public Racer
{
private:
	
	#ifdef	_WIN32
		KeyboardHotkey m_whipKey;
		KeyboardHotkey m_treatKey;
		KeyboardHotkey m_brakeKey;
		KeyboardHotkey m_leftKey;
		KeyboardHotkey m_rightKey;
	#endif	/* _WIN32 */
	
	ControllerHotkey m_cwhipKey;
	ControllerHotkey m_ctreatKey;
	ControllerHotkey m_cbrakeKey;
	ControllerHotkey m_cleftKey;
	ControllerHotkey m_crightKey;

	Timer m_controlTimer; 

	enum CameraType { TPS, FPS, OVER };

	CameraType m_camera_type;
	bool m_rear_cam;
	bool m_francis_hack;
	bool m_waypoint_missed;
public:
	HumanRacer( const unsigned long base_id = 0 ) : 
	#ifdef	_WIN32
		m_whipKey(base_id+5) , m_treatKey(base_id+6) , m_brakeKey(base_id+7),
		m_leftKey(base_id+8), m_rightKey(base_id+9) ,
	#endif	/* _WIN32 */
	m_cwhipKey(base_id) , m_ctreatKey(base_id+1) , m_cbrakeKey(base_id+2),
	m_cleftKey(base_id+3), m_crightKey(base_id+4), m_camera_type(TPS), m_rear_cam(false), m_francis_hack(false),
		m_waypoint_missed(false) {
		
	}
	
	~HumanRacer() {
	}

	void apply_controls(double deltaTime);
	void init_controls( const unsigned long controller_id  ,
						#ifdef	_WIN32
							const unsigned long whip_key , const unsigned long treat_key , 
							const unsigned long brake_key , const unsigned long left_key , const unsigned long right_key ,
							const unsigned long whip_modifier , const unsigned long treat_modifier , const unsigned long brake_modifier , 
							const unsigned long left_modifier , const unsigned long right_modifier , 
						#endif	/* _WIN32 */
						const unsigned long controller_whip_key , const unsigned long controller_treat_key , 
						const unsigned long controller_brake_key , const unsigned long controller_left_key , const unsigned long controller_right_key );
	void update(double deltaTime); 
	void disable_controls(); 
	void enable_controls();
	void update_camera(); 
	void next_cam();
	void prev_cam();
	void set_cam_pos();
	void toggle_rear_cam();
	bool is_rear_toggled();
	void send_stats();

	inline void set_francis_hack(const bool value) { m_francis_hack = value; };

	inline bool get_francis_hack() const { return m_francis_hack; }
};

#endif
