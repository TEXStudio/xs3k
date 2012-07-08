#include <string>

#include "ConsoleCommand.hpp"

#include "asset/assetManager.hpp"
#include "log/logmanager.hpp"
#include "message/TEXEngineInterface.hpp"
#include "physics/PhysicsManager.hpp"
#include "physics/PhysicsObj.hpp"
#include "scene/sceneManager.hpp"
#include "scene/scene.hpp"
#include "renderer/renderManager.hpp"
#include "ui/uiManager.hpp"
#include "ui/textbox.hpp"
#include "util/color.hpp"

#include "GameLogicManager.hpp"
#include "GameEventCodes.h"

#ifdef	_WIN32
	#include <cstdio>
#else
	#include <stdio.h>
#endif


ConsoleCommand::ConsoleCommand(){
}


ConsoleCommand::~ConsoleCommand(){
}

ClearCommand::ClearCommand(){

}

ClearCommand::~ClearCommand(){

}

int ClearCommand::execute(const char* params){
	TEXEngine::Core::LogManager::get().clear();
	GameLogicManager::Get().update_console();
	return 0;
}

QuitCommand::QuitCommand(){

}

QuitCommand::~QuitCommand(){

}

int QuitCommand::execute(const char* params){
	TEXEngine::Core::LogManager::get().log_message("quitting");
	TEXEngine::Core::trigger_event(QUIT_GAME, TEXEngine::Core::Message(0, 0, 0));
	return 0;
}

DebugRenderingCommand::DebugRenderingCommand(){

}

DebugRenderingCommand::~DebugRenderingCommand(){

}

int DebugRenderingCommand::execute(const char* params){
	std::string p(params);
	int ret = 0;
	if(p.size() == 0){
		bool current = !TEXEngine::Core::PhysicsManager::get().get_enable_debug_render();
		TEXEngine::Core::PhysicsManager::get().set_enable_debug_render(current);
		if(current){
			TEXEngine::Core::LogManager::get().log_message("debug renderer enabled");
		}else{
			TEXEngine::Core::LogManager::get().log_message("debug renderer disabled");
		}
		return 0;
	}else{
		if(!p.compare("1") || !p.compare("true")){
			TEXEngine::Core::PhysicsManager::get().set_enable_debug_render(true);
			TEXEngine::Core::LogManager::get().log_message("debug renderer enabled");
		}else if(!p.compare("0") || !p.compare("false")){
			TEXEngine::Core::PhysicsManager::get().set_enable_debug_render(false);
			TEXEngine::Core::LogManager::get().log_message("debug renderer disabled");
		}
		else ret = 1;
	}

	return ret;
}

TreatHackCommand::TreatHackCommand(){

}

TreatHackCommand::~TreatHackCommand(){

}

int TreatHackCommand::execute(const char* params){
	unsigned int v;
	if(sscanf(params, "%Ld", &v) == EOF){
		return 1;
	}
	GameLogicManager::Get().cheat_set_treats(v);
	TEXEngine::Core::LogManager::get().log_message("set treats to %i", v);
	return 0;
}

ClearColorCommand::ClearColorCommand(){

}

ClearColorCommand::~ClearColorCommand(){

}

int ClearColorCommand::execute(const char* params){
	float f1, f2, f3, f4;
	if(sscanf(params, "%f %f %f %f", &f1, &f2, &f3, &f4) == EOF){
		if(sscanf(params, "%f %f %f", &f1, &f2, &f3) == EOF){
			return 1;
		}else f4 = 1.0f;
	}

	TEXEngine::Core::RenderManager::get().clear_color(TEXEngine::Util::ColorRGBAf(f1, f2, f3, f4));
	TEXEngine::Core::LogManager::get().log_message("set clear color to %f,%f,%f,%f", f1, f2, f3, f4);
	return 0;
}

ConsoleBackgroundColorCommand::ConsoleBackgroundColorCommand(){

}

ConsoleBackgroundColorCommand::~ConsoleBackgroundColorCommand(){

}

int ConsoleBackgroundColorCommand::execute(const char* params){
	float f1, f2, f3, f4;
	if(sscanf(params, "%f %f %f %f", &f1, &f2, &f3, &f4) == EOF){
		if(sscanf(params, "%f %f %f", &f1, &f2, &f3) == EOF){
			return 1;
		}else f4 = 1.0f;
	}

	TEXEngine::UI::Textbox* console = TEXEngine::Core::UIManager::get().get_textbox(16100);
	if(console){
		console->set_background(TEXEngine::Util::ColorRGBAf(f1, f2, f3, f4));
		TEXEngine::Core::LogManager::get().log_message("set console bg color to %f,%f,%f,%f", f1, f2, f3, f4);
		return 0;
	}

	return 2;
}


NoMechanicsCommand::NoMechanicsCommand(){

}

NoMechanicsCommand::~NoMechanicsCommand(){

}

int NoMechanicsCommand::execute(const char* params){
	std::string p(params);
	int ret = 0;
	if(p.size() == 0){
		bool current = !GameLogicManager::Get().get_francis_hack();
		GameLogicManager::Get().set_francis_hack(current);
		if(current){
			TEXEngine::Core::LogManager::get().log_message("pack gameplay mechanics enabled");
		}else{
			TEXEngine::Core::LogManager::get().log_message("pack gameplay mechanics disabled");
		}
		return 0;
	}else{
		if(!p.compare("1") || !p.compare("true")){
			GameLogicManager::Get().set_francis_hack(true);
			TEXEngine::Core::LogManager::get().log_message("pack gameplay mechanics enabled");
		}else if(!p.compare("0") || !p.compare("false")){
			GameLogicManager::Get().set_francis_hack(false);
			TEXEngine::Core::LogManager::get().log_message("pack gameplay mechanics disabled");
		}
		else ret = 1;

	}

	return ret;
}


PlayerPositionCommand::PlayerPositionCommand(){

}

PlayerPositionCommand::~PlayerPositionCommand(){

}

int PlayerPositionCommand::execute(const char* params){
	std::string s(params);
	if(s.size() == 0){
		TEXEngine::Asset::Entity* vehicle = TEXEngine::Core::AssetManager::get().get_entity(10);
		if(vehicle){
			TEXEngine::Physics::PhysicsObj* po = vehicle->physics_obj();
			if(po){
				TEXEngine::Math::Vector3f pos = po->get_pos();
				TEXEngine::Core::LogManager::get().log_message("player position is: %f %f %f", pos.x, pos.y, pos.z);
				return 0;
			}
		}
	}else{
		float f1, f2, f3;
		if(sscanf(params, "%f %f %f", &f1, &f2, &f3) == EOF){
			return 1;
		}
		TEXEngine::Asset::Entity* vehicle = TEXEngine::Core::AssetManager::get().get_entity(10);
		if(vehicle){
			TEXEngine::Physics::PhysicsObj* po = vehicle->physics_obj();
			if(po){
				po->set_pos(TEXEngine::Math::Vector3f(f1, f2, f3));
				TEXEngine::Core::LogManager::get().log_message("set player position to: %f %f %f", f1, f2, f3);
				return 0;
			}
		}
	}
	return 2;
}

AIPositionCommand::AIPositionCommand(){

}

AIPositionCommand::~AIPositionCommand(){

}

int AIPositionCommand::execute(const char* params){
	std::string s(params);
	
	float f1, f2, f3;
	if(sscanf(params, "%f %f %f", &f1, &f2, &f3) == EOF){
		return 1;
	}
	TEXEngine::Asset::Entity* vehicle = TEXEngine::Core::AssetManager::get().get_entity(24);
	if(vehicle){
		TEXEngine::Physics::PhysicsObj* po = vehicle->physics_obj();
		if(po){
			po->set_pos(TEXEngine::Math::Vector3f(f1, f2, f3));
			TEXEngine::Core::LogManager::get().log_message("set bot position to: %f %f %f", f1, f2, f3);
			return 0;
		}
	}
	return 2;
}
