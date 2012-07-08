#ifndef XSLEDDING_PACK_HPP_INCLUDED
#define XSLEDDING_PACK_HPP_INCLUDED

#include "Animal.hpp" 
#include <string>
#include <vector>

#include "util/ncf.hpp"

using TEXEngine::Util::NCF1;

class Pack
{
private:
	Animal m_animals[4];
	NCF1 m_ncf; 
	//Entity m_entity; 
	float m_thrust, m_torque, m_torqueModifier, m_speed; 
	double m_powerupTimer;
	int m_activePowerup; 
	std::string m_species;
public: 
	Pack() : m_thrust(0.0f), m_torque (0.0f), m_torqueModifier (1.0f), m_speed(0.0f),  m_powerupTimer(0.0), m_activePowerup (-1) {}
	explicit Pack(int ID) : m_thrust(0.0f), m_torque (0.0f), m_torqueModifier (1.0f), m_speed(0.0f), 
							m_powerupTimer(0.0), m_activePowerup (-1) {}
	~Pack() {};
	//void set_entity(int id); 

	void init(const std::vector<std::string>& animals, const std::string& species, const bool IsHuman);
	void update(double deltaTime);
	void speed(float s); 
	float speed(); 

	Vector3f edm(); 
	float thrust(); 
	float deviation(); 

	void apply_powerup(int type); 
	void apply_torque(float t); 
	void apply_whip();
	void apply_treat(); 
	void reset_pack();
	//void set_position(Vector3f p);  
};

#endif