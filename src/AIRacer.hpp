#ifndef XSLEDDING_AIRACER_HPP_INCLUDED
#define XSLEDDING_AIRACER_HPP_INCLUDED

#include "Racer.hpp"

class AIRacer : public Racer
{
private:
	bool m_is_easy;
public:
	AIRacer() : m_is_easy(true) {}
	~AIRacer() {} 	
	void apply_controls(double deltaTime);
	void update(double deltaTime);
	void set_easy(bool value);
};

#endif