#include <string>

#include "MenuScene.hpp"

#include "message/message.hpp"
#include "message/TEXEngineInterface.hpp"
#include "ui/uiUtility.hpp"
#include "util/ncf.hpp"
#include "MenuParams.h"
#include "GameEventCodes.h"
#include "ui/uiManager.hpp"
#include "ui/container.hpp"

using namespace TEXEngine::Core;
using namespace TEXEngine::UI;
using namespace std;

MenuScene::MenuScene( const unsigned long id ) : LogicScene(id){

}

MenuScene::~MenuScene(){

}

