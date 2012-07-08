#ifndef		XSLEDDING_LOGICSCENE_HPP_INCLUDED
	#define XSLEDDING_LOGICSCENE_HPP_INCLUDED

	#include	<string>
	#include	"message/object.hpp"
	#include	"message/message.hpp"
	#include	"message/TEXEngineInterface.hpp"
	#include	"asset/assetManager.hpp"
	#include	"port/Timer.h"
	#include	"util/ncf.hpp"
	#include	"log/logmanager.hpp"
	#include	"HumanRacer.hpp"
	#include	"keyboardHotkey.hpp"
	#include	"controllerHotkey.hpp"
	#include	"GameEventCodes.h"

	using		TEXEngine::Port::Timer;
	using		TEXEngine::Util::NCF1; 
	using		namespace X3000;



	class	LogicScene	:	public TEXEngine::Core::Object
	{

		protected:

			static	HumanRacer*	m_player;


			NCF1				m_config;
			void				set_scene_id( const unsigned int id );
			unsigned int		get_scene_id();

		private:


			#ifdef	_WIN32
				KeyboardHotkey	m_pauseRaceKey;
				KeyboardHotkey	m_next_camera_key;
				KeyboardHotkey	m_previous_camera_key;
				KeyboardHotkey	m_rearview_camera_key;
				KeyboardHotkey	m_reset_position_key;
			#endif	/* _WIN32 */
			
			ControllerHotkey	m_cpauseRaceKey;
			ControllerHotkey	m_cnext_camera_key;
			ControllerHotkey	m_cprevious_camera_key;
			ControllerHotkey	m_crearview_camera_key;
			ControllerHotkey	m_creset_position_key;
			Timer				m_gameTimer; 
			std::string			m_source; 
			double				m_deltaTime; //seconds
			double				m_previousTime; //seconds
			unsigned long		m_update_rate;
			unsigned int		m_state;
			unsigned int		m_scene_id;

		
		public:
	
			enum SceneStateEnum { 
				SceneStateEnum_RESET = 0, 
				SceneStateEnum_RUNNING, 
				SceneStateEnum_PAUSED, 
				SceneStateEnum_FINISHED, 
				SceneStateEnum_QUIT 
			};

			struct highscore_struct{
				highscore_struct():time(9999.0){}
				std::string name;
				double time;
			};

			enum SceneUpdateRateEnum {
				SCENE_UPDATE_DEFAULT = 0, 
				SCENE_UPDATE_FAST , 
				SCENE_UPDATE_MEDIUM ,
				SCENE_UPDATE_SLOW
			};

			static void			create_player( const unsigned int id );
			static void			destroy_player();
			static HumanRacer*	get_player();


			LogicScene( const unsigned long id = 0 );
			virtual ~LogicScene();


			void				source( const std::string& source );
			void				state(const unsigned int s);
			void				start_updating();
			void				stop_updating();
			void				update_rate( const unsigned int rate );
			int					load();
			void				field(const std::string& name, const std::string& value);
			void				write();
			void				start_timer();
			void				stop_timer();
			void				resume_timer();
			void				update_time();
			
			#ifdef	_WIN32
				void				keyboard_pause_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event );
				void				keyboard_next_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event );
				void				keyboard_previous_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event );
				void				keyboard_rearview_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event );
				void				keyboard_reset_position_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event );
			#endif	/* _IN32 */
			
			void				controller_pause_hotkey( const unsigned int key , const unsigned long down_event );
			void				controller_next_camera_hotkey( const unsigned int key , const unsigned long down_event );
			void				controller_previous_camera_hotkey( const unsigned int key , const unsigned long down_event );
			void				controller_rearview_camera_hotkey( const unsigned int key , const unsigned long down_event );
			void				controller_reset_position_hotkey( const unsigned int key , const unsigned long down_event );
			void				enable_keys();
			void				enable_play_keys();
			void				disable_keys();
			void				disable_play_keys();
		
			unsigned int		state() const;
			unsigned long		update_rate() const;
			const char*			field(const std::string& name );
			double				time_difference() const;
			bool				is_rear_down() const;


			virtual bool		init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys ) = 0;
			virtual void		update() = 0;
			virtual void		pause() = 0;
			virtual void		unpause() = 0; 
			virtual void		endgame( bool finished ) = 0;
			virtual void		reset() = 0; 
			virtual void		cleanup() = 0; 
			virtual bool		setup_scene( const unsigned int id = 1 , const std::string& name = std::string("") ) = 0;
			virtual void		enable_player_keys() = 0;
			virtual void		disable_player_keys() = 0;
	};


	/*
		Function implementations
	*/


	inline void			LogicScene::create_player( const unsigned int id )	{ m_player = new (std::nothrow) HumanRacer(id); };
	inline void			LogicScene::destroy_player()
	{
		if ( m_player )
		{
			delete m_player;
			m_player = NULL;
		}
	};

	inline HumanRacer*	LogicScene::get_player()							{ return m_player; };
	inline void			LogicScene::set_scene_id( const unsigned int id )	{ m_scene_id = id; };
	inline unsigned int	LogicScene::get_scene_id()							{ return m_scene_id; };


	inline void LogicScene::source( const std::string& source )				{ m_source = source; };
	inline void	LogicScene::state( const unsigned int s )					{ m_state = s; };
	inline void	LogicScene::start_updating()								{ register_event(m_update_rate); };
	inline void	LogicScene::stop_updating()									{ unregister_event(m_update_rate); };
	inline void	LogicScene::update_rate( const unsigned int rate )
	{ 
		if ( rate >= DEFAULT  &&  rate <= SLOW )
		{
			unregister_event(m_update_rate);
			m_update_rate = TEXEngine::Core::UPDATE + rate;
			register_event(m_update_rate);
		}
	};

	inline int LogicScene::load()
	{
		int	return_value = 1;



		if ( m_source != "" )
		{
			m_config.set_source(m_source.c_str());
			return_value = m_config.parse();

			if ( return_value ){
				TEXEngine::Core::LogManager::get().log_error("Failed to parse file %s.",m_source.c_str());
			}else{
				TEXEngine::Core::LogManager::get().log_message("Loading entity from %s.",m_source.c_str());
				return_value = TEXEngine::Core::AssetManager::get().load(&m_config);
				if(return_value){
					TEXEngine::Core::LogManager::get().log_error("Failed to load assets from file %s.",m_source.c_str());
				}
			}
		}


		return return_value;
	};

	inline void	LogicScene::field(const std::string& name , const std::string& value)
	{
		if ( m_source != "" )
			m_config.set(name.c_str(),value.c_str());
	};

	inline void	LogicScene::write()
	{
		if ( m_source != "" ){
			//TEXEngine::Core::LogManager::get().log_warning("LogicScene::write() temporarily disabled!");
			m_config.dump(m_source.c_str());
		}
	};

	inline void	LogicScene::start_timer()									{ m_previousTime = 0.0; m_gameTimer.start(); };
	inline void	LogicScene::stop_timer()									{ m_gameTimer.stop(); };
	inline void	LogicScene::resume_timer()									{ m_gameTimer.resume(); };
	inline void	LogicScene::update_time()
	{
		double current_time = m_gameTimer.get_time_in_sec();

		m_deltaTime = current_time - m_previousTime;
		m_previousTime = current_time;
	};

	#ifdef	_WIN32
		inline void LogicScene::keyboard_pause_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event )
		{
			m_pauseRaceKey.keybind(key);
			m_pauseRaceKey.modifier(modifier);
			m_pauseRaceKey.down_event(down_event);
			m_pauseRaceKey.disable();
		};

		inline void LogicScene::keyboard_next_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event )
		{
			m_next_camera_key.keybind(key);
			m_next_camera_key.modifier(modifier);
			m_next_camera_key.down_event(down_event);
			m_next_camera_key.disable();
		};

		inline void LogicScene::keyboard_previous_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event )
		{
			m_previous_camera_key.keybind(key);
			m_previous_camera_key.modifier(modifier);
			m_previous_camera_key.down_event(down_event);
			m_previous_camera_key.disable();
		};

		inline void LogicScene::keyboard_rearview_camera_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event )
		{
			m_rearview_camera_key.keybind(key);
			m_rearview_camera_key.modifier(modifier);
			m_rearview_camera_key.down_event(down_event);
			m_rearview_camera_key.disable();
		};

		inline void LogicScene::keyboard_reset_position_hotkey( const unsigned int key , const unsigned int modifier , const unsigned long down_event )
		{
			m_reset_position_key.keybind(key);
			m_reset_position_key.modifier(modifier);
			m_reset_position_key.down_event(down_event);
			m_reset_position_key.disable();
		};
	#endif	/* _WIN32 */

	inline void LogicScene::controller_pause_hotkey( const unsigned int key , const unsigned long down_event )
	{
		m_cpauseRaceKey.keybind(key);
		m_cpauseRaceKey.down_event(down_event);
		m_cpauseRaceKey.disable();
	};

	inline void LogicScene::controller_next_camera_hotkey( const unsigned int key , const unsigned long down_event )
	{
		m_cnext_camera_key.keybind(key);
		m_cnext_camera_key.down_event(down_event);
		m_cnext_camera_key.disable();
	};

	inline void LogicScene::controller_previous_camera_hotkey( const unsigned int key , const unsigned long down_event )
	{
		m_cprevious_camera_key.keybind(key);
		m_cprevious_camera_key.down_event(down_event);
		m_cprevious_camera_key.disable();
	};

	inline void LogicScene::controller_rearview_camera_hotkey( const unsigned int key , const unsigned long down_event )
	{
		m_crearview_camera_key.keybind(key);
		m_crearview_camera_key.down_event(down_event);
		m_crearview_camera_key.disable();
	};

	inline void LogicScene::controller_reset_position_hotkey( const unsigned int key , const unsigned long down_event )
	{
		m_creset_position_key.keybind(key);
		m_creset_position_key.down_event(down_event);
		m_creset_position_key.disable();
	};

	inline void LogicScene::enable_keys()
	{
		#ifdef	_WIN32
			m_pauseRaceKey.enable();
		#endif	/* _WIN32 */

		m_cpauseRaceKey.enable();
		enable_play_keys();
	};

	inline void LogicScene::disable_keys()
	{
		#ifdef	_WIN32
			m_pauseRaceKey.disable();
		#endif	/* _WIN32 */

		m_cpauseRaceKey.disable();
		disable_play_keys();
	};

	inline void LogicScene::enable_play_keys()
	{
		#ifdef	_WIN32
			m_next_camera_key.enable();
			m_previous_camera_key.enable();
			m_rearview_camera_key.enable();
			m_reset_position_key.enable();
		#endif	/* _WIN32 */

		m_cnext_camera_key.enable();
		m_cprevious_camera_key.enable();
		m_crearview_camera_key.enable();
		m_creset_position_key.enable();
	};

	inline void LogicScene::disable_play_keys()
	{
		#ifdef	_WIN32
			m_next_camera_key.disable();
			m_previous_camera_key.disable();
			m_rearview_camera_key.disable();
			m_reset_position_key.disable();
		#endif	/* _WIN32 */

		m_cnext_camera_key.disable();
		m_cprevious_camera_key.disable();
		m_crearview_camera_key.disable();
		m_creset_position_key.disable();
	};


	inline unsigned int	LogicScene::state() const							{ return m_state; };
	inline unsigned long	LogicScene::update_rate() const					{ return m_update_rate; };
	inline const char*	LogicScene::field(const std::string& value )
	{
		if ( m_source != "" )
			return m_config.get(value.c_str());
		else
			return "";
	};

	inline double		LogicScene::time_difference() const					{ return m_deltaTime; };
	inline bool			LogicScene::is_rear_down() const
	{
		#ifdef	_WIN32
			return (m_rearview_camera_key.is_down()  ||  m_crearview_camera_key.is_down()) ;
		#else
			return m_crearview_camera_key.is_down();		
		#endif	/* _WIN32 */
	};


#endif		/* XSLEDDING_LOGICSCENE_HPP_INCLUDED */