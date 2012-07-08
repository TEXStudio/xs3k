#include "Animal.hpp"
#include <algorithm>
#include <cmath>
#ifdef	_WIN32
	#include <cstdio>
#else	
	#include <stdio.h>
#endif	/* _WIN32 */
#include "util/ncf.hpp"
#include "log/logManager.hpp"
#include "MenuParams.h"

const float LARGE_MOD = 0.03f;
const float SMALL_MOD = 0.01f; 
const float MAX_SPEED = 8.0f; 


std::vector<std::vector<float> > Animal::m_weights(4,std::vector<float>(4,0));
std::vector<std::vector<float> > Animal::m_thresholds(4,std::vector<float>(3,0));
float	Animal::m_first_threshold_value = 0.0f;
float	Animal::m_second_threshold_value = 0.0f;
float	Animal::m_third_threshold_value = 0.0f;
bool Animal::m_initialised = false;


void Animal::load_weights_and_thresholds()
{
	if ( !m_initialised )
	{
		TEXEngine::Util::NCF1	config_file;
		std::string				source(PATH_STUFF "data/scripts/ncf/animalModifiers.ncf1");



		config_file.set_source(source.c_str());
		if ( !config_file.parse() )
		{
			if ( config_file.query_group("AnimalStatisticsConfiguration") )
			{
				TEXEngine::Util::NCF1*	animal_statistics = config_file.group("AnimalStatisticsConfiguration");



				if( sscanf(animal_statistics->get("first_threshold_value"),"%f",&m_first_threshold_value) == EOF )
					TEXEngine::Core::LogManager::get().log_error("Could not parse the first threshold value parameter");
				
				if( sscanf(animal_statistics->get("second_threshold_value"),"%f",&m_second_threshold_value) == EOF )
					TEXEngine::Core::LogManager::get().log_error("Could not parse the second threshold value parameter");

				if( sscanf(animal_statistics->get("third_threshold_value"),"%f",&m_third_threshold_value) == EOF )
					TEXEngine::Core::LogManager::get().log_error("Could not parse the third threshold value parameter");


				for ( unsigned int i = 0;  i < 4;  ++i )
				{
					std::string				group_name("");
					TEXEngine::Util::NCF1*	stat_group = NULL;



					if ( i == 0 )
						group_name = "energy";
					else if ( i == 1 )
						group_name = "morale";
					else if ( i == 2 )
						group_name = "discipline";
					else if ( i == 3 )
						group_name = "thrust";

					
					if ( animal_statistics->query_group(group_name.c_str()) )
					{
						TEXEngine::Util::NCF1*	stat_sub_group = NULL;
						float					temp = 0.0f;


						stat_group = animal_statistics->group(group_name.c_str());

						if ( stat_group->query_group("weights") )
						{
							stat_sub_group = stat_group->group("weights");

							if( sscanf(stat_sub_group->get("energy"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the energy weight for %s ",group_name.c_str());
							else
								m_weights[i][0] = temp;

							if( sscanf(stat_sub_group->get("morale"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the morale weight for %s ",group_name.c_str());
							else
								m_weights[i][1] = temp;

							if( sscanf(stat_sub_group->get("discipline"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the discipline weight for %s ",group_name.c_str());
							else
								m_weights[i][2] = temp;

							if( sscanf(stat_sub_group->get("thrust"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the thrust weight for %s ",group_name.c_str());
							else
								m_weights[i][3] = temp;
						}
						else
							TEXEngine::Core::LogManager::get().log_error("Weights for %s could not be loaded!",group_name.c_str());


						if ( stat_group->query_group("thresholds") )
						{
							stat_sub_group = stat_group->group("thresholds");

							if( sscanf(stat_sub_group->get("first"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the first thershold for %s ",group_name.c_str());
							else
								m_thresholds[i][0] = temp;

							if( sscanf(stat_sub_group->get("second"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the second threshold for %s ",group_name.c_str());
							else
								m_thresholds[i][1] = temp;

							if( sscanf(stat_sub_group->get("third"),"%f",&temp) == EOF )
								TEXEngine::Core::LogManager::get().log_error("Could not parse the third threshold for %s ",group_name.c_str());
							else
								m_thresholds[i][2] = temp;
						}
						else
							TEXEngine::Core::LogManager::get().log_error("Thresholds for %s could not be loaded!",group_name.c_str());
					}
					else
						TEXEngine::Core::LogManager::get().log_error("Animal statistic weightings and thresholds for %s was not found!!",group_name.c_str());
				}


				m_initialised = true;
			}
			else
				TEXEngine::Core::LogManager::get().log_error("Animal statistic weightings and thresholds group was not found!");
		}
		else
			TEXEngine::Core::LogManager::get().log_error("Animal statistic weightings and thresholds file could not be loaded!");		
	}
}

float Animal::progress_function( const float stat , const float first_threshold , const float second_threshold , const float third_threshold )
{
	float	position = stat;
	float	temp = position;
	float	return_value = m_first_threshold_value;



	if ( position >= third_threshold )
	{
		position -= third_threshold;
		return_value = m_second_threshold_value + std::min(std::exp(position),m_third_threshold_value);
	}
	else if ( position >= second_threshold )
	{
		return_value = m_second_threshold_value;
	}
	else if ( position >= first_threshold )
	{
		float	value_difference = m_second_threshold_value - m_first_threshold_value;
		float	difference = second_threshold - first_threshold;

		position -= first_threshold;
		return_value = m_first_threshold_value + position*value_difference/difference;
	}
	

	return return_value;
};


Animal::Animal() : m_energy(1.0f), m_discipline(1.0f), m_morale(1.0f), m_thrust(0.0f), m_speed (0.0f),
	m_eMod(1.0f), m_dMod(1.0f), m_mMod(1.0f), m_tMod(1.0f), m_wMod(1.0f), 
	m_mass(1.0f), m_deviation(0.0f), m_predictability(1.0f), m_totalTime(0.0),  
	m_name("default")
{
	load_weights_and_thresholds();
}

Animal::~Animal(){}

void Animal::init_modifiers(float e, float d, float m, float t, float w)
{
	m_eMod = e; 
	m_dMod = d;
	m_mMod = m;
	m_tMod = t; 
	m_wMod = w;
}

void Animal::init_properties(float m, float p)
{
	m_mass = m; 
	m_predictability = p; 
}

void Animal::name(std::string n)
{
	m_name = n; 
}

void Animal::energy(float e)
{
	m_energy = e;
}
void Animal::discipline(float d)
{
	m_discipline = d; 
}
void Animal::morale(float m)
{
	m_morale = m;
}
void Animal::thrust(float t)
{
	m_thrust = t;
}

void Animal::deviation(float d)
{
	m_deviation = d; 
}

void Animal::adjust_modifiers(float e, float d, float m, float t, float w)
{
	m_eMod += e; 
	m_dMod += d;
	m_mMod += m;
	m_tMod += t; 
	m_wMod += w;
}

void Animal::update(double deltaTime)
{
	m_totalTime += deltaTime; 

	if (m_speed > 2.0f)
	{
		m_energy = std::max(0.1f, (float)(m_energy - m_thrust * m_thrust * 1.4f * LARGE_MOD * deltaTime));
		m_energy = std::min(1.0f, (float)(m_energy + m_morale * LARGE_MOD * m_eMod * deltaTime));

		m_discipline = std::max(0.1f, (float)(m_discipline - m_energy * LARGE_MOD * deltaTime ));
		m_discipline = std::max(0.1f, (float)(m_discipline - m_thrust * SMALL_MOD * deltaTime ));

		m_morale = std::max(0.1f, (float)(m_morale - m_discipline * SMALL_MOD * deltaTime )); 
		//m_morale = min(1.0f, (float)(m_morale + m_speed * SMALL_MOD * m_mMod * deltaTime ));

		m_thrust = 0.3f + m_energy * 0.5f + m_morale * 0.2f; 
	}
	else m_thrust = std::max(0.0f, m_thrust - (float)deltaTime);


	m_deviation = (float)(sin(m_totalTime + sin(m_totalTime * m_predictability) * m_predictability) * (1.0f - std::min(1.0f, m_discipline * 1.3f)));
}

void Animal::speed(float s)
{
	m_speed = s;
}

void Animal::apply_whip()
{
	if (m_speed > 2.0f)
	{
		m_discipline = std::min(m_discipline + 0.3f * m_wMod * m_dMod, 1.0f);
		m_morale = std::max(m_morale - 0.08f * m_wMod, 0.0f);
	}
	else 
	{
		m_thrust = 10.0f; 
	}

}

void Animal::apply_treat()
{
	if (m_speed > 0.01f)
	{
		m_morale = std::min(m_morale + 0.7f * m_tMod * m_mMod, 1.0f); 
		m_energy = std::min(m_energy + 0.4f * m_tMod * m_eMod, 1.0f); 
	}
	else
	{
		adjust_modifiers(0.05f, 0.0f, 0.1f, 0.05f, 0.05f);
	}
}

void Animal::reset()
{
	m_energy = 1.0f;
	m_discipline = 1.0f; 
	m_morale = 1.0f; 
	m_thrust = 0.0f;
	m_deviation = 0.0f; 
	m_totalTime = 0.0; 
	m_speed = 0.0f;
}

Vector3f Animal::edm()
{
	Vector3f Temp; 
	Temp.x = m_energy; 
	Temp.y = m_discipline;
	Temp.z = m_morale; 
	return Temp; 
}

