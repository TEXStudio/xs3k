#include "util\nstring.hpp"
#include "Pack.hpp"
#include "GameLogicManager.hpp"

using TEXEngine::Util::NString::to_float;
using namespace std;

void Pack::update(double deltaTime)
{
	m_powerupTimer += deltaTime; 
	m_animals[0].update(deltaTime);
	m_animals[1].update(deltaTime);
	m_animals[2].update(deltaTime);
	m_animals[3].update(deltaTime);
	m_thrust = (m_animals[0].thrust() +
			m_animals[1].thrust() +
			m_animals[2].thrust() +
			m_animals[3].thrust()) * 0.25f; 

	if (m_powerupTimer < 5.0)
	{
		switch (m_activePowerup)
		{
		case 1:
			for (int i = 0; i < 4; i++)
			{
				m_animals[i].energy(1.0f);
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++)
			{
				m_animals[i].discipline(1.0f);
			}
			break;
		case 3:
			for (int i = 0; i < 4; i++)
			{
				m_animals[i].morale(1.0f);
			}
			break;
		default:
			break;
		}
	}

	// TEST CODE
	///////////////////////

	/*
	for (int i = 0; i < 4; i++)
	{
		m_animals[i].discipline(0.1f);
	}*/

}

void Pack::speed(float s)
{
	m_speed = s; 
	m_animals[0].speed(s);
	m_animals[1].speed(s);
	m_animals[2].speed(s);
	m_animals[3].speed(s);
}

float Pack::speed()
{
	return m_speed; 
}

Vector3f Pack::edm()
{
	Vector3f Temp; 
	Temp.x = (m_animals[0].edm().x + 
				m_animals[1].edm().x + 
				m_animals[2].edm().x + 
				m_animals[3].edm().x) * 0.25f;

	Temp.y = (m_animals[0].edm().y + 
				m_animals[1].edm().y + 
				m_animals[2].edm().y + 
				m_animals[3].edm().y) * 0.25f;
	
	Temp.z = (m_animals[0].edm().z + 
				m_animals[1].edm().z + 
				m_animals[2].edm().z + 
				m_animals[3].edm().z) * 0.25f;
	return Temp;
}

float Pack::thrust()
{
	if (m_powerupTimer < 5.0 && m_activePowerup == 4)
	{
		return 10.0f; 
	}
	else return m_thrust;
}

float Pack::deviation()
{
	return (m_animals[0].deviation() +
			m_animals[1].deviation() +
			m_animals[2].deviation() +
			m_animals[3].deviation()) * 0.25f; 
}

void Pack::apply_whip()
{
	m_animals[0].apply_whip();
	m_animals[1].apply_whip();
	m_animals[2].apply_whip();
	m_animals[3].apply_whip();
}

void Pack::apply_treat()
{
	m_animals[0].apply_treat();
	m_animals[1].apply_treat();
	m_animals[2].apply_treat();
	m_animals[3].apply_treat();
}

void Pack::reset_pack()
{
	m_animals[0].reset();
	m_animals[1].reset();
	m_animals[2].reset();
	m_animals[3].reset();
}

void Pack::apply_powerup(int type)
{
	m_activePowerup = type; 
	m_powerupTimer = 0.0; 
}

void Pack::init(const std::vector<std::string>& animals, const string& species, const bool IsHuman){
	
	if (animals.size() < 4)
		TEXEngine::Core::LogManager::get().log_error("Animal name parameter of invalid size!");
	else
	{
		m_species = species; 
		if (IsHuman) m_ncf.set_source(PATH_STUFF"data/scripts/ncf/AnimalList.ncf1"); 
		else m_ncf.set_source(PATH_STUFF"data/scripts/ncf/BotAnimalList.ncf1"); 
	
		int error = m_ncf.parse();
		if(!error){
			float mass = 1.0f; 
			NCF1 *species_group = m_ncf.group(m_species.c_str());
			if (species_group){
				for (int i = 0; i < 4; i++)	{
					NCF1 *individual = species_group->group(animals[i].c_str());
					if (individual)	{
						m_animals[i].name(animals[i]);
						m_animals[i].reset();
						m_animals[i].init_properties(mass, to_float(species_group->get("predictability")));
			
						m_animals[i].init_modifiers(	to_float(individual->get("E")),
														to_float(individual->get("D")),
														to_float(individual->get("M")),
														to_float(species_group->get("treat_modifier")), 
														to_float(species_group->get("whip_modifier")));
					}else
						TEXEngine::Core::LogManager::get().log_error("There's no animal called '%s' for species %s.",animals[i].c_str(),m_species.c_str());
				}	
			}else
				TEXEngine::Core::LogManager::get().log_error("There's no species called '%s' in file %s.",m_species.c_str(),m_ncf.get_source());
		}else
			TEXEngine::Core::LogManager::get().log_error("Failed to parse the file %s .",m_ncf.get_source());
	}
}