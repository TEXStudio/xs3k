#ifndef XSLEDDING_ANIMAL_HPP_INCLUDED
#define XSLEDDING_ANIMAL_HPP_INCLUDED
#include <string>
#include <vector>
#include "math/vector.hpp"

using TEXEngine::Math::Vector3f;

class Animal {
private:
	static std::vector<std::vector<float> > m_weights;
	static std::vector<std::vector<float> > m_thresholds;
	static float m_first_threshold_value;
	static float m_second_threshold_value;
	static float m_third_threshold_value;
	static bool m_initialised;

	float m_energy, m_discipline, m_morale, m_thrust, m_deviation, m_speed;
	float m_eMod, m_dMod, m_mMod, m_tMod, m_wMod;
	float m_mass, m_predictability; 
	double m_totalTime; 
	std::string m_name;

	static void load_weights_and_thresholds();
	static float progress_function( float stat , float first_threshold , float second_threshold , float third_threshold );
public:

	Animal();
	~Animal();

	// Initialization
	void init_modifiers(float e, float d, float m, float t, float w);
	void init_properties(float m, float p);
	void name(std::string n);

	// Manual Adjustment e.g. powerups

	void energy(float e);
	void discipline(float d);
	void morale(float m);
	void thrust(float t);
	void deviation(float d); 
	void adjust_modifiers(float e, float d, float m, float t, float w);

	// Running
	std::string name() { return m_name; } 
	float mass() { return m_mass; } 
	float predictability() { return m_predictability; } 

	float deviation() { return m_deviation; } 
	float thrust() { return m_thrust; } 
	Vector3f edm(); 

 
	void speed(float s);
	void update(double deltaTime);
	void apply_whip();
	void apply_treat(); 

	void reset();
};

#endif