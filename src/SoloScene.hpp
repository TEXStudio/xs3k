#ifndef		XSLEDDING_SOLOSCENE_HPP_INCLUDED
	#define XSLEDDING_SOLOSCENE_HPP_INCLUDED

	#include "LogicScene.hpp"
	#include "asset/shader.hpp"
	#include "asset/texture.hpp"
	#include "asset/shaderManager.hpp"

	class GameLogicManager;

	class	SoloScene	:	public LogicScene
	{

		private:
			
			KeyboardHotkey camera_up;
			KeyboardHotkey camera_down;
			KeyboardHotkey camera_left;
			KeyboardHotkey camera_right;
			KeyboardHotkey camera_tiltup;
			KeyboardHotkey camera_tiltdown;
			KeyboardHotkey camera_tiltleft;
			KeyboardHotkey camera_tiltright;


			Timer				m_goTimer;
			std::string			m_lapRecordName;
			double				m_lapRecord; 


			void				update_waypointmarker(const double d);
			void				update_powerupsRotation(const double d);
			virtual void		calculate_and_send_position();

	protected:
			highscore_struct	top3_scores[3];
			bool				m_hasSentScores;

		public: 
			
			SoloScene( const unsigned long id = 0 );
			~SoloScene();


			void				set_record( const double record );
			void				write_record();
			void				start_go_timer();
			void				resume_go_timer();
			void				stop_go_timer();
			
			double				go_timer_value();
			double				get_lap_record();


			void				on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters );

			bool				init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys );
			void				update();
			void				pause();
			void				unpause(); 
			void				endgame( bool finished );
			void				reset(); 
			void				cleanup();
			bool				setup_scene( const unsigned int id = 1 , const std::string& name = std::string("") );
			void				enable_player_keys();
			void				disable_player_keys();
	};


	/* 
		Function implementations
	*/


	inline void	SoloScene::calculate_and_send_position()
	{
		TEXEngine::Core::Message	parameters(3,0,0);


		parameters.add_integral_data(0,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(get_id()));
		parameters.add_integral_data(1,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(0));
		parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(1));
		trigger_event(PLAYER_POSITION,parameters);
	}

	inline void	SoloScene::set_record( const double record )	{ m_lapRecord = record; };
	inline void	SoloScene::start_go_timer()						{ m_goTimer.start(); };
	inline void	SoloScene::resume_go_timer()					{ m_goTimer.resume(); };
	inline void	SoloScene::stop_go_timer()						{ m_goTimer.stop(); };

	inline double	SoloScene::go_timer_value()					{ return m_goTimer.get_time_in_sec(); };
	inline double	SoloScene::get_lap_record()					{ return m_lapRecord; };


	inline void	SoloScene::enable_player_keys()
	{ 
		enable_play_keys(); 
		
		if ( m_player )
			m_player->enable_controls();
	};

	inline void	SoloScene::disable_player_keys()
	{
		disable_play_keys();
		
		if ( m_player )
			m_player->disable_controls();
	};



#endif		/* XSLEDDING_SOLOSCENE_HPP_INCLUDED */