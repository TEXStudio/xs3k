#include <string>
#include "asset\assetManager.hpp"
#include "physics\PhysicsManager.hpp"
#include "log\logmanager.hpp"
#include "audio\audiomanager.hpp"

#include "Racer.hpp" 
#include "Animal.hpp"

#include "GameLogicManager.hpp"

using TEXEngine::Core::AssetManager;
using TEXEngine::Core::PhysicsManager;
using TEXEngine::Core::AudioManager;

void Racer::init(const int laps , const int consumables, const std::string& name, const std::vector<std::string>& animals, const std::string& species, const int playernumber, const bool IsHuman)
{
	m_maxlaps = laps;
	m_consumables = consumables;
	m_name = name; 
	m_pack.init(animals, species, IsHuman);
	m_lapNumber = 1;
	m_lastWaypoint = 0;
	m_bestLap = 9999.0;
	m_currentLap = 0.0;
	//m_entityPack = NULL;
	m_lastWaypointObject = NULL;
	m_nextWaypointObject = NULL; 

	if (m_entity)
	{
		m_lastWaypointObject = AssetManager::get().get_entity(100)->physics_obj();
		m_nextWaypointObject = AssetManager::get().get_entity(101)->physics_obj();
		m_waypointDirection = Vector3f(-1, 0, 0);

		if (m_lastWaypointObject && m_nextWaypointObject) m_waypointDirection = Vector3f(m_nextWaypointObject->get_pos() - m_lastWaypointObject->get_pos()).normalized();
	}
	m_packTransform.load_identity();

}

void Racer::set_entity(int id)
{
	m_entity = AssetManager::get().get_entity(id);
}

void Racer::set_entityPack(int id)
{
	m_entityPack = AssetManager::get().get_entity(id); 
}

void Racer::set_entitySled(int id)
{
	m_entitySled = AssetManager::get().get_entity(id); 
}

void Racer::update_meshes()
{
	if(m_needs_reset)
	{
		reset_to_last_waypoint();
		m_needs_reset = false;
	}
	if(m_entity)
	{
		PhysicsObj* po = m_entity->physics_obj();
		if(po)
		{
			VehicleObject* vehicle = static_cast<VehicleObject*>(po);
			if(vehicle)
			{
				float steer = vehicle->get_steering();
				
				m_steer = steer*0.314f;
				if(m_steer > 3.14f || m_steer < -3.14f)
					m_steer = 0.0f;

				// setting sled at its position
				Matrix4x4f temp = po->get_transform_matrix();
				temp.transpose();
				temp.translate(Vector3f(-4.1f,0.0f,0.0f));
				temp.rotate(Vector3f(0.0f,1.0f,0.0f),-m_steer);
				if(m_entitySled)
					m_entitySled->matrix(temp);

				// setting the pack at its position
				temp = po->get_transform_matrix();
				temp.transpose();
				temp.translate(Vector3f(3.9f,0.0f,0.0f));
				temp.rotate(Vector3f(0.0f,1.0f,0.0f),m_steer);
				if(m_entityPack)
					m_entityPack->matrix(temp);
			}
		}
	}
}

void Racer::set_position(Vector3f p)
{
	if (!m_entity) return;

	PhysicsObj *SledObj = NULL;
	SledObj = m_entity->physics_obj();

	if (SledObj) SledObj->set_pos(p);
}

void Racer::set_orientation(Vector3f o)
{
}

void Racer::set_state(StateEnum state)
{
	m_state = state; 
}

void Racer::set_lap(int lapNumber)
{
	m_lapNumber = lapNumber;
}

void Racer::inc_laps()
{
	m_lapNumber++;
}

void Racer::inc_wpts()
{
	m_lastWaypoint++; 
}

void Racer::placing(int p)
{
	m_placing = p;
}

Vector3f Racer::edm()
{
	return m_pack.edm();
}

Vector3f Racer::pos()
{
	Vector3f Temp;
	Temp = Vector3f(0,0,0);

	if (m_entity)
	{
		PhysicsObj *SledObj = NULL;
		SledObj = m_entity->physics_obj();
		if (SledObj)
		{
			Temp = SledObj->get_pos();
		}
	}
	return Temp; 
}

double Racer::race_progress() const
{
	if (!m_entity) return 0.0;
	
	PhysicsObj *SledObj = NULL;
	SledObj = m_entity->physics_obj();

	if (!SledObj || !m_lastWaypointObject || !m_nextWaypointObject) return 0.0; 

	Vector3f last2racer = SledObj->get_pos() - m_lastWaypointObject->get_pos();
	Vector3f last2next = m_nextWaypointObject->get_pos() - m_lastWaypointObject->get_pos();
	//last2racer.normalize();
	double len = last2next.length();
	last2next.normalize();

	double dot = TEXEngine::Math::dot(last2racer,last2next)/len;

	if(dot > 1.0)
		dot = 1.0;

	double res = m_lapNumber * 1000 + m_lastWaypoint * 10 + TEXEngine::Math::dot(last2racer,last2next)/len;
	return res; 
}

double Racer::lap_progress() const
{
	return m_lastWaypoint * 0.02;
}

bool Racer::is_finished() const
{
	return (m_lapNumber > m_maxlaps);
}

void Racer::react_to(unsigned int ent_id)
{
	
	unsigned int ID = ent_id;

	if (!m_entity) return; // A CHECK TO PREVENT USAGE BEFORE EVERYTHING IS IN PLACE

	if(m_nextWaypointObject && ID == m_nextWaypointObject->entity())
	{
		TEXEngine::Core::LogManager::get().log_message("W %i",ent_id-100);
		inc_wpts();
		m_lastWaypoint %= 51;
		m_lastWaypointObject = m_nextWaypointObject;
		m_nextWaypointObject = AssetManager::get().get_entity( ( (m_lastWaypoint+1)%51 ) + 100)->physics_obj();
		PhysicsObj *SledObj = NULL;
		SledObj = m_entity->physics_obj();

		if (!m_lastWaypointObject || !m_nextWaypointObject || !SledObj) return; 

		m_resetTransform = SledObj->get_transform_matrix();
		Vector4f line_2(m_resetTransform.get_row_vector(1));
		line_2.y = 1.0f;
		m_resetTransform.set_row_vector(line_2,1);
		TEXEngine::Core::LogManager::get().log_message("SAVED position (%f,%f,%f)",SledObj->get_pos().x,SledObj->get_pos().y,SledObj->get_pos().z);
		
		if(m_lastWaypoint == 0)
		{
			inc_laps();
			m_bestLap = min(m_bestLap, m_currentLap); 
			m_currentLap = 0.0; 
		}
		m_waypointDirection = Vector3f(m_nextWaypointObject->get_pos() - m_lastWaypointObject->get_pos()).normalized();


		if (/*m_lastWaypoint%2 == 1 && */LogicScene::get_player() && m_entity == LogicScene::get_player()->m_entity) // im hoping this doesn't crash
		{
			//Entity* nextWP = AssetManager::get().get_entity( ( (m_lastWaypoint+2)%51 ) + 100);
			Entity* markerEntity = AssetManager::get().get_entity(5);
			if(m_nextWaypointObject && markerEntity){
				//PhysicsObj* po = nextWP->physics_obj();
				PhysicsObj* po2 = markerEntity->physics_obj();
				if(po2){
					po2->set_pos(m_nextWaypointObject->get_pos());
				}
			}
			/*
			Entity* nextWP = AssetManager::get().get_entity( ( (m_lastWaypoint+2)%51 ) + 100);
			Entity* markerEntity = AssetManager::get().get_entity(5);
			if(nextWP && markerEntity){
				PhysicsObj* po = nextWP->physics_obj();
				PhysicsObj* po2 = markerEntity->physics_obj();
				if(po && po2){
					po2->set_pos(po->get_pos());
				}
			}
			*/
		}
	}
	else if ((ID > 150) && (ID <= 153))
	{
		TEXEngine::Core::LogManager::get().log_message("reacting to powerup 1");
		m_pack.apply_powerup(1);

		// Code to re-assign position and move powerup
		Vector3f Temp = GameLogicManager::Get().front_powerup();
	
		PhysicsObj *PowerupObj = NULL;
		Entity* pe = AssetManager::get().get_entity(ID);
		if(pe) PowerupObj = pe->physics_obj();
		if (PowerupObj)
		{
			PowerupObj->set_pos(Temp);
			GameLogicManager::Get().pop_and_add_powerup(Temp);
		}
	}
	else if ((ID > 153) && (ID <= 156))
	{
		TEXEngine::Core::LogManager::get().log_message("reacting to powerup 2");
		m_pack.apply_powerup(2);

		// Code to re-assign position and move powerup
		Vector3f Temp = GameLogicManager::Get().front_powerup();
		PhysicsObj *PowerupObj = NULL;
		Entity* pe = AssetManager::get().get_entity(ID);
		if(pe) PowerupObj = pe->physics_obj();
		if (PowerupObj)
		{
			PowerupObj->set_pos(Temp);
			GameLogicManager::Get().pop_and_add_powerup(Temp);
		}
	}
	else if ((ID > 156) && (ID <= 159))
	{
		TEXEngine::Core::LogManager::get().log_message("reacting to powerup 3");
		m_pack.apply_powerup(3);

		// Code to re-assign position and move powerup
		Vector3f Temp = GameLogicManager::Get().front_powerup();
		PhysicsObj *PowerupObj = NULL;
		PowerupObj = AssetManager::get().get_entity(ID)->physics_obj();
		if (PowerupObj)
		{
			PowerupObj->set_pos(Temp);
			GameLogicManager::Get().pop_and_add_powerup(Temp);
		}
	}
	else if ((ID > 159) && (ID <= 162))
	{
		TEXEngine::Core::LogManager::get().log_message("reacting to powerup 4");
		m_pack.apply_powerup(4);

		// Code to re-assign position and move powerup
		Vector3f Temp = GameLogicManager::Get().front_powerup();
		PhysicsObj *PowerupObj = NULL;
		Entity* pe = AssetManager::get().get_entity(ID);
		if(pe) PowerupObj = pe->physics_obj();
		if (PowerupObj)
		{
			PhysicsObj *po = m_entity->physics_obj();
			if (po)
			{
				po->apply_rel_force(Vector3f(10000.0f, 0.0f, 0.0f));
				po->apply_rel_impulse(Vector3f(10000.0f, 0.0f, 0.0f));
			}
			PowerupObj->set_pos(Temp);
			GameLogicManager::Get().pop_and_add_powerup(Temp);

#ifdef _WIN32
			if (m_entity == LogicScene::get_player()->m_entity) AudioManager::get().play_audio(TEXEngine::Core::AssetManager::get().get_audioclip(109));
#endif

		}
	}
	else if (ID > 162 && ID <= 165)
	{
		// Additional consumable
		Vector3f Temp = GameLogicManager::Get().front_powerup();
		PhysicsObj *TreatObj = NULL;
		Entity* pe = AssetManager::get().get_entity(ID);
		if(pe) TreatObj = pe->physics_obj();
		if (TreatObj)
		{
			m_consumables++;
			TreatObj->set_pos(Temp);
			GameLogicManager::Get().pop_and_add_powerup(Temp);
		}
	}
	else if ((ID > 20) && (ID <= 30))
	{
		// PLACEHOLDER FOR RACER-RACER COLLISIONS
	}
	else if (ID == 99)
	{
		// FALLING THROUGH HOLES 
		m_needs_reset = true;
	}
}

void Racer::reset_to_last_waypoint()
{
	PhysicsObj* SledObj = m_entity->physics_obj();
	if (SledObj)
	{
		SledObj->reset_velocities();
		SledObj->set_transform_matrix(m_resetTransform);

		//move the marker to the next waypoint because if it's still "2 waypoints" away, it may be too far
		if(m_nextWaypointObject){

			Entity* markerEntity = AssetManager::get().get_entity(5);
			if(markerEntity){
				PhysicsObj* po = markerEntity->physics_obj();
				if(po){
					po->set_pos(m_nextWaypointObject->get_pos());
				}
			}


		}

		TEXEngine::Core::LogManager::get().log_message("RESETTING position to (%f,%f,%f)",SledObj->get_pos().x,SledObj->get_pos().y,SledObj->get_pos().z);
	}
}