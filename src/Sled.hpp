#ifndef XSLEDDING_SLED_H_INCLUDED
#define XSLEDDING_SLED_H_INCLUDED

#include "asset/entity.hpp"

using TEXEngine::Asset::Entity;

class Sled
{
private:
	Entity entity;  
public:
	Sled() {}
	Sled(int id) {}
	~Sled() {}
	void init();
	void set_entity(int id);
	void apply_brake(float b);
	void reset_sled();
	void update(double deltaTime);
	float speed();
};

#endif 