#ifndef		XSLEDDING_VERSUSSCENE_HPP_INCLUDED
	#define	XSLEDDING_VERSUSSCENE_HPP_INCLUDED

	#include "SoloScene.hpp"
	#include "HumanRacer.hpp" 
	#include "AIRacer.hpp" 



	class VersusScene : public SoloScene
	{
		private: 
	
			static const unsigned int	m_number_of_bots = 1;

			AIRacer						m_bots[m_number_of_bots];
			std::vector<Racer*>			m_finished_racers;
			std::vector<bool>			m_finished_vector_lock;
			unsigned int				m_finished_racer_count;
	

			unsigned int				sort_finished_vector();
			unsigned int				calculate_position();
			void						calculate_and_send_position();

		public:
	
			VersusScene( const unsigned long id = 0 );
			~VersusScene();

			bool						init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys );
			void						update();
			void						endgame( bool finished );
			void						reset();
			void						cleanup(); 
			bool						setup_scene( const unsigned int id = 2  , const std::string& name = std::string("") );
	};



	/* 
		Function implementations
	*/




#endif		/* XSLEDDING_VERSUSSCENE_HPP_INCLUDED */