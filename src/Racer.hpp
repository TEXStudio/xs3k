#ifndef XSLEDDING_RACER_HPP_INCLUDED
#define XSLEDDING_RACER_HPP_INCLUDED
#include <string> 
#include <vector>
#include <queue>
#include "asset\entity.hpp" 
#include "math\vector.hpp"
#include "physics\CollisionListener.hpp"
#include "physics\PhysicsObj.hpp"

#include "Pack.hpp" 
#include "Animal.hpp"

static const float THRUST_MODIFIER = 800.0f;
static const float STEER_MODIFIER = 200.0f;
static const float TORQUE_MODIFIER = 35.0f;
static const float BASE_FRICTION = 5.0f;
static const float FRICTION_MODIFIER = 10.0f;




enum StateEnum { 
	StateEnum_READY, 
	StateEnum_RACING, 
	StateEnum_FINISHED 
};

using TEXEngine::Asset::Entity;
using TEXEngine::Physics::CollisionListener;
using TEXEngine::Physics::PhysicsObj;

class Racer : public CollisionListener
{
private: 

public:
	Entity *m_entity; 
	Entity *m_entityPack;
	Entity *m_entitySled;
	PhysicsObj *m_lastWaypointObject; 
	PhysicsObj *m_nextWaypointObject;
	Vector3f m_waypointDirection; // Needs fully implementing
	Matrix4x4f m_packTransform;
	Matrix4x4f m_resetTransform;
	Pack m_pack;
	std::string m_name; 
	int m_consumables, m_lapNumber, m_maxlaps , m_lastWaypoint, m_placing;
	float m_steerValue, m_thrustValue, m_brakeValue, m_handBrake;
	double m_bestLap, m_currentLap; 
	StateEnum m_state;
	float m_steer; // for testing
	bool m_needs_reset;
	

	Racer() : m_name("default"), m_consumables(5), m_lapNumber(1), m_maxlaps(0) , m_lastWaypoint(0), 
									m_bestLap(9999), m_currentLap(0), m_placing (9), 
									m_entity(NULL), m_entityPack(NULL), m_entitySled(NULL),
									m_steerValue (0.0f), m_thrustValue(0.0f), m_brakeValue(0.0f), m_handBrake (0.0f),
									m_steer(0.0f), m_needs_reset(false) {}
	virtual ~Racer() {}  
	void init(const int laps , const int consumables, const std::string& name, const std::vector<std::string>& animals, const std::string& species, const int playernumber, const bool IsHuman);
	void set_entity(int id);
	void set_entityPack(int id);
	void set_entitySled(int id);
	void set_position(Vector3f p); 
	void set_orientation(Vector3f o);
	void set_state(StateEnum state); 
	void set_lap(int lapNumber); 
	void reset_to_last_waypoint();

	void update_meshes();
	
	virtual void apply_controls(double deltaTime) = 0;
	void react_to(unsigned int ent_id); 
	virtual void update(double deltaTime) = 0; 

	void inc_laps();
	void inc_wpts();
	void placing(int p);

	// UI Getters
	int lap_number() { return m_lapNumber; }
	int max_laps() { return m_maxlaps; }
	void max_laps( const unsigned int laps ) { m_maxlaps = laps; }
	int consumables() { return m_consumables; } 
	void consumables(const int c){m_consumables = c;}
	std::string name() { return m_name; } 
	double current_lap() { return m_currentLap; } 
	double best_lap() 
	{ 
		if (m_lapNumber > 1) return m_bestLap; 
		else return m_currentLap; 
	} 
	StateEnum state() { return m_state; } 
	int placing() { return m_placing; } 

	// Do something here cause it was not implemented and was causing errors
	float speed() { return 0.0f; };
	Vector3f edm();
	Vector3f pos();
	double race_progress() const;
	double lap_progress() const;
	bool is_finished() const;
};

#endif 