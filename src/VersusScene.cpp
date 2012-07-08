#include <sstream>
#include "MenuParams.h"
#include "VersusScene.hpp"
#include "scene/sceneManager.hpp"
#include "scene/scene.hpp"
#include "physics/PhysicsManager.hpp"
#include "physics/PhysicsObj.hpp"
#include "GameLogicManager.hpp"



unsigned int VersusScene::sort_finished_vector()
{
	unsigned int	return_value =  1;
	unsigned int	count  = m_number_of_bots + 1;
	unsigned int	finished = min(count,m_finished_racer_count);


	for ( unsigned int i = 0;  i < finished;  ++i )
	{
		if ( !m_finished_vector_lock[i]  &&  m_finished_racers[i] )
		{
			for ( unsigned int j = i+1;  j < count;  ++j )
			{
				Racer*	temp = m_finished_racers[i];



				if ( m_finished_racers[j]  &&  m_finished_racers[i]->race_progress() < m_finished_racers[j]->race_progress() )
				{
					m_finished_racers[i] = m_finished_racers[j];
					m_finished_racers[j] = temp;
				}
			}
		}

		if ( m_finished_racers[i] != get_player() )
			++return_value;
	}


	return return_value;
};

unsigned int	VersusScene::calculate_position()
{
	unsigned int	position = 1;



	if ( m_player )
	{
		for ( unsigned int i = 0;  i < m_number_of_bots; ++i )
		{
			if ( m_bots[i].is_finished() )
				++position;
			else if ( m_bots[i].race_progress() > m_player->race_progress() )
					++position;
		}
	}


	return position;
}

void	VersusScene::calculate_and_send_position()
{
	if ( m_player )
	{
		TEXEngine::Core::Message	parameters(3,0,0);

		parameters.add_integral_data(0,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(get_id()));
		parameters.add_integral_data(1,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(0));
		parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(calculate_position()));
		trigger_event(PLAYER_POSITION,parameters);
	}
}

VersusScene::VersusScene( const unsigned long id )	:	
	SoloScene(id) , m_finished_racers(m_number_of_bots+1) , m_finished_vector_lock(m_number_of_bots+1,false) , m_finished_racer_count(0)
{
};

bool VersusScene::setup_scene( const unsigned int id  , const std::string& name )
{
	bool	return_value = false;
	
	
	if ( SoloScene::setup_scene(id,name) )
	{
		TEXEngine::Asset::Scene* scene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
		if (scene){
			scene->attach(24);
			scene->attach(23);

			TEXEngine::Core::PhysicsManager::get().register_listener(24,&m_bots[0]);

			return_value = true;
		}
	}


	return return_value;
}

VersusScene::~VersusScene()
{
};

bool	VersusScene::init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys )
{
	bool	return_value = false;



	if ( SoloScene::init(params,scene_keys,player_keys) ) // consider having predetermined aiPack identities and do a loop for this
	{
		TEXEngine::Asset::Scene* scene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
		if(scene)
		{
			if ( m_player  &&  m_player->m_entity )
			{
				m_player->max_laps(3);

				// setting player position to startpoint slightly to the side
				PhysicsObj* sledPO = m_player->m_entity->physics_obj();

				if(sledPO)
				{
					Vector3f v = sledPO->get_pos();
					v.z -= 5.0f;
					sledPO->set_pos(v);
					sledPO->reset_velocities();
					m_player->m_resetTransform = sledPO->get_transform_matrix();
				}
			}

			std::vector<std::string> AnimalNames(4,params[8]);
			std::stringstream	buffer;
			for ( unsigned int i = 0;  i < m_number_of_bots;  ++i)
			{
				
				std::string	name("");	

				if(!params[3].compare(SPECIES_PROPERTY_DOG))
				{
					scene->attach(20 + i*5);
					m_bots[i].set_entityPack(20 + i*5);
				}
				else if(!params[3].compare(SPECIES_PROPERTY_MOUSE))
				{
					scene->attach(21 + i*5);
					m_bots[i].set_entityPack(21 + i*5);
				}
				else if(!params[3].compare(SPECIES_PROPERTY_DINO))
				{
					scene->attach(22 + i*5);
					m_bots[i].set_entityPack(22 + i*5);
				}
				else TEXEngine::Core::LogManager::get().log_error("can't add a pack entity: unrecognized species");


				buffer.clear();
				buffer << "Bot" << (i+1);
				buffer >> name;
				m_bots[i].set_entity(24 + i*5);
				m_bots[i].set_entitySled(23 + i*5);
				m_bots[i].init(3,5,name,AnimalNames,params[3],i+2,false);
				if(!params[8].compare(DIFFICULTY_PROPERTY_EASY))
					m_bots[i].set_easy(true);
				else if(!params[8].compare(DIFFICULTY_PROPERTY_HARD))
					m_bots[i].set_easy(false);


				// setting bot position to startpoint slightly to the side
				if ( m_bots[i].m_entity )
				{
					PhysicsObj* aiSledPO = m_bots[i].m_entity->physics_obj();

					if ( m_player &&  m_player->m_entity )
					{
						PhysicsObj* playerSled = m_player->m_entity->physics_obj();
						if(aiSledPO && playerSled)
						{
							Matrix4x4f	tranfs(playerSled->get_transform_matrix());
							Vector3f v(playerSled->get_pos());

							
							v.z += 10.0f;

							aiSledPO->set_transform_matrix(tranfs);
							aiSledPO->set_pos(v);
							aiSledPO->reset_velocities();
							m_bots[i].m_resetTransform = aiSledPO->get_transform_matrix();
						}
					}
				}

				m_bots[i].m_pack.reset_pack();
				m_bots[i].set_state(StateEnum_READY);
			}

			m_finished_racer_count = 0;
			for ( unsigned int i = 0;  i < m_number_of_bots;  ++i)
			{
				m_finished_racers[i] = NULL;
				m_finished_vector_lock[i] = false;
			}

			TEXEngine::Core::SceneManager::get().activate(get_scene_id());

			// PLAY AUDIO CLIP "READY?"
			start_go_timer();
			state(SceneStateEnum_RUNNING);
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tScene setup finished.");
			return_value = true;
		}
		else TEXEngine::Core::LogManager::get().log_error(" scene not found");
	}


	return return_value;
};

void	VersusScene::update()
{
	if (state() == SceneStateEnum_RUNNING )
	{
		if ( m_bots[0].state() == StateEnum_READY)
		{
			if ( go_timer_value() > 3.0)
			{
				for ( unsigned int i = 0;  i < m_number_of_bots;  ++i)
				{
					m_bots[i].set_state(StateEnum_RACING);
				}
			}
		}
		else
		{
			for ( unsigned int i = 0;  i < m_number_of_bots;  ++i )
			{
				if ( !m_bots[i].is_finished() )
				{
					m_bots[i].update(time_difference());

					if ( m_bots[i].is_finished() )
					{
						m_bots[i].set_state(StateEnum_FINISHED);
						m_finished_racers[m_finished_racer_count++] = &m_bots[i];

						if (  m_player == NULL ||  ( m_player  && !m_player->is_finished() ) )
							m_finished_vector_lock[m_finished_racer_count-1] = true;
					}
				}
			}
		}
	}


	SoloScene::update();


	if ( m_player  &&  m_player->state() != StateEnum_FINISHED  &&  m_player->is_finished() )
	{
		m_player->set_state(StateEnum_FINISHED);
		m_finished_racers[m_finished_racer_count++] = m_player;
		endgame(true);
	}
};

void	VersusScene::endgame( bool finished )
{
	TEXEngine::Core::Message msg(0, 3, 3);
	for(unsigned int i = 0; i < 3; ++i){
		msg.add_real_data(i, top3_scores[i].time);
		msg.add_pointer(i, &top3_scores[i].name);
	}
	TEXEngine::Core::trigger_event(SEND_SCORES, msg);



	TEXEngine::Core::Message	parameters(4,2,0);



	parameters.add_integral_data(0,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(get_id()));
	parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>((finished ? 1 : 0 )));

	if ( finished )
		parameters.add_integral_data(3,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(sort_finished_vector()));
	else
		parameters.add_integral_data(3,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(calculate_position()));

	if ( m_player )
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(m_player->best_lap()));
	else
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(0.0f));
	parameters.add_real_data(1,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(get_lap_record()));



	state(SceneStateEnum_FINISHED);
	
	disable_player_keys();
	GameLogicManager::Get().enable_menu_keys();

	if ( m_player )
		m_player->set_state(StateEnum_FINISHED);


	TEXEngine::Core::LogManager::get().log_message("Game Log:\tGame finished, switching to the finished screen");
	TEXEngine::Core::trigger_event(SCENE_FINISHED,parameters);
};

void	VersusScene::reset()
{
	SoloScene::reset();
};

void	VersusScene::cleanup()
{
	SoloScene::cleanup();

	TEXEngine::Asset::Scene* scene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
	if(scene)
	{
		for ( unsigned int i = 0;  i < m_number_of_bots;  ++i)
		{
			scene->detach(20 + i*5);
			scene->detach(21 + i*5);
			scene->detach(22 + i*5);
		}
	}
	else
		TEXEngine::Core::LogManager::get().log_warning("Won't detach. Reason: The scene was not found!");

};
