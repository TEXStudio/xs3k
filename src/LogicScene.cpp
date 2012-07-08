#include	"LogicScene.hpp"


HumanRacer*	LogicScene::m_player = NULL;


LogicScene::LogicScene( const unsigned long id )	:	
	TEXEngine::Core::Object(id) , 
	m_source("") , 
	m_deltaTime (0.0) , m_previousTime (0.0) , 
	m_update_rate(TEXEngine::Core::UPDATE) , 
	m_state (SceneStateEnum_RESET) , 
	m_scene_id(1)
{
	#ifdef	_WIN32
		m_pauseRaceKey.disable();
		m_next_camera_key.disable();
		m_previous_camera_key.disable();
		m_reset_position_key.disable();
	#endif	/* _WIN32 */
			
	m_cpauseRaceKey.disable();
	m_cnext_camera_key.disable();
	m_cprevious_camera_key.disable();
	m_creset_position_key.disable();
};

LogicScene::~LogicScene()	{ disable_keys(); };