#include <cmath>
#include "HumanRacer.hpp"
#include "physics\PhysicsManager.hpp"
#include "message\TEXEngineInterface.hpp"
#include "log\logmanager.hpp"
#include "GameEventCodes.h"
#include "audio\AudioManager.hpp"

using namespace X3000;
using X3000::HotkeyKeys;
using TEXEngine::Core::PhysicsManager;
using TEXEngine::Core::AudioManager;
using TEXEngine::Core::Message;
using TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE;
using TEXEngine::Core::MESSAGE_REAL_DATA_TYPE;
using TEXEngine::Core::trigger_event;
using TEXEngine::Physics::VehicleObject;
using TEXEngine::Math::scalar_t;
using TEXEngine::Math::Matrix3x3f;

void HumanRacer::update(double deltaTime)
{
	if (m_state == StateEnum_RACING)
	{
		
		TEXEngine::Physics::VehicleObject* po = static_cast<VehicleObject*>(m_entity->physics_obj());

		if(m_entity && m_francis_hack)
		{
#ifdef	_WIN32
			if ( m_whipKey.is_down()  ||  (m_cwhipKey.is_down() && !m_ctreatKey.is_down() )) // PRESSED switched to DOWN
#else
			if ( m_cwhipKey.is_down() && !m_ctreatKey.is_down() )
#endif	/* _WIN32 */
			{
				if (po) po->accelerate(1500.0f);
			}
			else 
				if (po) po->not_accelerating();
		}



		// THRUST
#ifdef	_WIN32
		if ( m_whipKey.is_pressed()  ||  m_cwhipKey.is_pressed() )
#else
		if ( m_cwhipKey.is_pressed() )
#endif	/* _WIN32 */
		{
			m_pack.apply_whip();
#ifdef _WIN32
			AudioManager::get().play_audio(TEXEngine::Core::AssetManager::get().get_audioclip(107));
#endif
		}

		// GIEF TREAT
#ifdef	_WIN32
		if ( ( m_treatKey.is_pressed()  ||  m_ctreatKey.is_pressed() )  && m_consumables > 0)
#else
		if ( m_ctreatKey.is_pressed() && !m_cwhipKey.is_down() && m_consumables > 0)
#endif	/* _WIN32 */
		{
			m_pack.apply_treat(); 
			m_consumables--; 
#ifdef _WIN32
			AudioManager::get().play_audio(TEXEngine::Core::AssetManager::get().get_audioclip(108));
			AudioManager::get().play_audio(TEXEngine::Core::AssetManager::get().get_audioclip(108));
#endif
		}

		m_pack.update(deltaTime);
		apply_controls(deltaTime);
		m_currentLap += deltaTime;


		send_stats();

		m_waypoint_missed = (m_lastWaypointObject && m_nextWaypointObject && 
							 Vector3f(po->get_pos() - m_lastWaypointObject->get_pos()).length() > 
							 Vector3f(m_nextWaypointObject->get_pos() - m_lastWaypointObject->get_pos()).length());
		
		TEXEngine::Asset::Light *HackLightPos = TEXEngine::AssetManager::get().get_light(2);
		TEXEngine::Asset::Light *HackLightDir = TEXEngine::AssetManager::get().get_light(3);
		if (HackLightPos && po)
		{
			HackLightPos->position(po->get_pos());
		}
		if (HackLightDir && po)
		{
			HackLightDir->position(Vector3f(po->get_front_vector()));
		}
	}

	if (m_state == StateEnum_RACING  ||  m_state == StateEnum_READY )
		update_meshes();


}

void HumanRacer::init_controls( const unsigned long controller_id  ,
								#ifdef	_WIN32
									const unsigned long whip_key , const unsigned long treat_key , 
									const unsigned long brake_key , const unsigned long left_key , const unsigned long right_key ,
									const unsigned long whip_modifier , const unsigned long treat_modifier , const unsigned long brake_modifier , 
									const unsigned long left_modifier , const unsigned long right_modifier , 
								#endif	/* _WIN32 */
								const unsigned long controller_whip_key , const unsigned long controller_treat_key , 
								const unsigned long controller_brake_key , const unsigned long controller_left_key , const unsigned long controller_right_key )
{	
	#ifdef	_WIN32
		m_whipKey.keybind(whip_key); 
		m_treatKey.keybind(treat_key);
		m_brakeKey.keybind(brake_key);
		m_leftKey.keybind(left_key);
		m_rightKey.keybind(right_key);

		m_whipKey.modifier(whip_modifier); 
		m_treatKey.modifier(treat_modifier);
		m_brakeKey.modifier(brake_modifier);
		m_leftKey.modifier(left_modifier);
		m_rightKey.modifier(right_modifier);

		m_whipKey.down_event(WHIP_HAPPENED); 
		m_treatKey.down_event(TREAT_HAPPENED);
		m_brakeKey.down_event(BRAKE_HAPPENED);
		m_leftKey.down_event(LEFT_HAPPENED);
		m_rightKey.down_event(RIGHT_HAPPENED);
	#endif	/* _WIN32 */

	m_cwhipKey.keybind(controller_whip_key); 
	m_ctreatKey.keybind(controller_treat_key);
	m_cbrakeKey.keybind(controller_brake_key);
	m_cleftKey.keybind(controller_left_key);
	m_crightKey.keybind(controller_right_key);

	m_cwhipKey.controller_id(controller_id); 
	m_ctreatKey.controller_id(controller_id);
	m_cbrakeKey.controller_id(controller_id);
	m_cleftKey.controller_id(controller_id);
	m_crightKey.controller_id(controller_id);

	m_cwhipKey.down_event(WHIP_HAPPENED); 
	m_ctreatKey.down_event(TREAT_HAPPENED);
	m_cbrakeKey.down_event(BRAKE_HAPPENED);
	m_cleftKey.down_event(LEFT_HAPPENED);
	m_crightKey.down_event(RIGHT_HAPPENED);
}

void HumanRacer::enable_controls()
{
	#ifdef	_WIN32
		m_whipKey.enable();
		m_treatKey.enable();
		m_brakeKey.enable();
		m_leftKey.enable();
		m_rightKey.enable();
	#endif	/* _WIN32 */

	m_cwhipKey.enable();
	m_ctreatKey.enable();
	m_cbrakeKey.enable();
	m_cleftKey.enable();
	m_crightKey.enable();
}

void HumanRacer::disable_controls()
{
	#ifdef	_WIN32
		m_whipKey.disable();
		m_treatKey.disable();
		m_brakeKey.disable();
		m_leftKey.disable();
		m_rightKey.disable();
	#endif	/* _WIN32 */

	m_cwhipKey.disable();
	m_ctreatKey.disable();
	m_cbrakeKey.disable();
	m_cleftKey.disable();
	m_crightKey.disable();
}

void HumanRacer::apply_controls(double deltaTime)
{
	if (!m_entity) return;
	Vector3f ForceContainer; 
	TEXEngine::Physics::VehicleObject* SledObj = static_cast<VehicleObject*>(m_entity->physics_obj());
	
	if (!SledObj) return; 

	m_thrustValue = m_pack.thrust(); 

	// STEERING
#ifdef	_WIN32
	if ( ( m_leftKey.is_down()  ||  m_cleftKey.is_down() )  &&  !( m_rightKey.is_down()  ||  m_crightKey.is_down() ) )
#else
	if ( m_cleftKey.is_down()  &&  !m_crightKey.is_down() )
#endif	/* _WIN32 */
	{
		if(m_francis_hack)
			SledObj->turn_left_increment();
		m_steerValue = 1.0f; 
	}
#ifdef	_WIN32
	else if ( ( m_rightKey.is_down()   ||  m_crightKey.is_down() )  &&  !( m_leftKey.is_down()  ||  m_cleftKey.is_down() ) )
#else
	else if ( m_crightKey.is_down()  &&  !m_cleftKey.is_down() )
#endif	/* _WIN32 */
	{
		if(m_francis_hack)
			SledObj->turn_right_increment();
		m_steerValue = -1.0f;
		
	}
#ifdef	_WIN32
	else if ( ( m_rightKey.is_down()   ||  m_ctreatKey.is_down() )  &&  ( m_leftKey.is_down()  ||  m_ctreatKey.is_down() ) && ( m_whipKey.is_pressed() || m_cwhipKey.is_pressed()))
#else
	else if ( m_ctreatKey.is_down() && m_cwhipKey.is_pressed() )
#endif	/* _WIN32 */
	{
		if(m_francis_hack)
			SledObj->apply_rel_impulse(Vector3f(4000.0f, 0.0f, 0.0f));
	}
	else
	{
		m_steerValue = 0.0f;
		if(m_francis_hack)
			SledObj->not_turning();
	}


	// BRAKING
#ifdef	_WIN32
	if ( m_brakeKey.is_down()  ||  m_cbrakeKey.is_down() )
#else
	if ( m_cbrakeKey.is_down() )
#endif	/* _WIN32 */
	{
		//m_brakeValue = min(1.0f, m_brakeValue + 0.3f * (float)deltaTime);
		//SledObj->brake(100.0f);
		m_brakeValue = 1.0f;
	}
	else {
		//SledObj->not_braking();
		m_brakeValue = 0.0f;	
	}

#ifdef _WIN32
	if ((m_brakeKey.is_down() && m_leftKey.is_down() && m_rightKey.is_down()) || (m_cbrakeKey.is_down() && m_cleftKey.is_down() &&  m_crightKey.is_down()))
#else
	if (m_cbrakeKey.is_down() && m_cleftKey.is_down() &&  m_crightKey.is_down())
#endif /* _WIN32 */ 
	{
		SledObj->brake();
		//m_handBrake = 500.0f;
	}
	else
	{
		//m_handBrake = 0.0f; 
		//SledObj->not_braking();
		if(m_brakeValue > 0)
			SledObj->brake(100.0f);
		else
			SledObj->not_braking();
	}


	// APPLY FORCES
	if(m_entity)
	{
		TEXEngine::Physics::VehicleObject* po = static_cast<VehicleObject*>(m_entity->physics_obj());
		if(po)
		{
			if(!m_francis_hack)
			{
				// THRUST
				if(m_pack.thrust() > 0)
				{
					if(m_pack.thrust() > 2)
						po->accelerate(2000.0f);
					po->accelerate(800.0f*m_pack.thrust());
				}
				else
					po->not_accelerating();
			
				// STEERING
				if(abs(m_pack.deviation()) < 0.1f && abs(m_steerValue) < 0.0001f)
					po->not_turning();
				else
				{
					if(m_steerValue > 0)
						po->turn_left(po->get_steer_inc() + m_pack.deviation()*(po->get_steer_inc()));
					else if(m_steerValue < 0)
						po->turn_right(po->get_steer_inc() - m_pack.deviation()*(po->get_steer_inc()));
					else
					{
						if(m_pack.deviation() > 0.1f)
							po->turn_left(m_pack.deviation()*(po->get_steer_inc()));
						else if(m_pack.deviation() < 0.1f)
							po->turn_right(-m_pack.deviation()*(po->get_steer_inc()));
					}
					//po->turn_left(m_steerValue*(po->get_steer_inc()) + m_pack.deviation()*(po->get_steer_inc()));
				}
			}
			m_pack.speed((float)Vector2f((scalar_t)po->get_velocity().x, (scalar_t)po->get_velocity().z).length());
		}
	}

}

void HumanRacer::send_stats()
{
	/* Send a message that the UI is listening to, with all the necessary parameters to update the hud
		for a given player ( the player is identified by the controller_id (
	*/
	Message		parameters(5,8,0);
	Vector3f	EDM = edm();

	parameters.add_integral_data(2,static_cast<MESSAGE_INTEGRAL_DATA_TYPE>(m_cwhipKey.controller_id()));
	parameters.add_integral_data(3,static_cast<MESSAGE_INTEGRAL_DATA_TYPE>(consumables()));
	parameters.add_integral_data(4,static_cast<MESSAGE_INTEGRAL_DATA_TYPE>(lap_number()));
	parameters.add_real_data(0,static_cast<MESSAGE_REAL_DATA_TYPE>(EDM.x));
	parameters.add_real_data(1,static_cast<MESSAGE_REAL_DATA_TYPE>(EDM.y));
	parameters.add_real_data(2,static_cast<MESSAGE_REAL_DATA_TYPE>(EDM.z));
	parameters.add_real_data(3,static_cast<MESSAGE_REAL_DATA_TYPE>(m_pack.speed() * 0.04f));
	parameters.add_real_data(4,static_cast<MESSAGE_REAL_DATA_TYPE>(lap_progress()));
	parameters.add_real_data(5,static_cast<MESSAGE_REAL_DATA_TYPE>(current_lap()));
	parameters.add_real_data(6,static_cast<MESSAGE_REAL_DATA_TYPE>(best_lap()));
	parameters.add_real_data(7,static_cast<MESSAGE_REAL_DATA_TYPE>(max_laps()));
	trigger_event(PLAYER_STATS,parameters);
};
void HumanRacer::next_cam()
{
	//m_camera_type = static_cast<CameraType>((m_camera_type+1)%NUMOFCAMS);
	m_camera_type = static_cast<CameraType>((m_camera_type+1)%3);
	set_cam_pos();
}

void HumanRacer::prev_cam()
{
	//m_camera_type = static_cast<CameraType>((m_camera_type+NUMOFCAMS-1)%NUMOFCAMS);
	m_camera_type = static_cast<CameraType>((m_camera_type+2)%3);
	set_cam_pos();
}

void HumanRacer::toggle_rear_cam()
{
	m_rear_cam = !m_rear_cam;
	set_cam_pos();
}

void HumanRacer::update_camera()
{
	if(m_entity)
	{
		TEXEngine::Physics::VehicleObject* po = static_cast<VehicleObject*>(m_entity->physics_obj());
		if(po)
		{
			Camera *cam = TEXEngine::AssetManager::get().get_camera(1);
			//cam->set_camera_type(TEXEngine::Asset::TEXENG_CAMERA_FPS);
			if (cam && (cam->get_camera_type() == TEXENG_CAMERA_CHASE))
			{
				//cam->target(po->get_pos());
				//cam->set_target_velocity(po->get_velocity());
				Vector3f pos = po->get_pos();
				

				int inv = -1;
				if(m_rear_cam)
				{
					cam->target(Vector3f(pos.x,pos.y+2.0f,pos.z));
					//inv = 1;
				}
				else if(m_camera_type == TPS)
				{
					cam->target(Vector3f(pos.x,pos.y+2.0f,pos.z));
				}
				else if(m_camera_type == FPS)
				{
					cam->target(Vector3f(pos.x,pos.y+4.0f,pos.z));
				}
				else if(m_camera_type == OVER)
				{
					Vector3f tmp = Vector3f(pos.x,pos.y,pos.z);


					float clampx = 110.0f;
					float clampz = 170.0f;

					//TEXEngine::LogManager::get().log_message("TESTING Y: %f",pos.y);

					if(tmp.x > clampx)
						tmp.x = clampx;
					else if(tmp.x < -clampx)
						tmp.x = -clampx;

					if(tmp.z > clampz)
						tmp.z = clampz;
					else if(tmp.z < -clampz)
						tmp.z = -clampz;

					cam->target(tmp);
				}

				// used to debug
				//Vector3f temp = cam->target();
				
				Vector3f ori = po->get_front_vector();
				ori = Vector3f(ori.x,0.0f,ori.z).normalized();

				if(m_camera_type == OVER && !m_rear_cam)
					cam->set_target_velocity(Vector3f(0.0f,0.0f,1.0f));
				else
					cam->set_target_velocity(ori);
			}
		}
	}
}

bool HumanRacer::is_rear_toggled()
{
	return m_rear_cam;
}

void HumanRacer::set_cam_pos()
{
	Camera *cam = TEXEngine::AssetManager::get().get_camera(1);
	if(cam)
	{
		if(m_rear_cam)
		{
			cam->set_chase_cam_position(Vector3f(0.0f, 5.0f, -10.0f));
		}
		else if(m_camera_type == TPS)
		{
			cam->set_chase_cam_position(Vector3f(0.0f, 5.0f, 20.0f));
		}
		else if(m_camera_type == FPS)
		{
			cam->set_chase_cam_position(Vector3f(0.0f, 0.0f, 6.0f));
		}
		else if(m_camera_type == OVER)
		{
			cam->set_chase_cam_position(Vector3f(0.0f, 90.0f, 1.0f));
		}
	}
}