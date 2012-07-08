#include <cmath>
#include "physics\PhysicsManager.hpp"
#include "log\logmanager.hpp"
#include "AIRacer.hpp"
using TEXEngine::Core::PhysicsManager;
using TEXEngine::Core::LogManager;
using TEXEngine::Math::scalar_t;
using TEXEngine::Math::Matrix3x3f;
using TEXEngine::Math::Quaternion;

void AIRacer::apply_controls(double deltaTime)
{	
	if (!m_entity) return;

	Vector3f ForceContainer; 
	TEXEngine::Physics::VehicleObject* SledObj = static_cast<VehicleObject*>(m_entity->physics_obj());

	if (!SledObj) return; 

	// AI CODE
	////////////////////////////

	if(m_pack.thrust() < 0.4f)
		m_pack.apply_whip();
	if(m_pack.edm().x < 0.4f)
		m_pack.apply_treat();
	if(m_pack.edm().y < 0.7f)
		m_pack.apply_whip();

	// STEERING
	if (!m_lastWaypointObject || !m_nextWaypointObject ) return; 

	Vector3f r2n = m_nextWaypointObject->get_pos() - SledObj->get_pos();
	r2n.normalize();

	
	Quaternion q = SledObj->get_quaternion();
	Vector3f v = Vector3f(1.0f,0.0f,0.0f);
	Vector3f ori = v + cross( Vector3f(q.x,q.y,q.z) , cross( Vector3f(q.x,q.y,q.z) , v )+( v * q.w ) )*2.0f;
	//Vector3f ori = SledObj->get_front_vector();
	Vector3f ori3f = Vector3f(ori.x,0.0f,ori.z).normalized();
	Matrix3x3f rotmat;
	rotmat.load_identity();
	rotmat.rotate(Vector3f(0.0f,1.0f,0.0f),1.57f);
	ori3f = rotmat * ori3f;
	m_steerValue = TEXEngine::Math::dot(r2n,ori3f);
	

	// BRAKING??
	/*
	m_brakeValue = max(0.0f, m_steerValue - 0.8f) * 5.0f; 

	if ( m_brakeValue > 0.0f )
		SledObj->brake(100.0f);
	else
		SledObj->not_braking();*/


	// APPLY FORCES
	if(m_entity)
	{
		TEXEngine::Physics::VehicleObject* po = static_cast<VehicleObject*>(m_entity->physics_obj());
		if(po)
		{
			// THRUST
			if(m_pack.thrust() > 0)
				po->accelerate(800.0f*m_pack.thrust());
			else
				po->not_accelerating();
			
			// STEERING
			if(!m_is_easy)
			{
				if(m_steerValue > 0.1)
					po->turn_right(po->get_steer_inc());
				else if(m_steerValue < -0.1)
					po->turn_left(po->get_steer_inc());
				else
					po->not_turning();
			}
			else
			{
				if(abs(m_pack.deviation()) < 0.1f && abs(m_steerValue) < 0.0001f)
					po->not_turning();
				else
				{
					if(m_steerValue > 0.1f)
						po->turn_right(po->get_steer_inc() - m_pack.deviation()*(po->get_steer_inc()));
					else if(m_steerValue < -0.1f)
						po->turn_left(po->get_steer_inc() + m_pack.deviation()*(po->get_steer_inc()));
					else
					{
						if(m_pack.deviation() > 0.1f)
							po->turn_left(m_pack.deviation()*(po->get_steer_inc()));
						else if(m_pack.deviation() < -0.1f)
							po->turn_right(-m_pack.deviation()*(po->get_steer_inc()));
					}
				}
			}

			m_pack.speed((float)Vector2f((scalar_t)po->get_velocity().x, (scalar_t)po->get_velocity().z).length());
		}
	}
}

void AIRacer::set_easy(bool value)
{
	m_is_easy = value;
}


void AIRacer::update(double deltaTime)
{	
	if (m_state == StateEnum_RACING)
	{
		//if (m_pack.edm().y < 0.1f) m_pack.apply_whip();
		if (m_pack.edm().z < 0.5f && m_consumables > 0)
		{
			m_pack.apply_treat();
			if(m_is_easy)
				m_consumables--; // COMMENTED TO PROVIDE AI CHEAT FOR UNLIMITED TREATS 
		}
		m_pack.update(deltaTime);
		apply_controls(deltaTime);
		m_currentLap += deltaTime;
	}

	if (m_state == StateEnum_RACING  ||  m_state == StateEnum_READY )
		update_meshes();
}