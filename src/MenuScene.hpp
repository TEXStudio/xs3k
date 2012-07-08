#ifndef XSLEDDING_MENUSCENE_HPP_INCLUDED
#define XSLEDDING_MENUSCENE_HPP_INCLUDED

#include "LogicScene.hpp"

class MenuScene : public LogicScene{
private: 


public:
	MenuScene( const unsigned long id = 0 );
	~MenuScene();	
	void on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters );
	bool init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys );
	void update();
	void pause();
	void unpause();
	void endgame( bool finished );
	void reset();
	void cleanup();
	bool setup_scene( const unsigned int id = 1 , const std::string& name = std::string("") );
	void enable_player_keys();
	void disable_player_keys();
	Racer* get_player();
};


inline bool MenuScene::setup_scene( const unsigned int id  , const std::string& name ) { return true; };
inline void MenuScene::on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters ) {};
inline bool MenuScene::init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys ) { return true; };
inline void MenuScene::update() {};
inline void MenuScene::pause() {};
inline void MenuScene::unpause() {};
inline void MenuScene::endgame( bool finished ) {};
inline void MenuScene::reset() {};
inline void MenuScene::cleanup() {};
inline void MenuScene::enable_player_keys() {};
inline void MenuScene::disable_player_keys() {};
inline Racer* MenuScene::get_player() {return NULL;}
#endif