#include <sstream>
#include <iostream>
#include <string>
#include <sstream>

#include "audio/audioManager.hpp"
#include "ui/uiManager.hpp"
#include "ui/textbox.hpp"
#include "ui/container.hpp"
#include "util/ncf.hpp"
#include "util/nstring.hpp"
#include "util/scale.hpp"
#include "util/translate.hpp"
#include "util/rotate.hpp"
#include "scene/sceneManager.hpp"
#include "scene/scene.hpp"
#include "SoloScene.hpp" 
#include "MenuParams.h"
#include "message/TEXEngineInterface.hpp"
#include "GameEventCodes.h"
#include "GameLogicManager.hpp"
#include "util/calcnorm.hpp"
#include "renderer/renderManager.hpp"
#include "physics/PhysicsManager.hpp"
#include "physics/PhysicsObj.hpp"
#include "math/mutil.h"

#include "GameLogicManager.hpp"

using TEXEngine::Util::NCF1;
using TEXEngine::Util::NString::to_double;
using TEXEngine::Math::degrees_to_radians;

using TEXEngine::Physics::PhysicsObj;
using namespace TEXEngine::Core;

SoloScene::SoloScene( const unsigned long id )	:	
	LogicScene(id) , m_lapRecord(9999.0), m_lapRecordName("Default"), m_hasSentScores(false),
	camera_up(131+30,UPARROW,NONE,DEFAULT,0) , camera_down(131+31,DOWNARROW,NONE,DEFAULT,0) , camera_left(131+32,LEFTARROW,NONE,DEFAULT,0) , camera_right(131+33,RIGHTARROW,NONE,DEFAULT,0),
	camera_tiltup(131+34,HOME,NONE,DEFAULT,0) , camera_tiltdown(131+35,END,NONE,DEFAULT,0) , camera_tiltleft(131+36,KDELETE,NONE,DEFAULT,0) , camera_tiltright(131+37,PAGEDOWN,NONE,DEFAULT,0)
{
}


bool SoloScene::setup_scene( const unsigned int id , const std::string& name ){
	bool	return_value = false;


	set_scene_id(id);
	int error = TEXEngine::Core::SceneManager::get().create(get_scene_id());

	if(error){
		TEXEngine::Core::LogManager::get().log_error("couldnt create scene %i", get_scene_id());
	}else{
		TEXEngine::Core::LogManager::get().log_message("created scene %i", get_scene_id());
		TEXEngine::Asset::Scene* soloscene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
		if (soloscene){
			soloscene->name(name.c_str());
			soloscene->active_camera(1); 			

			

			soloscene->attach(10); // attaching player
			soloscene->attach(9); // attaching sled
			soloscene->attach(99); // attaching pitplane
			soloscene->attach(100); // attaching startpoint
			soloscene->attach(5); //attaching waypoint 
			soloscene->attach(998); // attaching sky



			//attaching waypoints entities
			for(unsigned int i = 101; i <= 150; ++i){
				soloscene->attach(i);
			}

			//attaching powerups entities			
			for(unsigned int i = 151; i <= 165; ++i){
				soloscene->attach(i);
			}


			return_value = true;
		}else{
			TEXEngine::Core::LogManager::get().log_error("the scene couldnt be found");
		}
	}


	return return_value;
}

SoloScene::~SoloScene()
{
}


void	SoloScene::write_record()
{
	if ( m_player  &&  m_player->best_lap() < m_lapRecord)
	{
		m_lapRecord = m_player->best_lap();
		bool hasBeatenSomeone = false;
		//for(unsigned int i = 0; i < 3 && !hasBeatenSomeone; ++i){
		for(unsigned int i = 0; i < 3; ++i){
			if(!hasBeatenSomeone && m_lapRecord > 1.0 && m_lapRecord < top3_scores[i].time){
				hasBeatenSomeone = true;
				//shift the records all records from the i-th get shifted down 1 rank
				highscore_struct temp_top3_scores[3];
				for(unsigned int j = 0; j < 3; ++j){
					temp_top3_scores[j].name = top3_scores[j].name;
					temp_top3_scores[j].time = top3_scores[j].time;
				}
				for(unsigned int j = i+1; j < 3; ++j){ //start at i+1 because we'll have our new name and score in the i-th score
					top3_scores[j].name = temp_top3_scores[j-1].name;
					top3_scores[j].time = temp_top3_scores[j-1].time;
				}


				std::stringstream recordName;
				recordName << "RecordName" << (i+1);
				std::stringstream recordValue;
				recordValue << "RecordValue" << (i+1);

				
				Textbox* tex = TEXEngine::Core::UIManager::get().get_textbox(13300);
				if(tex){
					field(recordName.str(), std::string(tex->get_text()));
					top3_scores[i].name = std::string(tex->get_text());
				}else{
					field(recordName.str(), string(""));
					top3_scores[i].name = "";
				}
				std::stringstream ssRecord;
				ssRecord << m_lapRecord;
				field(recordValue.str(), ssRecord.str());
				top3_scores[i].time = m_lapRecord;
			
				hasBeatenSomeone = true;
			}else{
				std::stringstream recordName;
				recordName << "RecordName" << (i+1);
				field(recordName.str(), top3_scores[i].name);

				std::stringstream recordValue;
				recordValue << "RecordValue" << (i+1);
				std::stringstream ssRecord;
				ssRecord << top3_scores[i].time;
				field(recordValue.str(), ssRecord.str());

			}
		}
	}


	write(); 
};


void	SoloScene::on_event( const unsigned long event_code , const TEXEngine::Core::Message& parameters )
{
	if ( event_code == update_rate() )
	{
		if ( m_player  &&  m_player->is_rear_toggled()  && !is_rear_down() )
			m_player->toggle_rear_cam();

		update();
	}
	else if ( event_code == SCENE_PAUSED )
	{
		if ( state() == SceneStateEnum_RUNNING )
			pause();
		else if ( state() == SceneStateEnum_PAUSED )
			unpause();
	}
	else if ( event_code == SCENE_QUIT )
	{
		if ( state() == SceneStateEnum_FINISHED )
		{
			TEXEngine::Core::LogManager::get().log_message("Game Log:\tTime trial game ended, switching to menu scene");
			write_record();
			cleanup();
			TEXEngine::Core::trigger_event(BACK_TO_MENU,TEXEngine::Core::Message());
		}
		else
			endgame(false);
	}
	else if ( event_code == NEXT_CAMERA )
	{
		if ( m_player )
			m_player->next_cam();
	}
	else if ( event_code == PREVIOUS_CAMERA )
	{
		if ( m_player )
			m_player->prev_cam();
	}
	else if ( event_code == REAR_VIEW )
	{
		if ( m_player )
			m_player->toggle_rear_cam();
	}
	else if ( event_code == RESET_POSITION )
	{
		if ( m_player )
			m_player->reset_to_last_waypoint();
	}
};


bool	SoloScene::init( const std::vector<string>& params , const std::vector<unsigned int>& scene_keys, const std::vector<std::vector<unsigned int> >& player_keys )
{
	bool	return_value = false;
	m_hasSentScores = false;


	if ( params.size() < 9  ||  player_keys.size() < 1  ||  
		#ifdef	_WIN32
			scene_keys.size() < 15  ||  player_keys[0].size() < 15 )
		#else
			scene_keys.size() < 5  ||  player_keys[0].size() < 5 )
		#endif	/* _WIN32 */
	{
		if ( params.size() < 9 )
			TEXEngine::Core::LogManager::get().log_error("\tScene Initialisation:\tWrong number of initialisation parameters!");

		#ifdef	_WIN32
			if ( scene_keys.size() < 15 )
		#else
			if ( scene_keys.size() < 5 )
		#endif	/* _WIN32 */
			TEXEngine::Core::LogManager::get().log_error("\tScene Initialisation:\tWrong number of scene keyboard hotkey parameters!");

		if ( player_keys.size() < 1 )
			TEXEngine::Core::LogManager::get().log_error("\tScene Initialisation:\tHotkey parameters for at least one player must be provided!");
		else
		{
			#ifdef	_WIN32
				if ( player_keys[0].size() < 15 )
			#else
				if ( player_keys[0].size() < 5 )
			#endif	/* _WIN32 */
				TEXEngine::Core::LogManager::get().log_error("\tScene Initialisation:\tWrong number of player keyboard hotkey parameters!");
		}
	}
	else
	{
		#ifdef	_WIN32
			keyboard_pause_hotkey(scene_keys[0],scene_keys[1],SCENE_PAUSED);
			keyboard_next_camera_hotkey(scene_keys[2],scene_keys[3],NEXT_CAMERA);
			keyboard_previous_camera_hotkey(scene_keys[4],scene_keys[5],PREVIOUS_CAMERA);
			keyboard_rearview_camera_hotkey(scene_keys[6],scene_keys[7],REAR_VIEW);
			keyboard_reset_position_hotkey(scene_keys[8],scene_keys[9],RESET_POSITION);
			controller_pause_hotkey(scene_keys[10],SCENE_PAUSED);
			controller_next_camera_hotkey(scene_keys[11],NEXT_CAMERA);
			controller_previous_camera_hotkey(scene_keys[12],PREVIOUS_CAMERA);
			controller_rearview_camera_hotkey(scene_keys[13],REAR_VIEW);
			controller_reset_position_hotkey(scene_keys[14],RESET_POSITION);
		#else
			controller_pause_hotkey(scene_keys[0],SCENE_PAUSED);
			controller_next_camera_hotkey(scene_keys[1],NEXT_CAMERA);
			controller_previous_camera_hotkey(scene_keys[2],PREVIOUS_CAMERA);
			controller_rearview_camera_hotkey(scene_keys[3],REAR_VIEW);
			controller_reset_position_hotkey(scene_keys[4],RESET_POSITION);
		#endif	/* _WIN32 */
		
		enable_keys();


		stringstream ss;
		ss << PATH_STUFF"data/scripts/ncf/" << params[9];
		source(ss.str());
		if ( !load() ){
		
			// CONSTRUCT THE SCENE GRAPH 
			//////////////////////////////////aa
			const float scaleValue = 250.0f;
			const float YscaleValue = 1.0f;

			TEXEngine::Asset::Scene* soloscene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
			unsigned int sceneryStartEntityIndex = 200;
			if(soloscene){
				unsigned int groundId = 96;
				if(sscanf(params[11].c_str(), "%Ld", &groundId) != EOF){
					soloscene->attach(groundId);
				}else{
					TEXEngine::Core::LogManager::get().log_message("invalid ground id passed from lua, check courses.ncf1");
				}
				if(sscanf(params[10].c_str(), "%Ld", &sceneryStartEntityIndex) == EOF){
					TEXEngine::Core::LogManager::get().log_message("invalid sceneryStartEntityIndex passed from lua, check courses.ncf1");
				}
			}else{
				TEXEngine::Core::LogManager::get().log_message("no scene found");
			}

			//billboard
			if(m_config.query_group("billboard")){
				NCF1 *billboard = m_config.group("billboard");
				Entity* e = AssetManager::get().get_entity(8);
				if(e && billboard)
				{
					Vector3f temp(scaleValue*TEXEngine::Util::NString::to_float(billboard->get("X")), 
								1+YscaleValue*TEXEngine::Util::NString::to_float(billboard->get("Y")), 
								scaleValue*TEXEngine::Util::NString::to_float(billboard->get("Z")));
					PhysicsObj* po = e->physics_obj();
					if(po){
						po->set_pos(temp);
						soloscene->attach(8);
					}
				}
			}

			//scenery
			NCF1 *scenery = m_config.group("scenery");
			if(scenery){
				const unsigned int maxScenery = 100;
				unsigned int loopMax = scenery->count_groups() > maxScenery ? maxScenery : scenery->count_groups();
				for(unsigned int i = 0; i < loopMax; ++i){
					Entity* e = AssetManager::get().get_entity(sceneryStartEntityIndex+i);
					if(e){
						Vector3f temp(scaleValue*TEXEngine::Util::NString::to_float(scenery->group(i)->get("X")), 
									1+YscaleValue*TEXEngine::Util::NString::to_float(scenery->group(i)->get("Y")), 
									scaleValue*TEXEngine::Util::NString::to_float(scenery->group(i)->get("Z")));
						PhysicsObj* po = e->physics_obj();


						if(po){
							po->set_pos(temp);
							soloscene->attach(sceneryStartEntityIndex+i);
						}
					}
				}
			}


			// reset WAYPOINTS locations
			//////////////////////////////////

			//find out scale value
			//Entity* groundEntity = AssetManager::get().get_entity(98);


			// Setting up start point and moving player to the location

			NCF1 *startpoint = m_config.group("startpoint");
			if(startpoint){
				Vector3f Temp; 
				Temp.x = scaleValue*TEXEngine::Util::NString::to_float(startpoint->get("X"));
				Temp.y = YscaleValue*TEXEngine::Util::NString::to_float(startpoint->get("Y"));
				Temp.z = scaleValue*TEXEngine::Util::NString::to_float(startpoint->get("Z"));
				Entity* e = AssetManager::get().get_entity(100);

				TEXEngine::Core::LogManager::get().log_message("STARTPOINT IN (%f,%f,%f)",Temp.x,Temp.y,Temp.z);

				if(e){
					PhysicsObj* po = e->physics_obj();
					if(po)
					{
						po->set_pos(Temp);
					}
				}

			}else TEXEngine::Core::LogManager::get().log_warning("no startpoint node in %s", m_config.get_source());

			NCF1 *waypoints = m_config.group("waypoints");
			if(waypoints){
				for (unsigned int i = 0; i < waypoints->count_groups(); ++i) {
					Vector3f Temp;
					Temp.x = scaleValue*TEXEngine::Util::NString::to_float(waypoints->group(i)->get("X"));
					Temp.y = YscaleValue*TEXEngine::Util::NString::to_float(waypoints->group(i)->get("Y"));
					Temp.z = scaleValue*TEXEngine::Util::NString::to_float(waypoints->group(i)->get("Z"));
					Entity* e = AssetManager::get().get_entity(101+i);

					TEXEngine::Core::LogManager::get().log_message("WAYPOINT %i IN (%f,%f,%f)",i+1,Temp.x,Temp.y,Temp.z);

					if( i == 0){
						//position the marker 
						Entity* markerEntity = AssetManager::get().get_entity(5);
						if(markerEntity){
							PhysicsObj* po = markerEntity->physics_obj();
							if(po){
								/*Matrix4x4f OriginMat; 
								OriginMat.load_identity();
								po->set_transform_matrix(OriginMat);*/
								Vector3f Temp2(Temp.x, Temp.y, Temp.z);
								po->set_pos(Temp2);
							}
						}
					}

					if(e){
						PhysicsObj* po = e->physics_obj();
						if(po){
							/*Matrix4x4f OriginMat; 
							OriginMat.load_identity();
							po->set_transform_matrix(OriginMat);*/
							po->set_pos(Temp);
						}
					}
				}
			}else TEXEngine::Core::LogManager::get().log_warning("no waypoints node in %s", m_config.get_source());



			// FILL POWERUP LOCATIONS
			/////////////////////////////////

			NCF1 *Powerups = m_config.group("powerups");

			if (Powerups) {
				GameLogicManager::Get().clear_powerup_queue();
				for (unsigned int i = 0; i < Powerups->count_groups(); ++i) {
					Vector3f Temp; 
					Temp.x = scaleValue*TEXEngine::Util::NString::to_float(Powerups->group(i)->get("X"));
					Temp.y = YscaleValue*TEXEngine::Util::NString::to_float(Powerups->group(i)->get("Y"));
					Temp.z = scaleValue*TEXEngine::Util::NString::to_float(Powerups->group(i)->get("Z"));

					GameLogicManager::Get().push_powerup(Temp);
				}
				for(unsigned int i = 0; i < 15; ++i){
					Entity* e = AssetManager::get().get_entity(151+i);
					if(e){
						PhysicsObj* po = e->physics_obj();
						if(po){
							Vector3f Temp = GameLogicManager::Get().front_powerup();
							po->set_pos(Temp);
							GameLogicManager::Get().pop_and_add_powerup(Temp);
						}
					}
				}
			}
			
			if(soloscene &&  m_player ){
				if(!params[3].compare(SPECIES_PROPERTY_DOG))
				{
					soloscene->attach(11);
					m_player->set_entityPack(11);
				}
				else if(!params[3].compare(SPECIES_PROPERTY_MOUSE))
				{
					soloscene->attach(12);
					m_player->set_entityPack(12);
				}
				else if(!params[3].compare(SPECIES_PROPERTY_DINO))
				{
					soloscene->attach(13);
					m_player->set_entityPack(13);
				}
				else TEXEngine::Core::LogManager::get().log_error("can't add a pack entity: unrecognized species");
			}else TEXEngine::Core::LogManager::get().log_error(" scene not found");


			TEXEngine::Core::SceneManager::get().activate(get_scene_id());
			


			//top3_scores[0].name
			/*
			std::string tempString = "RecordValue";
			//m_config.get(value.c_str());
			if(m_config.query_property(tempString.c_str())){
				m_lapRecord = to_double(field(tempString)); 
			}

			m_lapRecordName = field(tempString); 
			*/			
			for(unsigned int i = 0; i < 3; ++i){
				std::stringstream recordName;
				recordName << "RecordName" << (i+1);
				std::stringstream recordValue;
				recordValue << "RecordValue" << (i+1);
				if(m_config.query_property(recordName.str().c_str()) && m_config.query_property(recordValue.str().c_str())){
					top3_scores[i].name = field(recordName.str());
					top3_scores[i].time = to_double(field(recordValue.str()));
				}
			}

			std::vector<std::string>	AnimalNames(4,std::string(""));
			AnimalNames[0] = params[4];
			AnimalNames[1] = params[5];
			AnimalNames[2] = params[6];
			AnimalNames[3] = params[7];
		
			if ( m_player )
			{
				m_player->set_entity(10);
				m_player->set_entitySled(9);
				m_player->init(999,5, params[0], AnimalNames, params[3], 1, true);

				#ifdef	_WIN32
					m_player->init_controls(0,player_keys[0][0],player_keys[0][2],player_keys[0][4],player_keys[0][6],player_keys[0][8],
											player_keys[0][1],player_keys[0][3],player_keys[0][5],player_keys[0][7],player_keys[0][9],
											player_keys[0][10],player_keys[0][11],player_keys[0][12],player_keys[0][13],player_keys[0][14]);
				#else
					m_player->init_controls(0,player_keys[0][0],player_keys[0][1],player_keys[0][2],player_keys[0][3],player_keys[0][4]);
				#endif	/* _WIN32 */

				m_player->set_state(StateEnum_READY);
				m_player->send_stats();
				m_player->disable_controls();

				PhysicsObj* sledPO = m_player->m_entity->physics_obj();
				Entity* startPointEntity = AssetManager::get().get_entity(100);
				if(startPointEntity){
					PhysicsObj* startPO = startPointEntity->physics_obj();
					if(sledPO && startPO)
					{
						Matrix4x4f tmp(startPO->get_transform_matrix());
						Vector3f v(startPO->get_pos());

						v.x -= 1.0f;
						sledPO->set_transform_matrix(tmp);
						sledPO->set_pos(v);
						sledPO->reset_velocities();
						m_player->m_resetTransform = sledPO->get_transform_matrix();
					}
				}
				m_player->m_pack.reset_pack();

				// PLAY AUDIO CLIP "READY?"
				start_go_timer();
				state(SceneStateEnum_RUNNING);

				start_updating();
				register_event(SCENE_PAUSED);
				register_event(SCENE_QUIT);
				register_event(NEXT_CAMERA);
				register_event(PREVIOUS_CAMERA);
				register_event(REAR_VIEW);
				register_event(RESET_POSITION);

				TEXEngine::Core::LogManager::get().log_message("Game Log:\tTime trial game initialised.");
				Camera *cam = TEXEngine::AssetManager::get().get_camera(1);
				if (cam)
				{
					cam->set_camera_type(TEXENG_CAMERA_CHASE);
					cam->set_chase_cam_position(Vector3f(0.0f, 5.0f, 20.0f));
					if (sledPO) 
					{
						Vector3f pos = sledPO->get_pos();
					
						cam->target(Vector3f(pos.x,pos.y+1.0f,pos.z));
						cam->up(Vector3f(0.0f, 1.0f, 0.0f));
					}
				}

				TEXEngine::Util::ColorRGBAf ClearColour;
				ClearColour.blue = 1.0f;
				ClearColour.red = 0.0f; 
				ClearColour.green = 0.7f; 
				TEXEngine::Core::RenderManager::get().clear_color(ClearColour);

				Entity *e = TEXEngine::AssetManager::get().get_entity(999);
				if (e)
				{
					Matrix4x4f m;
					m.load_identity();
					m.translate(Vector3f(0.0f, 3.0f, 0.0f));
					e->matrix(m);
				}
			
				soloscene->attach(999); //attaching water
				return_value = true;
			}

			#ifdef _WIN32

				unsigned int audioId = 103;
				if(sscanf(params[12].c_str(), "%Ld", &audioId) != EOF){
					AudioManager::get().stop_music();
					AudioManager::get().play_audio(AssetManager::get().get_audioclip(audioId));
				}else{
					TEXEngine::Core::LogManager::get().log_message("invalid audio id passed from lua, check courses.ncf1");
				}


			#endif //_WIN32
			//check for godmode
			unsigned int godmode = 0;
			if(sscanf(params[13].c_str(), "%Ld", &godmode) != EOF){
				GameLogicManager::Get().set_francis_hack(godmode > 0 ? true : false);
			}else{
				TEXEngine::Core::LogManager::get().log_message("invalid audio id passed from lua, check courses.ncf1");
			}


		}
	}


	return return_value;
}

void SoloScene::update_powerupsRotation(const double d){

	TEXEngine::Asset::Scene* soloscene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
	if(soloscene){
		for(unsigned int i = 151; i <= 165; ++i){
			Entity* e = AssetManager::get().get_entity(i);
			if(e){
				Matrix4x4f m = e->matrix();
				m.rotate(Vector3f(0.0f, d, 0.0f));
				e->matrix(m);
			}
		}
	}
	
}

void SoloScene::update_waypointmarker(const double d){
	static double markerDeviation = 0.3;
	static double currentDeviation = 0.0;
	static bool goingDown = true;//that's what she wants

	Entity* markerEntity = AssetManager::get().get_entity(5);
	if(markerEntity){
		PhysicsObj* po = markerEntity->physics_obj();
		if(po){
			Vector3f Temp = po->get_pos();
			Temp.y += currentDeviation;
			po->set_pos(Temp);
			if(goingDown){
				currentDeviation-=d;
				if(currentDeviation < -markerDeviation){
					goingDown = !goingDown;
					currentDeviation = -markerDeviation;
				}
			}else{
				currentDeviation+=d;
				if(currentDeviation > markerDeviation){
					goingDown = !goingDown;
					currentDeviation = markerDeviation;
				}
			}
		}
	}
}

void	SoloScene::update()
{
	if (state() == SceneStateEnum_RUNNING )
	{
		update_time();

		if ( m_player )
		{
			if (m_player->state() == StateEnum_READY)
			{
				if (m_goTimer.get_time_in_sec() > 3.0)
				{
					// PLAY AUDIO CLIP "GO!"
					m_player->set_state(StateEnum_RACING);
					enable_player_keys();
					stop_go_timer();
					start_timer();
					TEXEngine::Core::LogManager::get().log_message("Game Log:\tGame started.");
				}
			}
			else
			{
				m_player->update(time_difference());
				update_waypointmarker(time_difference());
				update_powerupsRotation(time_difference());

				m_player->update_camera();
			}

			calculate_and_send_position();
		}

		TEXEngine::Core::PhysicsManager::get().update(time_difference());
	}

	
	// DEBUG CAMERA
	//////////////////////
	/*
	Camera *cam = TEXEngine::AssetManager::get().get_camera(1);
	cam->set_camera_type(TEXEngine::Asset::TEXENG_CAMERA_FPS);

	if (cam)
	{
		if (camera_up.is_down())
			cam->move_forward(25.0f * time_difference());
		if (camera_down.is_down())
			cam->move_forward(-25.0f * time_difference());
		if (camera_left.is_down())
			cam->strafe(-25.0f * time_difference());
		if (camera_right.is_down())
			cam->strafe(25.0f * time_difference());
		if (camera_tiltup.is_down())
			cam->set_pitch(20.0f * time_difference());
		if (camera_tiltdown.is_down())
			cam->set_pitch(-20.0f * time_difference());
		if (camera_tiltleft.is_down())
			cam->set_yaw(-20.0f * time_difference());
		if (camera_tiltright.is_down())
			cam->set_yaw(20.0f * time_difference()); 
	}*/
}

void	SoloScene::pause()
{
	if ( m_player  &&  m_player->state() == StateEnum_READY )
		stop_go_timer();
	else
		stop_timer();


	state(SceneStateEnum_PAUSED);	
	disable_player_keys();


	TEXEngine::Core::LogManager::get().log_message("Game Log:\tGame paused.");
	Container* pauseContainer = TEXEngine::Core::UIManager::get().get_container(14000);
	if(pauseContainer){
		pauseContainer->set_visible(true);
		if(!m_hasSentScores){
			m_hasSentScores = true;
			Message msg(0, 3, 3);
			for(unsigned int i = 0; i < 3; ++i){
				msg.add_real_data(i, top3_scores[i].time);
				msg.add_pointer(i, &top3_scores[i].name);
			}
			trigger_event(SEND_SCORES, msg);
		}
	}

	TEXEngine::Core::trigger_event(SCENE_ISPAUSED,TEXEngine::Core::Message(0,0,0));
	TEXEngine::Core::trigger_event(ENABLE_MENU_KEYS,TEXEngine::Core::Message(0,0,0));
}

void	SoloScene::unpause()
{
	if ( m_player  &&  m_player->state() == StateEnum_READY )
		resume_go_timer();
	else
		start_timer();


	state(SceneStateEnum_RUNNING);
	enable_player_keys();


	TEXEngine::Core::LogManager::get().log_message("Game Log:\tGame resumed.");
	TEXEngine::Core::trigger_event(SCENE_ISRESUMED,TEXEngine::Core::Message(0,0,0));
	TEXEngine::Core::trigger_event(DISABLE_MENU_KEYS,TEXEngine::Core::Message(0,0,0));
}

void	SoloScene::endgame( bool finished )
{
	TEXEngine::Core::Message	parameters(4,2,0);



	parameters.add_integral_data(0,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(get_id()));
	unsigned int lap = m_player->lap_number();
	if ( m_player  && m_player->lap_number() > 1 )
		parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(1));
	else
		parameters.add_integral_data(2,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(0));
	parameters.add_integral_data(3,static_cast<TEXEngine::Core::MESSAGE_INTEGRAL_DATA_TYPE>(1));

	if ( m_player )
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(m_player->best_lap()));
	else
		parameters.add_real_data(0,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(0.0f));
	parameters.add_real_data(1,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(m_lapRecord));



	state(SceneStateEnum_FINISHED);
	
	disable_player_keys();
	GameLogicManager::Get().enable_menu_keys();

	if ( m_player )
		m_player->set_state(StateEnum_FINISHED);


	TEXEngine::Core::LogManager::get().log_message("Game Log:\tGame finished, switching to the finished screen");
	TEXEngine::Core::trigger_event(SCENE_FINISHED,parameters);
}

void	SoloScene::reset()
{
}

void	SoloScene::cleanup()
{
	state(SceneStateEnum_QUIT);

	stop_timer();
	stop_go_timer();

	disable_keys();

	disable_player_keys();

	stop_updating();
	TEXEngine::Core::RenderManager::get().clear_color(TEXEngine::Util::ColorRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
	TEXEngine::Asset::Scene* soloscene = TEXEngine::Core::SceneManager::get().scene(get_scene_id());
	if(soloscene){
		soloscene->detach(11);
		soloscene->detach(12);
		soloscene->detach(13);
		soloscene->detach(92);
		soloscene->detach(93);
		soloscene->detach(94);
		soloscene->detach(95);
		soloscene->detach(96);
		soloscene->detach(97);
		soloscene->detach(98);
		soloscene->detach(999);
		//detach scenery
		for(unsigned int i = 0; i < 700; ++i){
			soloscene->detach(200+i);
		}
	}else{
		TEXEngine::Core::LogManager::get().log_warning("Won't detach. Reason: The scene was not found!");
	}

	TEXEngine::Core::SceneManager::get().activate(0); 
	unregister_event(SCENE_PAUSED);
	unregister_event(SCENE_QUIT);
}
